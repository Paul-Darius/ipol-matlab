typedef char* char16_t;
#include "mex.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#ifndef DISABLE_OMP
//#include <omp.h>
#endif//DISABLE_OMP

//#include "../../iio.h"

#include "../../tvl1flow_lib.c"


#define PAR_DEFAULT_OUTFLOW "flow.flo"
#define PAR_DEFAULT_NPROC   0
#define PAR_DEFAULT_TAU     0.25
#define PAR_DEFAULT_LAMBDA  0.15
#define PAR_DEFAULT_THETA   0.3
#define PAR_DEFAULT_NSCALES 100
#define PAR_DEFAULT_ZFACTOR 0.5
#define PAR_DEFAULT_NWARPS  5
#define PAR_DEFAULT_EPSILON 0.01
#define PAR_DEFAULT_VERBOSE 0


/**
 *
 *  Function to read images using the iio library
 *  It always returns an allocated the image.
 *
 */

/*
static float *read_image(const char *filename, int *w, int *h)
{
	float *f = iio_read_image_float(filename, w, h);
	if (!f)
		fprintf(stderr, "ERROR: could not read image from file "
				"\"%s\"\n", filename);
	return f;
}

*/

/**
 *
 *  Main program:
 *   This program reads the following parameters from the console and
 *   then computes the optical flow:
 *   -nprocs      number of threads to use (OpenMP library)
 *   -I0          first image
 *   -I1          second image
 *   -tau         time step in the numerical scheme
 *   -lambda      data term weight parameter
 *   -theta       tightness parameter
 *   -nscales     number of scales in the pyramidal structure
 *   -zfactor     downsampling factor for creating the scales
 *   -nwarps      number of warps per scales
 *   -epsilon     stopping criterion threshold for the iterative process
 *   -out         name of the output flow field
 *   -verbose     switch on/off messages
 *
 */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (!(nlhs==1))
	{
    	mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs","One output required.");
    }
    if (!(nrhs==2 || nrhs==3))
    {
    	mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs","Two double matrix and one optional structure are required.");
    }

	int i = 1;
	int   nproc   = PAR_DEFAULT_NPROC;
	float tau     = PAR_DEFAULT_TAU;
	float lambda  = PAR_DEFAULT_LAMBDA;
	float theta   = PAR_DEFAULT_THETA;
	int   nscales = PAR_DEFAULT_NSCALES;
	float zfactor = PAR_DEFAULT_ZFACTOR;
	int   nwarps  = PAR_DEFAULT_NWARPS;
	float epsilon = PAR_DEFAULT_EPSILON;
	int   verbose = PAR_DEFAULT_VERBOSE;
	if (nrhs==3)
    {
      int tmp=0;
      double* ptr;
      for( tmp=0; tmp<mxGetNumberOfFields(prhs[2]);tmp++)
      {
        if ( strcmp(mxGetFieldNameByNumber(prhs[2],tmp),"nproc")==0)
        {
          if (!(mxIsDouble(mxGetField(prhs[2],0,mxGetFieldNameByNumber(prhs[2],0)))))
          {
            mexErrMsgTxt("A double argument was expected.");
          }
          if (mxGetNumberOfElements((mxGetField(prhs[2],0,mxGetFieldNameByNumber(prhs[2],0))))!=1)
          {
            mexErrMsgTxt("Only one value was expected.");
          }
          ptr=mxGetPr(mxGetField(prhs[2],0,mxGetFieldNameByNumber(prhs[2],tmp)));
          nproc=ptr[0];
          //else
          //{
          //  mexErrMsgTxt("The nproc value is not acceptable. For more information, type \"help tvl1\"");
          //}
        }
      }
	}


	if (nproc < 0) {
		nproc = PAR_DEFAULT_NPROC;
		if (verbose) fprintf(stderr, "warning: "
				"nproc changed to %d\n", nproc);
	}
	if (tau <= 0 || tau > 0.25) {
		tau = PAR_DEFAULT_TAU;
		if (verbose) fprintf(stderr, "warning: "
				"tau changed to %g\n", tau);
	}
	if (lambda <= 0) {
		lambda = PAR_DEFAULT_LAMBDA;
		if (verbose) fprintf(stderr, "warning: "
				"lambda changed to %g\n", lambda);
	}
	if (theta <= 0) {
		theta = PAR_DEFAULT_THETA;
		if (verbose) fprintf(stderr, "warning: "
				"theta changed to %g\n", theta);
	}
	if (nscales <= 0) {
		nscales = PAR_DEFAULT_NSCALES;
		if (verbose) fprintf(stderr, "warning: "
				"nscales changed to %d\n", nscales);
	}
	if (zfactor <= 0 || zfactor >= 1) {
		zfactor = PAR_DEFAULT_ZFACTOR;
		if (verbose) fprintf(stderr, "warning: "
				"zfactor changed to %g\n", zfactor);
	}
	if (nwarps <= 0) {
		nwarps = PAR_DEFAULT_NWARPS;
		if (verbose) fprintf(stderr, "warning: "
				"nwarps changed to %d\n", nwarps);
	}
	if (epsilon <= 0) {
		epsilon = PAR_DEFAULT_EPSILON;
		if (verbose) fprintf(stderr, "warning: "
				"epsilon changed to %f\n", epsilon);
	}
