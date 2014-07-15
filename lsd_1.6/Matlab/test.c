#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(!mxIsStruct(prhs[0]))
	{
		mexErrMsgTxt("Ceci n'est pas une structure");
	}
	mexPrintf("%s:%f",mxGetFieldNameByNumber(prhs[0],0),mxGetFieldByNumber(prhs[0],0,0));
		mexPrintf("%s",mxGetFieldNameByNumber(prhs[0],1));
	/*pointeur[0]=mxGetField(prhs[0],0,mxGetFieldNameByNumber(prhs[0],0));*/
	double* ptr=mxGetPr(mxGetField(prhs[0],0,mxGetFieldNameByNumber(prhs[0],0)));
	mexPrintf("\n%e,%f,%d;%e",ptr[0],ptr[1],ptr[2],ptr[5]);
	/*else mexPrintf("L'élément n'est pas de type double.");*/
	mexPrintf("\n%d",mxGetNumberOfElements((mxGetField(prhs[0],0,mxGetFieldNameByNumber(prhs[0],0)))));
}