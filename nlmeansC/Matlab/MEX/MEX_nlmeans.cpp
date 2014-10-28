#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "libmexipol.h"
}
#include "../../libdenoising.h"

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    number_io(nlhs,2,nrhs,2) ;
    is_type("double",prhs,0) ;
    is_type("double",prhs,1) ;
    
    // read input
    int nx,ny,nc;
    float *d_v = image_matlab_to_c_malloc(prhs[0], &nx, &ny, &nc);
    
    // variables
    int d_w = (int) ny;
    int d_h = (int) nx;
    int d_c = (int) nc;
    if (d_c == 2) {
        d_c = 1;    // we do not use the alpha channel
    }
    if (d_c > 3) {
        d_c = 3;    // we do not use the alpha channel
    }
    
    int d_wh = d_w * d_h;
    int d_whc = d_c * d_w * d_h;
    
    // test if image is really a color image even if it has more than one channel
    if (d_c > 1) {
        
        // dc equals 3
        int i=0;
        while (i < d_wh && d_v[i] == d_v[d_wh + i] && d_v[i] == d_v[2 * d_wh + i ])  {
            i++;
        }
        if (i == d_wh) d_c = 1;
    }
    
    // add noise
    double* pointer = mxGetPr(prhs[1]);
    float fSigma = (float)pointer[0];
    float *noisy = new float[d_whc];
    for (int i=0; i < d_c; i++) {
        fiAddNoise(&d_v[i * d_wh], &noisy[i * d_wh], fSigma, i, d_wh);
    }
    
    // denoise
    float **fpI = new float*[d_c];
    float **fpO = new float*[d_c];
    float *denoised = new float[d_whc];
    for (int ii=0; ii < d_c; ii++) {
        fpI[ii] = &noisy[ii * d_wh];
        fpO[ii] = &denoised[ii * d_wh];
    }
    int bloc, win;
    float fFiltPar;
    if (d_c == 1) {
        
        if (fSigma > 0.0f && fSigma <= 15.0f) {
            win = 1;
            bloc = 10;
            fFiltPar = 0.4f;
            
        } else if ( fSigma > 15.0f && fSigma <= 30.0f) {
            win = 2;
            bloc = 10;
            fFiltPar = 0.4f;
            
        } else if ( fSigma > 30.0f && fSigma <= 45.0f) {
            win = 3;
            bloc = 17;
            fFiltPar = 0.35f;
            
        } else if ( fSigma > 45.0f && fSigma <= 75.0f) {
            win = 4;
            bloc = 17;
            fFiltPar = 0.35f;
            
        } else if (fSigma <= 100.0f) {
            
            win = 5;
            bloc = 17;
            fFiltPar = 0.30f;
            
        } else {
            mexPrintf("error : algorithm parametrized only for values of sigma less than 100.0\n");
            exit(-1);
        }
    } else {
        if (fSigma > 0.0f && fSigma <= 25.0f) {
            win = 1;
            bloc = 10;
            fFiltPar = 0.55f;
            
        } else if (fSigma > 25.0f && fSigma <= 55.0f) {
            win = 2;
            bloc = 17;
            fFiltPar = 0.4f;
            
        } else if (fSigma <= 100.0f) {
            win = 3;
            bloc = 17;
            fFiltPar = 0.35f;
            
        } else {
            printf("error : algorithm parametrized only for values of sigma less than 100.0\n");
            exit(-1);
        }
    }
    nlmeans_ipol(win, bloc, fSigma, fFiltPar, fpI,  fpO, d_c, d_w, d_h);
    
    // save noisy and denoised images
    image_c_to_matlab(noisy, d_w, d_h, d_c, plhs, 0) ;
    image_c_to_matlab(denoised, d_w, d_h, d_c, plhs, 1) ;
    free(d_v);
}
