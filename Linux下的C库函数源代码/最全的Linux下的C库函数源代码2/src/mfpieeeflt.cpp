/***
*mfpieeeflt.cpp - Managed floatin point exception filter
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#include <fpieee.h>
#include <mtdll.h>
#include <stdio.h>
#include <internal.h>

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention
#pragma warning(disable:4483)   // disable warning/error about __identifier(<string>)

typedef int (__clrcall *filter_m)(_FPIEEE_RECORD *);

namespace __identifier("<CrtImplementationDetails>")
{

class HandlersFP
{
public:
[System::ThreadStaticAttribute] __declspec(appdomain) static filter_m __pFilter_m;
};

__declspec(appdomain) filter_m HandlersFP::__pFilter_m = (filter_m) _encoded_null();
}

using namespace __identifier("<CrtImplementationDetails>");

int __cdecl fpieee_flt_thunk_func(_FPIEEE_RECORD *pfpieee_record)
{
    filter_m pFunc = (filter_m) _decode_pointer(HandlersFP::__pFilter_m);
    if (pFunc)
    {
        return pFunc(pfpieee_record);
    }
    else
    {
        return 0;
    }
}

_MRTIMP int __cdecl _fpieee_flt
(
    unsigned long exceptionCode,
    struct _EXCEPTION_POINTERS *pExceptionPointers,
    int (__clrcall *pFunc)(_FPIEEE_RECORD *)
)
{
    HandlersFP::__pFilter_m = (filter_m) _encode_pointer(pFunc);
    return _fpieee_flt(exceptionCode, pExceptionPointers, fpieee_flt_thunk_func);
}
