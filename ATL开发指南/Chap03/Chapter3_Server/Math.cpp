//
// Math.cpp : Implementation of CMath
//

#include "stdafx.h"
#include "Chapter3_Server.h"
#include "Math.h"

/////////////////////////////////////////////////////////////////////////////
// CMath


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
    *plResult = lOp1 / lOp2;
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
