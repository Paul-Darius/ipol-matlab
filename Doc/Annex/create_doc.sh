#!/bin/bash
# create_doc.sh

mkdir Matlab
cd Matlab
touch compileMex.m
touch $1.m
touch README.md
echo "## Matlab wrapper for $1

### $1 Program :

If you want to use the $1 program :
- Go to the current directory with Matlab
- Within Matlab, type :

>compileMex

You can now use the $1 function within matlab

Type :
> help $1

in Matlab to see how it works.
" > README.md
mkdir MEX
cd MEX
touch MEX_$1.c
touch libmexipol.h
echo "/*! \mainpage Mexipol library Documentation
*
* \section intro_sec Presentation
*
* This is the Mexipol library documentation, a simple tool to make interfaces between C and Matlab easily.
*
* \section what_sec What is this library ?
* The library presented here has been developped in order to make ipol's C/C++ algorithms easierly usable in Matlab than with classic mex functions.<BR>
* However, all the mexipol's functions are based on mex functions. Libmexipol simply provides adapted mex-using functions to specific ipol's issues.<BR>
* Basically those issues are : Error messages, string handling, images handling and structure handling.<BR>
* Thus, this document's goal is to show any mexipol's function prototype and goal, give basic examples of use, and advise the user on what to do and what not to do with it.<BR>
*
* \section how_to_sec2 How to use this document ?
* This document is a dictionnary to understand the specific functions used during the C to Matlab ipol-algorithms conversion process which is presented <a href="yahoo.com">here</a>.<BR>
* It is not supposed to be read alone, nor supposed to be totally ignored.<BR>
*
* \section where_sec Where can I find the Mexipol library ?
* The library is available <a href="https://github.com/Paul-Darius/ipol-matlab/tree/master/mex-ipol">here</a>, on GitHub.
*
* \section how_to_sec How to use the library ?
*	To use the library, just copy the libmexipol.h and libmexipol.c files in your project's directory and add them like any other .c/.h file to your project.
*
* \section how_to_sec3 Can I find Ipol's algorithms already available for Matlab ?
* Of course, they are all available <a href="https://github.com/Paul-Darius/ipol-matlab">here</a>.<BR>
*
* \section copyright Copyright and License
*  Copyright (c) 2014 Paul-Darius Sarmadi <sarmadipauldarius@yahoo.fr>
*
* <BR><BR>
*
*/

/*!
* \file      libmexipol.h
* \author    Paul-Darius Sarmadi
* \version   1.0
* \date      August 21, 2014
* \brief     Mexipol's functions
* \details   This .h document contains all the mexipol's functions prototypes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mex.h"


/* input/output handling */


/*! \fn void number_io(int nlhs, int i, int nrhs, int j)
\brief Error message if the nlhs or prhs is not correct.
\details
<strong>Goal :</strong> Prints an error message if the number of outputs or inputs is not the required one.<BR>
<strong>Output :</strong> None.<BR>
\param nlhs The number of outputs.
\param i The number of outputs required by the algorithm.
\param nrhs The number of inputs.
\param j The number of inputs required by the algorithm.
*/
void number_io(int nlhs, int i, int nrhs, int j);

/*! \fn void is_type(char *type, const mxArray *prhs[], int i)
\brief Error message if the type is not the required one.
\details
<strong>Goal :</strong> Prints an error message if the type is not the required one.<BR>
<strong>Output :</strong> None.<BR>
\param type type required as a string : must be "string", "double" or "structure"
\param prhs The function's prhs
\param nrhs The element number of prhs which is the number as an input.
\param j The number of input required by the algorithm.
*/

void is_type(char *type, const mxArray *prhs[], int i);

/*! \fn int is_correct_string( char* requiredString, mxArray *prhsi)
\brief Strcmp for a string from Matlab and a char array.
\details
<strong>Goal :</strong> The goal is to compare a specified string to an element of prhs which must also be a string.<BR>
<strong>Output :</strong> 0 if the strings are differents. 1 if they are the same.<BR>
\param requiredString The required string
\param prhsi The string which is an element of prhs
*/
int is_correct_string( char* requiredString, mxArray *prhsi);


/* String functions */


