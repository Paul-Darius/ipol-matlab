#include <stdio.h>
#include <stdlib.h>
#include "mex.h"

/* String functions */
char* get_string(const mxArray* prhsi);
void send_string(char* string, const mxArray* plhsi);

/* Structure functions */
void get_structure(const mxArray* prhsi, char* array); // if data is type double we have to convert string to double
void send_double_array(double* array, const mxArray* plhsi); // send a double array to an element from the structure
void send_matrix(double* array, int num_col, int num_row, const mxArray* plhsi);
void send_string_array(char* array, const mxArray* plhsi);