/*
#ifndef DISABLE_OMP
	if (nproc > 0)
		omp_set_num_threads(nproc);
#endif//DISABLE_OMP
*/

	int    nx = mxGetN(prhs[0])/3;
	int    ny = mxGetM(prhs[0]);
	int    nx2 = mxGetN(prhs[1])/3;
	int    ny2 = mxGetM(prhs[1]);
	mexPrintf("%d %d", mxGetN(prhs[0]), nx);
	float *I0=malloc(3*nx*ny*sizeof(float));
	int tmp=0;
	for (tmp=0; tmp<3*nx*ny; tmp++)
	{	
		I0[tmp] = (float)(mxGetPr(prhs[0]))[tmp];
	}
	float *I1=malloc(3*nx2*ny2*sizeof(float));
	tmp=0;
	for (tmp=0; tmp<3*nx2*ny2; tmp++)
	{	
		I0[tmp] = (float)(mxGetPr(prhs[1]))[tmp];
	}
		if (nx == nx2 && ny == ny2)
	{

		const float N = 1 + log(hypot(nx, ny)/16.0) / log(1/zfactor);
		if (N < nscales)
			nscales = N;
		if (verbose)
		{
			mexPrintf(
				"nproc=%d tau=%f lambda=%f theta=%f nscales=%d "
				"zfactor=%f nwarps=%d epsilon=%g\n",
				nproc, tau, lambda, theta, nscales,
				zfactor, nwarps, epsilon);
		}

		float *u = xmalloc(2 * nx * ny * sizeof(*u));
		float *v = u + nx*ny;

		mexPrintf("%d %d\n", nx, nx2);

		Dual_TVL1_optic_flow_multiscale(
				I0, I1, u, v, nx, ny, tau, lambda, theta,
				nscales, zfactor, nwarps, epsilon, verbose
		);


		//iio_save_image_float_split(outfile, u, nx, ny, 2);
		mwSize dim = 3;
    	const mwSize dims[3]={ny,nx,3};
    	plhs[0]=mxCreateNumericArray(dim, dims,mxDOUBLE_CLASS,mxREAL);
    	double* pointeur=(double*)mxGetPr(plhs[0]);
    	int k=0;
    	for (k=0;k<3*nx*ny;k++)
    	{
        	//pointeur[k]=(double)u[k];
        	//mexPrintf("%e\n", u[k]);

    	}
		free(I0);
		free(I1);
		free(u);
	} 
	else
	{
		mexErrMsgTxt("ERROR: input images size mismatch ");
	}
}
