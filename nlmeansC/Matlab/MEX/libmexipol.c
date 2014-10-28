
//
//  libmexipol.c
//  
//
//  Created by Paul-Darius Sarmadi.
//
//
#ifdef __APPLE__
typedef char* char16_t;
#endif
#include "libmexipol.h"
#include "mex.h"
#include <stdio.h>
#include <stdlib.h>

/* input/output handling */

void number_io(int nlhs, int i, int nrhs, int j)
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

void is_type(char *type, const mxArray *prhs[], int i)
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
    else if(strcmp (type,"structure")==0)
    {
        if (!mxIsStruct(prhs[i]))
        {
            char* numero[200];
            sprintf(numero, "%d", i);
            char* errmsg[200];
            strcpy (errmsg, "The type of the data number ");
            strcat (errmsg, numero);
            strcat (errmsg, " must be a structure.");
            mexErrMsgTxt(errmsg);
        }
    }
    else
    {
        mexErrMsgTxt("Ask the programmer of this soft to clear up this issue");
    }
}

int is_correct_string( char* requiredString, mxArray *prhsi)
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

char* get_string_malloc(mxArray* prhsi)
{
    char* buf;
    size_t buflen;
    int status;
    buflen = mxGetN(prhsi)*sizeof(char)+1;
    buf = (char*)malloc(buflen);
    status = mxGetString(prhsi, buf, (mwSize)buflen);
    return buf;
}

void set_string(char* message, mxArray* plhsi)
{
    plhsi=mxCreateString(message);
}

/* Array functions */

void image_matlab_to_c(const mxArray *prhsi, int number_of_channels, float* image)
{
    if (number_of_channels<1)
    {
        mexErrMsgTxt("Ask the programmer of this soft to clear up this issue");
    }
    else
    {
        int nx, ny;
        if (number_of_channels==1)
        {
            nx=mxGetN(prhsi);
            ny=mxGetM(prhsi);
            int tmpx, tmpy;
            for (tmpx=0; tmpx<nx; tmpx++)
                for (tmpy=0; tmpy<ny; tmpy++)
                    image[tmpy+tmpx*(*ny)] = (float)(mxGetPr(prhsi))[tmpy*(*nx)+tmpx];
        }
        else if (number_of_channels>1)
        {
            nx=mxGetN(prhsi)/number_of_channels;
            ny=mxGetM(prhsi);
            int tmpx, tmpy, tmpz;
            for (tmpz=0;tmpz<number_of_channels;tmpz++)
            {
                for (tmpx=0; tmpx<nx; tmpx++)
                {
                    for (tmpy=0; tmpy<ny; tmpy++)
                    {
                        image[tmpy+tmpx*(*ny)+tmpz*(*nx)*(*ny)] = (float)(mxGetPr(prhsi))[tmpy*(*nx)+tmpx+tmpz*(*nx)*(*ny)];
                    }
                }
            }
        }
    }
}

float* image_matlab_to_c_malloc(const mxArray *prhsi, int *nx, int *ny, int *nz)
{
    int num=mxGetNumberOfDimensions(prhsi);
    *nx = mxGetDimensions(prhsi)[0];
    *ny = mxGetDimensions(prhsi)[1];
    *nz = mxGetDimensions(prhsi)[2];
    int tmpx, tmpy, tmpz;
    float* image=malloc((*nx)*(*ny)*(*nz)*sizeof(float));
    for (tmpz=0;tmpz<*nz;tmpz++)
    {
        for (tmpx=0; tmpx<*nx; tmpx++)
        {
            for (tmpy=0; tmpy<*ny; tmpy++)
            {
                image[tmpy+tmpx*(*ny)+tmpz*(*nx)*(*ny)] = (float)(mxGetPr(prhsi))[tmpy*(*nx)+tmpx+tmpz*(*nx)*(*ny)];
            }
        }
    }
    return image;
}


void image_c_to_matlab(float* image, int nx, int ny, int number_of_channels, mxArray *plhs[], int i)
{
    if (number_of_channels<1)
    {
        mexErrMsgTxt("Ask the programmer of this soft to clear up this issue");
    }
    else
    {
        if (number_of_channels==1)
        {
            plhs[i]=mxCreateNumericMatrix(ny, nx,mxDOUBLE_CLASS,mxREAL);
            double* pointeur=(double*)mxGetPr(plhs[i]);
            int tmpx,tmpy;
            for (tmpy=0; tmpy<ny; tmpy++)
                for (tmpx=0; tmpx<nx; tmpx++)
                    pointeur[tmpy+tmpx*ny]=(double)image[tmpx+tmpy*nx];
        }
        else
        {
            mwSize dim = 3;
            const mwSize dims[3]={ny,nx,number_of_channels};
            plhs[i]=mxCreateNumericArray(dim, dims,mxDOUBLE_CLASS,mxREAL);
            double* pointeur=(double*)mxGetPr(plhs[i]);
            int tmpx,tmpy,tmpz;
            for (tmpz=0;tmpz<number_of_channels;tmpz++)
            {
                for (tmpy=0; tmpy<ny; tmpy++)
                {
                    for (tmpx=0; tmpx<nx; tmpx++)
                    {
                        pointeur[tmpy+tmpx*ny+tmpz*nx*ny]=(double)image[tmpx+tmpy*nx+tmpz*nx*ny];
                    }
                }
            }
        }
    }
    
}


/* Struct functions */


value_struct* get_structure_malloc(const mxArray* prhsi, int *numb_fields)
{
    if (!mxIsStruct(prhsi))
    {
        mexErrMsgTxt("Ask the programmer of this soft to clear up this issue");
    }
    *numb_fields = mxGetNumberOfFields(prhsi);
    value_struct* structure = malloc(*numb_fields*sizeof(value_struct));
    int k=0;
    for (k=0;k<*numb_fields;k++)
    {
        structure[k].name=malloc(sizeof(char)*strlen(mxGetFieldNameByNumber(prhsi,k)));
        structure[k].name=mxGetFieldNameByNumber(prhsi,k);
        structure[k].value=(mxGetPr(mxGetField(prhsi,0,mxGetFieldNameByNumber(prhsi,k))))[0];
    }
    return structure;
}

void set_structure(value_struct* structure, const mxArray* plhs[], int i, int numb_fields)
{   
    int k=0;
    char **field_names=malloc(sizeof(char*)*numb_fields);
    for (k=0;k<numb_fields;k++)
    {
        field_names[k]=malloc(sizeof(char)*(strlen(structure[k].name)+1));
        field_names[k]=structure[k].name;
    }
    for (k=0;k<numb_fields;k++)
    {
        mwSize dim = 1;
        const mwSize dims[1]={1};
        mxArray* valeur =mxCreateNumericArray(dim, dims,mxDOUBLE_CLASS,mxREAL);
        double* pointer = (double*)mxGetPr(valeur);
        pointer[0]=(double)structure[k].value;
        mexPrintf("%s=%e",structure[k].name,pointer[0]);
        plhs[i]=mxCreateStructArray(dim, dims, numb_fields, field_names);
        mxSetField(plhs[i],0,structure[k].name,valeur);
    }
    free(field_names);
}
