#ifdef __APPLE__
typedef char* char16_t;
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mex.h"


/* input/output handling */


void numberio(int nlhs, int i, int nrhs, int j);

// Input : number of output, number of output required, number of input, number of input required
// Output : None.
// Other informations : Prints an error message if the number is not the one required.

void type(char *type, const mxArray *prhs[], int i);

// Input : type required, prhs, element number of the prhs
// Output : None.
// Other Informations : Prints an error message if the type is not the one required.
// PS : this function will evolve : must add the struct type.
// For more possibilities, the function mxIsclass is very useful.

int isCorrectString( char* requiredString, mxArray *prhsi);

// Input : string required, string element of prhs
// Output : 0 if the two strings are differents, 1 if they are the same


/* String functions */



int get_string_length(mxArray* prhsi);

// Input : a string element of prhs
// Output : size of the string

char* get_string(mxArray* prhsi);

// Input : a string element from prhs
// Output : The string as a char* usable by language C

// Typical code :
// char *message = malloc(sizeof(char)*(get_string_length(prhs[X])+1));
// message = get_string(prhs[X]);
// (...) using the variable message (...)
// free(message);

void set_string(char* message, mxArray* prhsi);

// Input : a char* string, element of prhs
// Output : none
// Other informations : the function sets the value of plhs[i] as the


/* Array functions */

float* imageMatlabToC_malloc(mxArray *prhsi, int *nx, int *ny, int *nz);

// Goal : prhsi is column-major image because of matlab's norm. The function creates a row-major image containing the same informations. Thos are used by the majority of C-programs
// input : prhs[i] you get the image from, number of column, number of rows, number of channels
// output :  pointer to the first element of the array.
// Other information : You must free the pointer when you do not need it anymore.

void imageCToMatlab(float* image, int nx, int ny, int number_of_channels, mxArray *plhs[], int i);

// Goal : prhsi is column-major image because of matlab's norm. The function creates a row-major image containing the same informations. Thos are used by the majority of C-programs
// input : an array that must be allocated, number of column, number of rows, number of channels, plhs, element number of plhs you want to take the image from
// output : none
// Other information : the array "image" contains now the required informations.

void imageMatlabToC(mxArray *prhsi, int number_of_channels, float* image);


// Goal : prhsi is column-major image because of matlab's norm. The function creates a row-major image containing the same informations. Thos are used by the majority of C-programs
// input : an array that must be allocated, number of column, number of rows, number of channels, plhs, element number of plhs you want to take the image from
// output : none
// Other information : the array "image" contains now the required informations.


// Now, let's see an example on how to use it. I want to take an image from Matlab -the input argument- and send it back to Matlab -the output argument-

//#include <stdio.h>
//#include <stdlib.h>
//#include "libmexipol.h"
//
//
//void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
//{
//    int nx, ny, nz;
//    float *image = imageMatlabToC_malloc(prhs[0],&nx,&ny,&nz);
//	  //Here, the programmer has to use his algorithm with the pointer "image" : algorithm(image);
//	  //This part is only necessary if the programmer wants to get back the image as an output. For example if he made modifications on it.
//    imageCToMatlab(image, nx, ny, nz, plhs,0);
//    free(image);
//}

/* Structure functions */

struct getStruct {
	char* name;
	double value;
};

typedef struct getStruct getStruct;

getStruct* get_structure(const mxArray* prhsi, &numb_fields);

void set_structure(getStruct* structure, const mxArray* plhs, int numb_fields)


