//
//  libmexipol.c
//  
//
//  Created by Paul-Darius Sarmadi.
//
//

#include "libmexipol.h"
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

void type(char *type, const mxArray *prhs[], int i)
{
    if (strcmp (type,"double")==0)
    {
        if (!mxIsDouble(prhs[i]))
        {
            char* numero[200];
            sprintf(numero, "%d", i);
            char* errmsg[200];
            strcpy (errmsg, "The type of the data number ");
            strcat (errmsg, numero);
            strcat (errmsg, " must be double.");
            mexErrMsgTxt(errmsg);

        }
    }
    else if (strcmp (type,"string")==0)
    {
        if (!mxIsClass(prhs[i],"mxCHAR_CLASS"))
        {
            char* numero[200];
            sprintf(numero, "%d", i);
            char* errmsg[200];
            strcpy (errmsg, "The type of the data number ");
            strcat (errmsg, numero);
            strcat (errmsg, " must be string.");
            mexErrMsgTxt(errmsg);

        }
    }
    else
    {
        mexErrMsgTxt("Ask the programmer of this soft to clear up this issue");
    }
}

int isCorrectString( char* requiredString, mxArray *prhsi)
{
    char* buf;
    size_t buflen;
    int status;
    buflen = mxGetN(prhsi)*sizeof(char)+1;
    buf = (char*)malloc(buflen);
    status = mxGetString(prhsi, buf, (mwSize)buflen);
    if (strcmp(buf,requiredString)!=0)
    {
        return 0;
    }
    else return 1;
}


/* String functions */


int get_string_length(mxArray* prhsi)
{
    return mxGetN(prhsi);
}

char* get_string(mxArray* prhsi)
{
    char* buf;
    size_t buflen;
    int status;
    buflen = mxGetN(prhsi)*sizeof(char)+1;
    buf = (char*)malloc(buflen);
    status = mxGetString(prhsi, buf, (mwSize)buflen);
    return buf;
}

void set_string(char* message, mxArray* prhsi)
{
    prhsi=mxCreateString(message);
}
/* */