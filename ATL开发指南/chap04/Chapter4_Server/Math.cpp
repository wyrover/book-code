// Math.cpp : Implementation of CMath
#include "stdafx.h"
#include "Chapter4_Server.h"
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

STDMETHODIMP CMath::Add(long lOp1, long lOp2, long * plResult)
{
    *plResult = lOp1 + lOp2;
    return S_OK;
}

STDMETHODIMP CMath::Subtract(long lOp1, long lOp2, long * plResult)
{
    *plResult = lOp1 - lOp2;
    return S_OK;
}

STDMETHODIMP CMath::Multiply(long lOp1, long lOp2, long * plResult)
{
    *plResult = lOp1 * lOp2;
    return S_OK;
}

STDMETHODIMP CMath::Divide(long lOp1, long lOp2, long * plResult)
{
    // Handle divide-by-zero error
    if (lOp2 == 0) {
        return Error("Divide by zero attemped");
    }

    *plResult = lOp1 / lOp2;
    return S_OK;
}

// New IMath2 methods
STDMETHODIMP CMath::Sum(short sArraySize,
                        short sArray[],
                        long* lResult)
{
    *lResult = 0;

    while (sArraySize) {
        *lResult += sArray[--sArraySize];
    }

    return S_OK;
}

STDMETHODIMP CMath::Compute(mathOPERATION enumOp,
                            long lOp1,
                            long lOp2,
                            long * plResult)
{
    switch (enumOp) {
    case mathAdd:
        return Add(lOp1, lOp2, plResult);

    case mathSubtract:
        return Subtract(lOp1, lOp2, plResult);

    case mathMultiply:
        return Multiply(lOp1, lOp2, plResult);

    case mathDivide:
        return Divide(lOp1, lOp2, plResult);
    }

    return S_OK;
}

STDMETHODIMP CMath::get_AdvancedMath(IAdvancedMath** pVal)
{
    GetUnknown()->QueryInterface(IID_IAdvancedMath, (void**) pVal);
    return S_OK;
}

// IAdvancedMath interface
long calcFactorial(short n)
{
    if (n > 1)
        return n * calcFactorial(n - 1);
    else
        return 1;
}

STDMETHODIMP CMath::Factorial(short sOp, long* pResult)
{
    *pResult = calcFactorial(sOp);
    return S_OK;
}

long calcFibonacci(short n)
{
    if (n <= 1)
        return 1;

    return calcFibonacci(n - 1) + calcFibonacci(n - 2);
}

STDMETHODIMP CMath::Fibonacci(short sOp, long* pResult)
{
    *pResult = calcFibonacci(sOp);
    return S_OK;
}

STDMETHODIMP CMath::get_Info(COMPONENT_INFO** ppInfo)
{
    *ppInfo = (COMPONENT_INFO *) CoTaskMemAlloc(sizeof(COMPONENT_INFO));
    ZeroMemory(*ppInfo, sizeof(COMPONENT_INFO));
    // Make these globals or defines
    (*ppInfo)->sMajor = 1;
    (*ppInfo)->sMinor = 0;
    char szBuffer[128];

    if (LoadString(_Module.GetResourceInstance(),
                   IDS_AUTHOR,
                   szBuffer,
                   sizeof(szBuffer))) {
        (*ppInfo)->pstrAuthor = (unsigned char*) CoTaskMemAlloc(lstrlen(szBuffer) + 1);
        memcpy((*ppInfo)->pstrAuthor, szBuffer, lstrlen(szBuffer) + 1);
    }

    if (LoadString(_Module.GetResourceInstance(),
                   IDS_NAME,
                   szBuffer,
                   sizeof(szBuffer))) {
        USES_CONVERSION;
        (*ppInfo)->bstrName = SysAllocString(A2W(szBuffer));
    }

    return S_OK;
}

STDMETHODIMP CMath::get_Name(BSTR* pbstrName)
{
    char szBuffer[128];

    if (LoadString(_Module.GetResourceInstance(),
                   IDS_NAME,
                   szBuffer,
                   sizeof(szBuffer))) {
        USES_CONVERSION;
        *pbstrName = SysAllocString(A2W(szBuffer));
    }

    return S_OK;
}