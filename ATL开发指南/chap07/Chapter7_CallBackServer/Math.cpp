// Math.cpp : Implementation of CMath
#include "stdafx.h"
#include "Chapter7_CallBackServer.h"
#include "Math.h"

/////////////////////////////////////////////////////////////////////////////
// CMath

STDMETHODIMP CMath::Add(long lOp1, long lOp2)
{
    long lResult = lOp1 + lOp2;

    if (m_pCallBack)
        m_pCallBack->ComputationComplete(lResult);

    return S_OK;
}

STDMETHODIMP CMath::Subtract(long lOp1, long lOp2)
{
    long lResult = lOp1 - lOp2;

    if (m_pCallBack)
        m_pCallBack->ComputationComplete(lResult);

    return S_OK;
}

STDMETHODIMP CMath::Multiply(long lOp1, long lOp2)
{
    long lResult = lOp1 * lOp2;

    if (m_pCallBack)
        m_pCallBack->ComputationComplete(lResult);

    return S_OK;
}

STDMETHODIMP CMath::Divide(long lOp1, long lOp2)
{
    long lResult = lOp1 / lOp2;

    if (m_pCallBack)
        m_pCallBack->ComputationComplete(lResult);

    return S_OK;
}

STDMETHODIMP CMath::Advise(ICallBack * pCallBack)
{
    m_pCallBack = pCallBack;
    return S_OK;
}

STDMETHODIMP CMath::UnAdvise()
{
    m_pCallBack = 0;
    return S_OK;
}
