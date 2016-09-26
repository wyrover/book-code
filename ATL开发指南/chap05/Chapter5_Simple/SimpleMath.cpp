//
// SimpleMath.cpp : Implementation of CSimpleMath
//

#include "stdafx.h"
#include "Chapter5_Simple.h"
#include "SimpleMath.h"

//////////////////////
// CSimpleMath
//////////////////////

// IMath interface
STDMETHODIMP CSimpleMath::Add(long lOp1, long lOp2, long* pResult)
{
    *pResult = lOp1 + lOp2;
    return S_OK;
}

STDMETHODIMP CSimpleMath::Subtract(long lOp1, long lOp2, long* pResult)
{
    *pResult = lOp1 - lOp2;
    return S_OK;
}

STDMETHODIMP CSimpleMath::Multiply(long lOp1, long lOp2, long* pResult)
{
    *pResult = lOp1 * lOp2;
    return S_OK;
}

STDMETHODIMP CSimpleMath::Divide(long lOp1, long lOp2, long* pResult)
{
    *pResult = lOp1 / lOp2;
    return S_OK;
}

