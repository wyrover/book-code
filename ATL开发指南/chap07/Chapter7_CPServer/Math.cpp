// Math.cpp : Implementation of CMath
#include "stdafx.h"
#include "Chapter7_CPServer.h"
#include "Math.h"

/////////////////////////////////////////////////////////////////////////////
// CMath


STDMETHODIMP CMath::Add(long lOp1, long lOp2, long *plResult)
{
    *plResult = lOp1 + lOp2;
    Fire_ComputationComplete(*plResult);
    return S_OK;
}

STDMETHODIMP CMath::Subtract(long lOp1, long lOp2, long *plResult)
{
    *plResult = lOp1 - lOp2;
    Fire_ComputationComplete(*plResult);
    return S_OK;
}

STDMETHODIMP CMath::Multiply(long lOp1, long lOp2, long *plResult)
{
    *plResult = lOp1 * lOp2;
    Fire_ComputationComplete(*plResult);
    return S_OK;
}

STDMETHODIMP CMath::Divide(long lOp1, long lOp2, long *plResult)
{
    *plResult = lOp1 / lOp2;
    Fire_ComputationComplete(*plResult);
    return S_OK;
}
