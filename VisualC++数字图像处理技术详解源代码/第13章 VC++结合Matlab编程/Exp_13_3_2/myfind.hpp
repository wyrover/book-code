//
// MATLAB Compiler: 2.0
// Date: Thu Oct 08 21:49:36 2009
// Arguments: "-t" "-L" "Cpp" "myfind" 
//
#ifndef __myfind_hpp
#define __myfind_hpp 1

#include "matlab.hpp"

extern mwArray myfind(mwArray x = mwArray::DIN, mwArray n = mwArray::DIN);
#ifdef __cplusplus
extern "C"
#endif
void mlxMyfind(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);

#endif
