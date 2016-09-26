// AdvancedMath.cpp : Implementation of CAdvancedMath
#include "stdafx.h"
#include "Chapter5_Contain.h"
#include "AdvancedMath.h"

/////////////////////////////////////////////////////////////////////////////
// CAdvancedMath


// IAdvancedMath interface
long calcFactorial(short n)
{
    if (n > 1)
        return n * calcFactorial(n - 1);
    else
        return 1;
}

STDMETHODIMP CAdvancedMath::Factorial(short sOp, long* plResult)
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

STDMETHODIMP CAdvancedMath::Fibonacci(short sOp, long* plResult)
{
    *plResult = calcFibonacci(sOp);
    return S_OK;
}

// Implementation needed with containment
// However, we just pass the call on to the contained component
STDMETHODIMP CAdvancedMath::Add(long lOp1, long lOp2, long * plResult)
{
    return m_pSimpleMath->Add(lOp1, lOp2, plResult);
}

STDMETHODIMP CAdvancedMath::Subtract(long lOp1, long lOp2, long * plResult)
{
    return m_pSimpleMath->Subtract(lOp1, lOp2, plResult);
}

STDMETHODIMP CAdvancedMath::Multiply(long lOp1, long lOp2, long * plResult)
{
    return m_pSimpleMath->Multiply(lOp1, lOp2, plResult);
}

STDMETHODIMP CAdvancedMath::Divide(long lOp1, long lOp2, long * plResult)
{
    return m_pSimpleMath->Divide(lOp1, lOp2, plResult);
}
