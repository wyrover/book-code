// CoCalc.cpp : Implementation of CCalc

#include "stdafx.h"
#include "CoCalc.h"


// CCalc

STDMETHODIMP CCalc::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_ICalc
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CCalc::Add(DOUBLE dOp1, DOUBLE dOp2, DOUBLE* pdResult)
{
    if (!pdResult)
        return E_POINTER;

    *pdResult = dOp1 + dOp2;
    return S_OK;
}
