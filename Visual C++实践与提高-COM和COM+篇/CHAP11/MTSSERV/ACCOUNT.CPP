// Account.cpp : Implementation of CAccount
#include "stdafx.h"
#include "Bank.h"
#include "Account.h"

/////////////////////////////////////////////////////////////////////////////
// CAccount

STDMETHODIMP CAccount::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IAccount
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CAccount::Post(long lAccount, long lAmount, BSTR *pbstrResult)
{
    *pbstrResult =::SysAllocString(L"Hello! This is my first MTS component!");
    return S_OK;
}
