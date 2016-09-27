// Account.cpp : Implementation of CAccount
#include "stdafx.h"
#include "ATLSampleExe.h"
#include "Account.h"

/////////////////////////////////////////////////////////////////////////////
// CAccount


STDMETHODIMP CAccount::get_Balance(long *pVal)
{
    *pVal = m_balance;
    return S_OK;
}

STDMETHODIMP CAccount::put_Balance(long newVal)
{
    m_balance = newVal;
    return S_OK;
}

STDMETHODIMP CAccount::Post(long lAmount, BSTR *pbstrResult)
{
    TCHAR szBuffer [512];

    if ((m_balance + lAmount) < 0) { //overdraw
        wsprintf(szBuffer, _T("Error. Balance(%ld) can't be overdrawn by %ld."), m_balance, -lAmount);
        *pbstrResult = TCHAR2BSTR(szBuffer);
    } else {
        m_balance += lAmount;
        wsprintf(szBuffer, _T("you'v just %s %ld, balance is %ld now!"),
                 ((lAmount >= 0) ? _T("Credit") : _T("Debit")), lAmount, m_balance);
        *pbstrResult = TCHAR2BSTR(szBuffer);
    }

    return S_OK;
}