/*! \fn int get_string_length(mxArray* prhsi)
\brief Get the size of a Matlab's string.
\details
<strong>Output :</strong> Size of the string.<BR><BR>
<strong>Typical code :</strong><BR>
@code
char *message = malloc(sizeof(char)*(get_string_length(prhs[X])+1));
message = get_string(prhs[X]);
(...) using the variable message (...)
free(message);
@endcode
\param prhsi a string element from prhs
*/

int get_string_length(mxArray* prhsi);

/*! \fn char* get_string_malloc(mxArray* prhsi)
\brief String from Matlab to char*.
\details
<strong>Output :</strong> The string as a char*.<BR>
\param prhsi a string element from prhs
*/

char* get_string_malloc(mxArray* prhsi);

/*! \fn set_string(char* message, mxArray* plhsi)
\brief char* to an output string.
\details
<strong>Output :</strong> none<BR>
<strong>Other informations :</strong> The function sets the value of message as the one of plhs[i].<BR>
<strong> NOTE :</strong> The function allocates space for the char. Thus, the user must free it when he does not need it anymore.<BR>
<strong>Example of use :</strong><BR>
char* message = set_string(plhs[1]);<BR>
(...) Using message (...)<BR>
free(message);<BR>
\param message The message you want as an output
\param plhsi The element of plhs you want as the previous string.
*/

void set_string(char* message, mxArray* plhsi);


/* Array functions */


/*! \fn float* image_matlab_to_c_malloc(mxArray *prhsi, int *nx, int *ny, int *nz)
\brief Make a matrix -which is a Matlab input- as a usable input for any C-made algorithm. Mostly used for images.
\details
<strong>Goal :</strong> prhsi is column-major image because of matlab's norm. The function creates a row-major image containing the same informations. Those are used by a majority of C-programs.<BR>
<strong>Output :</strong> Pointer to the first element of the array.<BR>
<strong>NOTE :</strong> The matrix can not be made of more than 3 dimensions.
\param prhsi The element of prhs the user gets the image from.
\param nx Pointer to the number of columns.
\param ny Pointer to the number of rows.
\param nz Pointer to the number of channels.
*/

float* image_matlab_to_c_malloc(mxArray *prhsi, int *nx, int *ny, int *nz);

/*! \fn void image_c_to_matlab(float* image, int nx, int ny, int number_of_channels, mxArray *plhs[], int i)
\brief Make a Matlab-usable matrix from a C-made matrix.
\details
<strong>Goal :</strong> The function creates a row-major float array as a usable output within Matlab. An example is available here : ::imageMatlabToC_malloc.<BR>
<strong>Output :</strong> None.<BR>
<strong>NOTE :</strong> As its name underlines it, the function allocates space for the float array. You must free it when you do not need the array anymore.<BR>
\param image The float-type image which must be sent to Matlab
\param nx Pointer to the number of columns.
\param ny Pointer to the number of rows.
\param number_of_channels Number of channels of the image.
\param plhs The plhs.
\param i The element number of the image as an output.
*/


void image_c_to_matlab(float* image, int nx, int ny, int number_of_channels, mxArray *plhs[], int i);

/*! \fn void image_matlab_to_c(mxArray *prhsi, int number_of_channels, float* image)
\brief Make a matrix -which is a Matlab input- as a usable input for any C-made algorithm. Mostly used for images. An alternative version of ::image_matlab_to_c_malloc where the user has to do the memory allocation himself.<BR>
\details
<strong>Goal :</strong> prhsi is column-major image because of matlab's norm. The function creates a row-major image containing the same informations. Those are used by a majority of C-programs.<BR>
<strong>Output :</strong> None.<BR>
<strong>Other informations :</strong> It is a modified version of image_matlab_to_c_malloc which is a void type function. The result is returned within the image variable instead of being returned by the function itself.<BR>
<strong>Example :</strong><BR>

I want to take an image from Matlab -the input argument- and send it back to Matlab -the output argument-.<BR>
@code
#include <stdio.h>
#include <stdlib.h>
#include "libmexipol.h"
#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
int nx, ny, nz;
float *image = (float*)malloc(sizeof(float)*nx*ny*nz);
image_matlab_to_c(image, nx, ny, nz, prhs, 0);
//Here, the programmer has to use his algorithm with the pointer "image" : algorithm(image);
//This part is only necessary if the programmer wants to get back the image as an output. For example if he made modifications on it.
image_c_to_matlab(image, nx, ny, nz, plhs,0);
free(image);
}
@endcode
\param prhsi The element of prhs the user gets the image from.
\param nomber_of_channels Number of channels of the image.
\param image Pointer to the image which is given by Matlab.
*/
void image_matlab_to_c(mxArray *prhsi, int number_of_channels, float* image);

