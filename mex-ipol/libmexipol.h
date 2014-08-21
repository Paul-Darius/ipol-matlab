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


void imageMatlabToC(mxArray* prhsi, int number_of_channels, float* image);

// input : prhs[i] you get the image from,
//
//

void imageCToMatlab(float* image, int number_of_channels, mxArray *plhsi);

//
//
//


/* Structure functions */

void get_structure(const mxArray* prhsi, char* array[]); // if data is type double we have to convert string to double
void send_double_array(double* array, const mxArray* plhsi); // send a double array to an element from the structure
void send_matrix(double* array, int num_col, int num_row, const mxArray* plhsi);
void send_string_array(char* array, const mxArray* plhsi);
