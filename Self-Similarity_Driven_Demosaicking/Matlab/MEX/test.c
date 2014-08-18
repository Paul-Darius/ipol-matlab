/* Test à effectuer */
/* Afin de comprendre comment matlab récupère les données d'un tableau multidimensionnel vers un vecteur.
Puis comment il remplit un tableau multidimensionnel à partir d'un vecteur */
typedef char* char16_t;
#include <stdio.h>
#include <stdlib.h>
#include "mex.h"


void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int nx=mxGetN(prhs[0]);
    int ny=mxGetM(prhs[0]);
	double * image=mxGetPr(prhs[0]);
	mexPrintf("%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e \n",image[0],image[1],image[2],image[3],image[4],image[5],image[6],image[7],image[8],image[9],image[10],image[11],image[12],image[13],image[14],image[15],image[16],image[17]);
	mwSize dim = 3;
    const mwSize dims[3]={ny,nx/3,3};
    plhs[0]=mxCreateNumericArray(dim, dims,mxDOUBLE_CLASS,mxREAL);
    double* pointeur=(double*)mxGetPr(plhs[0]);
    int i,j;
    for (i=0;i<nx*ny;i++)
    	{
    		pointeur[i]=image[i];
    	}
}