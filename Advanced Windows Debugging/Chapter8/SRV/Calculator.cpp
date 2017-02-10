/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <stdio.h>
#include <tchar.h>
#include <srv.h>
#include <comdef.h>
#include <ole2.h>

#define _ATL_APARTMENT_THREADED

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

using namespace ATL;

#include "Calculator.h"


// CCalculator

STDMETHODIMP CCalculator::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_ICalculator
    };

    for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}

STDMETHODIMP CCalculator::Sum( 
            __int32 a,
            __int32 b,
            __int32 *result)
{
    *result = a * b;
    return S_OK;
}
    
STDMETHODIMP CCalculator::SumSlow( 
            __int32 a,
            __int32 b,
            __int32 *result)
{
    Sleep(10*60*1000);
    *result = a * b;
    return S_OK;
}
