//
// MATLAB Compiler: 2.0
// Date: Thu Oct 08 21:49:36 2009
// Arguments: "-t" "-L" "Cpp" "myfind" 
//
#include "StdAfx.h"
#include "myfind.hpp"

//
// The function "Mmyfind" is the implementation version of the "myfind"
// M-function from file "E:\Program Files\MATLAB\MATLAB\work\myfind.m" (lines
// 1-2). It contains the actual compiled code for that M-function. It is a
// static function and must only be called from one of the interface functions,
// appearing below.
//
//
// function y = myfind(x,n)
//
static mwArray Mmyfind(int nargout_, mwArray x, mwArray n) {
    mwArray y(mclGetUninitializedArray());
    mwValidateInputs("myfind", 2, &x, &n);
    //
    // y = x(find(x>n));
    //
    y = x(find(x > n));
    mwValidateOutputs("myfind", 1, nargout_, &y);
    return y;
}

//
// The function "myfind" contains the normal interface for the "myfind"
// M-function from file "E:\Program Files\MATLAB\MATLAB\work\myfind.m" (lines
// 1-2). This function processes any input arguments and passes them to the
// implementation version of the function, appearing above.
//
mwArray myfind(mwArray x, mwArray n) {
    int nargout(1);
    mwArray y(mclGetUninitializedArray());
    y = Mmyfind(nargout, x, n);
    return y;
}

//
// The function "mlxMyfind" contains the feval interface for the "myfind"
// M-function from file "E:\Program Files\MATLAB\MATLAB\work\myfind.m" (lines
// 1-2). The feval function calls the implementation version of myfind through
// this function. This function processes any input arguments and passes them
// to the implementation version of the function, appearing above.
//
void mlxMyfind(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]) {
    MW_BEGIN_MLX();
    {
        mwArray mprhs[2];
        mwArray mplhs[1];
        int i;
        if (nlhs > 1) {
            error(
              "Run-time Error: File: myfind Line: 1 Column: "
              "0 The function \"myfind\" was called with mor"
              "e than the declared number of outputs (1)");
        }
        if (nrhs > 2) {
            error(
              "Run-time Error: File: myfind Line: 1 Column:"
              " 0 The function \"myfind\" was called with m"
              "ore than the declared number of inputs (2)");
        }
        for (i = 0; i < 2 && i < nrhs; ++i) {
            mprhs[i] = mwArray(prhs[i], 0);
        }
        for (; i < 2; ++i) {
            mprhs[i].MakeDIN();
        }
        mplhs[0] = Mmyfind(nlhs, mprhs[0], mprhs[1]);
        plhs[0] = mplhs[0].FreezeData();
    }
    MW_END_MLX();
}
