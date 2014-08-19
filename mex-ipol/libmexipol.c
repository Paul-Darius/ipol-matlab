//
//  libmexipol.c
//  
//
//  Created by Paul-Darius Sarmadi on 19/08/2014.
//
//
typedef char* char16_t;
#include "mex.h"
#include <stdio.h>
#include <stdlib.h>

/* input/output handling */

void numberio(int nlhs, int i, int nrhs, int j)
{
    if (nlhs != i)
    {
        mexErrMsgTxt("The number of output arguments is not correct");
    }
    if (nrhs != j)
    {
        mexErrMsgTxt("The number of input arguments is not correct");
    }
}