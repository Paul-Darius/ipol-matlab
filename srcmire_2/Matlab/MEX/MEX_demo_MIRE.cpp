/*
 * Copyright (c) 2014, Paul-Darius Sarmadi <paul-darius.sarmadi@telecom-sudparis.eu>
 * Based on "demo_mire.cpp" by Yohann Tendero <yohann.tendero@cmla.ens-cachan.fr>
 */

#include "mex.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../MIRE.h"
#include "../../borders.h"


void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nlhs!=1)
    {
        mexErrMsgTxt("One matrix is needed as an output");
    }
    if(nrhs!=1)
    {
        mexErrMsgTxt("One double matrix is needed as an input");
    }
    if(!mxIsDouble(prhs[0]))
    {
        mexErrMsgTxt("The input must be a double typed matrix");
    }
    
////////////////////////////////////////////////////////////////////////////////
////////////////////////// CONSTANT PARAMETER DEFINITION////////////////////////
////////////////////////////////////////////////////////////////////////////////

//During the optimal sigma parameter guess phase each sigma such that
//sigma=SIGMA_MIN+k*DELTAT and sigma<=SIGMA_MAX will be tested.

    const int SIGMA_MIN=0; // minimal std-dev of the Gaussian-weighting function
    const int SIGMA_MAX=8; //maximal std-dev of the Gaussian-weighting function
    const float DELTA=0.5; //step between two consecutive std-dev




////////////////////////////////////////////////////////////////////////////////
////////////////////////// READ IMAGE///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    int w1=mxGetN(prhs[0]);
    int h1=mxGetM(prhs[0]);
    
    float* Image= new float[w1*h1];
    int tmp=0;
	 int tmpx, tmpy;
    for (tmpx=0; tmpx<w1; tmpx++) 
       for (tmpy=0; tmpy<h1; tmpy++) 
	 	   Image[tmpx+tmpy*w1] = (float)(mxGetPr(prhs[0]))[tmpy+tmpx*h1];


////////////////////////////////////////////////////////////////////////////////
//////////////DEALING WITH BORDER EFFECTS: MIRROR symmetry ON COLUMNS////////////
////////////////////////////////////////////////////////////////////////////////




    int W=w1+8*SIGMA_MAX;
//new  width of image afer mirror symmetry : 4 times SIGMA_MAX on each sides
    float* Imsym= new float[W*h1]; //new image allocation

//input : image, New image, width,height, total size of the extension
    borders(Image,Imsym,w1,h1,4*SIGMA_MAX); //Output :
//Classic mirror symmetry on columns :
//    C1 C2 ... CN => C2 C1 |C1 C2 ... CN|CN CN-1 etc





////////////////////////////////////////////////////////////////////////////////
//////////////////////////TRANSFERING DATAS TO CENTRAL FUNCTION/////////////////
////////////////////////////////////////////////////////////////////////////////

//input image , width (after symetrication), height
    MIRE_automatic(Imsym,W,h1,SIGMA_MIN, SIGMA_MAX, DELTA);




////////////////////////////////////////////////////////////////////////////////
///////////REMOVING THE SYMETRIZATION : CROP (see DEALING WITH BORDERS//////////
////////////////////////////////////////////////////////////////////////////////



//#pragma omp parallel for
// The following realize a CROP
    for (unsigned column=0;column<w1;column++)  // for all columns
    {
        for (unsigned line=0;line<h1;line++)  // for all lines
        {
            Image[line*w1+column]
            =Imsym[line*(w1+8*SIGMA_MAX)+column+4*SIGMA_MAX];
// using the useless memory of the input
        }
    }

    delete [] Imsym;


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////DYNAMIC CORRECTION////////////////////////////////
///////////////////////////////IMPOSING/////////////////////////////////////////
///////////////////////////////[0,255]//////////////////////////////////////////


//Computing the min and max of the output
    float min=Image[0];
    float max=Image[0];
//#pragma omp parallel for
    for (unsigned i=1;i<w1*h1;i++)
    {
        if (Image[i]<min) min=Image[i];
        if (Image[i]>max) max=Image[i];
    }


//Actually changing image-values
//#pragma omp parallel for
    for (unsigned i=1;i<w1*h1;i++)
    {
        Image[i]=(255*(Image[i]-min)/(max-min));
    }


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////WRITING THE OUTPUT////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    
    mwSize dim = 2;
    const mwSize dims[2]={h1,w1};
    plhs[0]=mxCreateNumericArray(dim, dims, mxDOUBLE_CLASS, mxREAL);
    double* pointeur=(double*)mxGetPr(plhs[0]);
    for (tmpy=0; tmpy<h1; tmpy++) 
       for (tmpx=0; tmpx<w1; tmpx++) 
       {
      	   pointeur[tmpy+tmpx*h1]=(double)Image[tmpx+tmpy*w1];
       }

    delete [] Image;

}
