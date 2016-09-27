// AdvancedMath.cpp : Implementation of CAdvancedMath
#include "stdafx.h"
#include "Chapter5_Aggregate.h"
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
