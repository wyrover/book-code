// Math.cpp : Implementation of CMath
#include "stdafx.h"
#include "Chapter10_BothServer.h"
#include "Math.h"

/////////////////////////////////////////////////////////////////////////////
// CMath

STDMETHODIMP CMath::Add(long lOp1, long lOp2, long* plResult)
{
    *plResult = lOp1 + lOp2;
    return S_OK;
}

STDMETHODIMP CMath::Subtract(long lOp1, long lOp2, long* plResult)
{
    *plResult = lOp1 - lOp2;
    return S_OK;
}

STDMETHODIMP CMath::Multiply(long lOp1, long lOp2, long* plResult)
{
    *plResult = lOp1 * lOp2;
    return S_OK;
}

STDMETHODIMP CMath::Divide(long lOp1, long lOp2, long* plResult)
{
    *plResult = lOp1 / lOp2;
    return S_OK;
}
