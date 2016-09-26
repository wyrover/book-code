// Math.cpp : Implementation of CMath
#include "stdafx.h"
#include "Chapter6_Server.h"
#include "Math.h"

/////////////////////////////////////////////////////////////////////////////
// CMath

STDMETHODIMP CMath::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IMath,
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CMath::Add(VARIANT varOp1, VARIANT varOp2, VARIANT *pvarResult)
{
    HRESULT hr;

    // If we have two strings, append them
    if (varOp1.vt == VT_BSTR &&
        varOp2.vt == VT_BSTR) {
        VariantInit(pvarResult);
        CComBSTR bstr(varOp1.bstrVal);
        bstr.AppendBSTR(varOp2.bstrVal);
        pvarResult->vt = VT_BSTR;
        pvarResult->bstrVal = bstr.Copy();
        return S_OK;
    }

    // Coerce the variant into the desired type
    // In this case we would like a long
    hr = VariantChangeType(&varOp1,
                           &varOp1,
                           0,
                           VT_I4);

    // If we can't get a long return invalidate argument
    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Coerce the variant into the desired type
    // In this case we would like a long
    hr = VariantChangeType(&varOp2,
                           &varOp2,
                           0,
                           VT_I4);

    // If we can't get a long return invalidate argument
    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Initialize the return value
    // If there isn't one, then just return
    if (pvarResult) {
        VariantInit(pvarResult);
        pvarResult->vt = VT_I4;
        pvarResult->lVal = varOp1.lVal + varOp2.lVal;
    }

    return S_OK;
}

STDMETHODIMP CMath::Subtract(VARIANT varOp1, VARIANT varOp2, VARIANT * pvarResult)
{
    CComVariant varOne(varOp1);
    CComVariant varTwo(varOp2);
    // Coerce the variant into the desired type
    HRESULT hr = varOne.ChangeType(VT_I4);

    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Coerce the variant into the desired type
    // In this case we would like a long
    hr = varTwo.ChangeType(VT_I4);

    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Initialize the return value
    // If there isn't one, then just return
    if (pvarResult) {
        VariantInit(pvarResult);
        pvarResult->vt = VT_I4;
        pvarResult->lVal = varOne.lVal - varTwo.lVal;
    }

    return S_OK;
}

STDMETHODIMP CMath::Multiply(VARIANT varOp1, VARIANT varOp2, VARIANT * pvarResult)
{
    CComVariant varOne(varOp1);
    CComVariant varTwo(varOp2);
    // Coerce the variant into the desired type
    HRESULT hr = varOne.ChangeType(VT_I4);

    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Coerce the variant into the desired type
    // In this case we would like a long
    hr = varTwo.ChangeType(VT_I4);

    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Initialize the return value
    // If there isn't one, then just return
    if (pvarResult) {
        VariantInit(pvarResult);
        pvarResult->vt = VT_I4;
        pvarResult->lVal = varOne.lVal * varTwo.lVal;
    }

    return S_OK;
}

STDMETHODIMP CMath::Divide(VARIANT varOp1, VARIANT varOp2, VARIANT * pvarResult)
{
    CComVariant varOne(varOp1);
    CComVariant varTwo(varOp2);
    // Coerce the variant into the desired type
    HRESULT hr = varOne.ChangeType(VT_I4);

    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Coerce the variant into the desired type
    // In this case we would like a long
    hr = varTwo.ChangeType(VT_I4);

    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Initialize the return value
    // If there isn't one, then just return
    if (pvarResult) {
        VariantInit(pvarResult);
        pvarResult->vt = VT_I4;
        pvarResult->lVal = varOne.lVal / varTwo.lVal;
    }

    return S_OK;
}

// IAdvancedMath
long calcFactorial(short n)
{
    if (n > 1)
        return n * calcFactorial(n - 1);
    else
        return 1;
}

STDMETHODIMP CMath::Factorial(short sOp, long* plResult)
{
    *plResult = calcFactorial(sOp);
    return S_OK;
}

long calcFibonacci(short n)
{
    if (n <= 1)
        return 1;

    return calcFibonacci(n - 1) + calcFibonacci(n - 2);
}

STDMETHODIMP CMath::Fibonacci(short sOp, long* plResult)
{
    *plResult = calcFibonacci(sOp);
    return S_OK;
}

//
// IMath2
//
STDMETHODIMP CMath::Sum(VARIANT varOp1, long * plResult)
{
    if (!(varOp1.vt & VT_I4))
        return DISP_E_TYPEMISMATCH;

    if (!(varOp1.vt & VT_ARRAY))
        return DISP_E_TYPEMISMATCH;

    SAFEARRAY* psa;

    if (varOp1.vt & VT_BYREF)
        psa = *(varOp1.pparray);
    else
        psa = varOp1.parray;

    // Sum the elements of the array
    long lLBound, lUBound;
    SafeArrayGetLBound(psa,
                       1, &lLBound);
    SafeArrayGetUBound(psa,
                       1, &lUBound);
    long lSum = 0;

    for (long i = lLBound; i <= lUBound; i++) {
        long lValue;
        SafeArrayGetElement(psa,
                            &i, &lValue);
        lSum += lValue;
    }

    *plResult = lSum;
    return S_OK;
}

#if 0
STDMETHODIMP CMath::Sum(SAFEARRAY* parray, VARIANT* pvarResult)
{
    // Sum the elements of the array
    long lLBound, lUBound;
    SafeArrayGetLBound(parray,
                       1, &lLBound);
    SafeArrayGetUBound(parray,
                       1, &lUBound);
    long lSum = 0;

    for (long i = lLBound; i <= lUBound; i++) {
        long lValue;
        SafeArrayGetElement(parray,
                            &i, &lValue);
        lSum += lValue;
    }

    if (pvarResult) {
        VariantInit(pvarResult);
        pvarResult->vt = VT_I4;
        pvarResult->lVal = lSum;
    }

    return S_OK;
}
#endif