/* Structure functions */

/*!
\struct value_struct
An array of value_struct can contain a full matlab's double structure. For example : [{one,1},{two,2}].
For more informations, take a look at ::get_structure_malloc.
*/
struct value_struct {
char* name;/*!< Name of the field. */
double value;/*!< Double value within the field. */
};

typedef struct value_struct value_struct;

/*! \fn value_struct* get_structure_malloc(const mxArray* prhsi, int *numb_fields)
\brief A necessary array vision of the structure : a Matlab structure to value_struct array convertor
\details
<strong>Goal :</strong> How to deal with the various and numerous parameters the user may need in order to use his algorithm properly ?<BR>
In a shell it is very easy to use optional parameters. However, within Matlab it is largely harder to manage it.<BR>
A solution is to use matlab's structures. It is very easy to create a structure within Matlab. A name is easily linked to a value.<BR><BR>
For example in Matlab :<BR>
@code
>option.one = 1;
>option.two = 2;
>option
-> option=
-> one: 1
-> two: 2
>output=algorithm(input,option)
@endcode
All the fields do not have to be explicited, and any of the optional values can be declared in any order. This is a good way to manage optional parameters.<BR>
The value_struct structure contains a fieldname and a value.<BR>
An array of value_struct can contain a full matlab's structure made of double values. For example : [{one,1},{two,2}].<BR>
Thus, the get_structure_malloc function has been designed to take a structure from matlab as an input, and give a value_struct array as an output. The mex-programmer can then easily deal with all the options the user choose, and with those the user does not choose.<BR>
As an example, the option input of the previous Matlab example would become : [{one,1},{two,2}].<BR>
The programmer can now easily use matlab's structures as he wish -mainly to manage optional parametres.<BR>
<strong>NOTE :</strong> As its names underlines it, the function allocates space for the value_struct array. You must free it when you do not need it anymore.<BR>
<strong>NOTE 2 :</strong> The elements of the structure must be double. The programmer has to test with the function ::type if it is really the case.<BR>

\param prhsi The element of prhs the user gets the image from.
\param nomber_of_channels Number of channels of the image.
\param image Pointer to the image which is given by Matlab.
*/

value_struct* get_structure_malloc(const mxArray* prhsi, int *numb_fields);

/*! \fn void set_structure(value_struct* structure, const mxArray* plhs[], int i, int numb_fields)
\brief value_struct* array to an output structure.
\details
<strong>Goal :</strong> Transform a value_struct array into an output structure to Matlab.<BR>
Here is an example of a working code. The function takes a struct as an input and gives the same struct as an output.<BR>
@code
int n;
value_struct* structure=get_structure_malloc(prhs[0], &n);
// The value_struct array structure has to be modified here //
set_structure(structure, plhs,0, n);
free(structure);
@endcode
\param structure The value_struct array.
\param plhs The plhs.
\param i The output number.
\param numb_fields The size of the array, i.e. the size of the structure.
*/

void set_structure(value_struct* structure, const mxArray* plhs[], int i, int numb_fields);" > libmexipol.h
touch libmexipol.c
echo "
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

void image_matlab_to_c(mxArray *prhsi, int number_of_channels, float* image)
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
image[tmpx+tmpy*nx] = (float)(mxGetPr(prhsi))[tmpy+tmpx*ny];
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
image[tmpx+tmpy*nx+tmpz*nx*ny] = (float)(mxGetPr(prhsi))[tmpy+tmpx*ny+tmpz*nx*ny];
}
}
}
}
}
}

float* image_matlab_to_c_malloc(mxArray *prhsi, int *nx, int *ny, int *nz)
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
image[tmpx+tmpy*(*nx)+tmpz*(*nx)*(*ny)] = (float)(mxGetPr(prhsi))[tmpy+tmpx*(*ny)+tmpz*(*nx)*(*ny)];
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
}" > libmexipol.c
cd ../..
rm create_doc.sh