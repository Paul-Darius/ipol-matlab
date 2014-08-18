/*
* Copyright (c) 2009-2011, A. Buades <toni.buades@uib.es>
* All rights reserved.
*  
*
* Patent warning:
*
* This file implements algorithms possibly linked to the patents
* 
* # J. Hamilton Jr and J. Adams Jr, “Adaptive color plan interpolation
* in single sensor color electronic camera,” 1997, US Patent 5,629,734.
*
* # D. Cok, “Signal processing method and apparatus for producing
* interpolated chrominance values in a sampled color image signal”,
* 1987, US Patent 4,642,678.
* 
* # A. Buades, T. Coll and J.M. Morel, Image data processing method by
* reducing image noise, and camera integrating means for implementing
* said method, EP Patent 1,749,278 (Feb. 7, 2007). 
* 
* This file is made available for the exclusive aim of serving as
* scientific tool to verify the  soundness and completeness of the
* algorithm description. Compilation, execution and redistribution
* of this file may violate patents rights in certain countries.
* The situation being different for every country and changing
* over time, it is your responsibility to determine which patent
* rights restrictions apply to you before you compile, use,
* modify, or redistribute this file. A patent lawyer is qualified
* to make this determination.
* If and only if they don't conflict with any patent terms, you
* can benefit from the following license terms attached to this
* file.
* 
* License:
*
* This program is provided for scientific and educational only:
* you can use and/or modify it for these purposes, but you are
* not allowed to redistribute this work or derivative works in
* source or executable form. A license must be obtained from the
* patent right holders for any other use.
*
*
*/

/**
 * @mainpage Self Similarity Driven Demosaicking
 *
 * README.txt:
 * @verbinclude README.txt
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "libdemosaicking.h"

#include "mex.h"

/**
 * @file   demosaickingIpol.cpp
 * @brief  Main executable file
 *
 * 
 *
 * @author Antoni Buades <toni.buades@uib.es>
 */



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
    float *data_in, *data_out;
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

    /* TIFF RGBA 8bit->float input */
    data_in = (float *) mxGetPr(prhs[0]);
    data_out = (float *) malloc(sizeof(float) * nx * ny * 4);

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
    /* copy alpha channel */
    memcpy(data_out + 3 * nx * ny, data_in + 3 * nx * ny,
		   nx * ny * sizeof(float));

	mwSize dim = 3;
    const mwSize dims[3]={ny,nx,3};
    plhs[0]=mxCreateNumericArray(dim, dims, mxDOUBLE_CLASS, mxREAL);
    double* pointeur=(double*)mxGetPr(plhs[0]);
    int k=0;
    for (k=0;k<3 * nx * ny;k++)
    {
        pointeur[k]=(double)data_out[k];
        printf("%f %lf\n", data_out[k], pointeur[k]);
    }

    free(pattern_str);
    }  
}
