/*
 * Copyright (c) 2014, Paul-Darius Sarmadi <paul-darius.sarmadi@telecom-sudparis.eu>
 * Based on "mosaic.cpp" by Nicolais Limare <nicolas.limare@cmla.ens-cachan.fr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
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
        int mask_R[4] = {0, 0, 0, 0};
        int mask_G[4] = {0, 0, 0, 0};
        int mask_B[4] = {0, 0, 0, 0};
        size_t i, j;
        char* buf;
        size_t buflen;
        int status;
        buflen = mxGetN(prhs[1])*sizeof(char)+1;
        buf = (char*)malloc(buflen);
        status = mxGetString(prhs[1], buf, (mwSize)buflen);

        /* get the pattern */

        if (0 == strcmp("RGGB", buf))
        {
	       mask_R[0] = 1;
	       mask_G[1] = 1;
    	mask_G[2] = 1;
	   mask_B[3] = 1;
       }
       else if (0 == strcmp("GRBG", buf))
       {
	       mask_G[0] = 1;
	      mask_R[1] = 1;
	       mask_B[2] = 1;
	       mask_G[3] = 1;
       }
       else if (0 == strcmp("GBRG", buf))
        {
	       mask_G[0] = 1;
	       mask_B[1] = 1;
	       mask_R[2] = 1;
	       mask_G[3] = 1;
        }
       else if (0 == strcmp("BGGR", buf))
        {
	       mask_B[0] = 1;
	       mask_G[1] = 1;
	       mask_G[2] = 1;
	       mask_R[3] = 1;
        }
        else
        {
            mexErrMsgTxt("The second argument must be \"RGGB\", \"GRBG\", \"GBRG\" or \"BGGR\"\n");
        }

        float* image= new float[nx*ny*3];
        int tmpx, tmpy, tmpz;
        for (tmpz=0;tmpz<3;tmpz++)
            for (tmpx=0; tmpx<nx; tmpx++)
                for (tmpy=0; tmpy<ny; tmpy++)
                    image[tmpx+tmpy*nx+tmpz*nx*ny] = (float)(mxGetPr(prhs[0]))[tmpy+tmpx*ny+tmpz*nx*ny];
        /*
         * process the RGB channels by multiplication the input pixel
         * values by a 2x2 mask
         */
        float *channel;
        channel = image;

        /* red channel */
        for (j = 0; j < ny; j++)
            for (i = 0; i < nx; i++)
                channel[i + nx * j] *= mask_R[(i % 2) + 2 * (j % 2)];
        
        /* green channel */
        channel = image + nx * ny;
        for (j = 0; j < ny; j++)
            for (i = 0; i < nx; i++)
                channel[i + nx * j] *= mask_G[(i % 2) + 2 * (j % 2)];
        
        /* blue channel */
        channel = image + 2 * nx * ny;
        for (j = 0; j < ny; j++)
            for (i = 0; i < nx; i++)
                channel[i + nx * j] *= mask_B[(i % 2) + 2 * (j % 2)];

         /* output */
        mwSize dim = 3;
        const mwSize dims[3]={ny,nx,3};
        plhs[0]=mxCreateNumericArray(dim, dims,mxDOUBLE_CLASS,mxREAL);
        double* pointeur=(double*)mxGetPr(plhs[0]);
        for (tmpz=0;tmpz<3;tmpz++)
            for (tmpy=0; tmpy<ny; tmpy++)
                for (tmpx=0; tmpx<nx; tmpx++)
                {
                    pointeur[tmpy+tmpx*ny+tmpz*nx*ny]=(double)image[tmpx+tmpy*nx+tmpz*nx*ny];
                }

        delete [] image;
        free(buf);
        }
    }
