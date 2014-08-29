/*
 * Copyright (c) 2014, Paul-Darius Sarmadi <paul-darius.sarmadi@telecom-sudparis.eu>
 * Based on "mosaic.cpp" by Nicolais Limare <nicolas.limare@cmla.ens-cachan.fr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdemosaicking.h"
#include "mex.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{	
    if (nlhs!=1)
    {
        mexErrMsgTxt("One matrix is needed as an output. Type \"help SSDD\" for more informations");
    }
    if (nrhs!=2)
    {
        mexErrMsgTxt("One matrix and one string are needed as input. Type \"help SSDD\" for more informations");
    }
    if (!(mxIsDouble(prhs[0])))
    {
        mexErrMsgTxt("The first input must be a double matrix. Type \"help SSDD\" for more informations");
    }
    else
    {
    int nx=mxGetN(prhs[0])/3;
    int ny=mxGetM(prhs[0]);

    size_t i, j;
    char* pattern_str;
    size_t buflen;
    int status;
    buflen = mxGetN(prhs[1])*sizeof(char)+1;
    pattern_str = (char*)malloc(buflen);
    status = mxGetString(prhs[1], pattern_str, (mwSize)buflen);

    unsigned char redx, redy;
    float *data_out;
    float *out_ptr, *end_ptr;
	
    /* pattern */

    if (0 == strcmp("RGGB", pattern_str))
    {
		redx = 0;
		redy = 0;
    }
    else if (0 == strcmp("GRBG", pattern_str))
    {
		redx = 1;
		redy = 0;
    }
    else if (0 == strcmp("GBRG", pattern_str))
    {
		redx = 0;
		redy = 1;
    }
    else if (0 == strcmp("BGGR", pattern_str))
    {
		redx = 1;
		redy = 1;
    }
    else
    {
        mexErrMsgTxt("pattern must be \"RGGB\", \"GRBG\", \"GBRG\" or \"BGGR\"\n");
    }
    float* data_in= new float[nx*ny*3];
    int tmpx, tmpy, tmpz;
    for (tmpz=0;tmpz<3;tmpz++)
        for (tmpx=0; tmpx<nx; tmpx++)
            for (tmpy=0; tmpy<ny; tmpy++)
                data_in[tmpx+tmpy*nx+tmpz*nx*ny] = (float)(mxGetPr(prhs[0]))[tmpy+tmpx*ny+tmpz*nx*ny];
    data_out = (float *) malloc(sizeof(float) * nx * ny * 3);

    /* process */	
	ssd_demosaicking_chain(redx, redy,
                      data_in, data_in + nx * ny, data_in + 2 * nx * ny,
                      data_out, data_out + nx * ny, data_out + 2 * nx * ny,
                      (int) nx, (int) ny);
    /* limit to 0-255 */
    out_ptr = data_out;
    end_ptr = out_ptr + 3 * nx * ny;

    while (out_ptr < end_ptr)
    {
		if ( 0 > *out_ptr)
			*out_ptr = 0;
		if ( 255 < *out_ptr)
			*out_ptr = 255;
		out_ptr++;
    }
        mwSize dim = 3;
        const mwSize dims[3]={ny,nx,3};
        plhs[0]=mxCreateNumericArray(dim, dims,mxDOUBLE_CLASS,mxREAL);
        double* pointeur=(double*)mxGetPr(plhs[0]);
        for (tmpz=0;tmpz<3;tmpz++)
            for (tmpy=0; tmpy<ny; tmpy++)
                for (tmpx=0; tmpx<nx; tmpx++)
                {
                    pointeur[tmpy+tmpx*ny+tmpz*nx*ny]=(double)data_out[tmpx+tmpy*nx+tmpz*nx*ny];
                }
        free(data_out);
        free(pattern_str);
    }
}
