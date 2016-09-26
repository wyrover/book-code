// Account.cpp : Implementation of CAccount
#include "stdafx.h"
#include "ADOSamp.h"
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
    HRESULT hr = S_OK;
    _RecordsetPtr pRs;
    _ConnectionPtr pConn;
    FieldsPtr pFields;
    FieldPtr pField;
    *pbstrResult = NULL;
    long lErrFlag = 0;
    TCHAR* pErrMsg = NULL;

    try {
        THROW_ERR(pConn.CreateInstance(__uuidof(Connection)), "can't create conn");
        THROW_ERR(pConn->Open("FILEDSN=bank", "", "", -1), "can't open database");
        TCHAR szBuffer [512];
        wsprintf(szBuffer, _T("SELECT Balance FROM Account WHERE AccountNo = %ld"), lAccount);
        _bstr_t bstrSQL = szBuffer;
        CComVariant vRecCount;
        pRs = pConn->Execute(bstrSQL, &vRecCount, -1);

        // If such record not exist, the account doesn't exist
        if (vRecCount.lVal == 0) {
            TCHAR szBuffer1 [512];
            wsprintf(szBuffer1, _T("Error. Account %ld not on file."), lAccount);
            THROW_STR(szBuffer1);
        }

        // Get the appropriate fields
        hr =  pRs->get_Fields(&pFields);
        RETHROW_ERR(hr);
        // Get the appropriate field
        CComVariant vField = L"Balance";
        CComVariant vBalance;
        hr = pFields->get_Item(vField, &pField) ;
        RETHROW_ERR(hr);
        hr = pField->get_Value(&vBalance) ;
        RETHROW_ERR(hr);

        // Check for overdraw, then prepare return string
        if ((vBalance.lVal + lAmount) < 0) {
            TCHAR szBuffer2 [512];
            wsprintf(szBuffer2, _T("Error. Account %ld would be overdrawn by %ld.  Balance is still %ld"),
                     lAccount, -lAmount, vBalance.lVal); //, vBalance.lVal - lAmount);
            THROW_STR(szBuffer2);
        }

        // Update the account balance and
        // then check to see if account would be overdrawn
        wsprintf(szBuffer, _T("UPDATE Account SET Balance = Balance + %li WHERE AccountNo = %li"),
                 lAmount, lAccount);
        bstrSQL = szBuffer;
        vRecCount.Clear();
        pConn->Execute(bstrSQL, &vRecCount, -1);
        TCHAR szBuffer3 [512];
        wsprintf(szBuffer3, _T("%s account %ld, balance is $%ld."),
                 ((lAmount >= 0) ? _T("Credit to") : _T("Debit from")), lAccount, vBalance.lVal + lAmount);
        *pbstrResult = TCHAR2BSTR(szBuffer3);
    } catch (HRESULT hr) {
        //  ErrorInfo is saved here because the following ADO cleanup code
        //  may clear it.
        IErrorInfo * pErrorInfo = NULL;
        GetErrorInfo(NULL, &pErrorInfo);

        // Fill in error information
        switch (lErrFlag) {
        case (0): // Unknown error occurred in this object
            TCHAR szErr [512];
            wsprintf(szErr, _T("Error 0x%x from CAccount calling %s."), hr, pErrMsg);
            pErrMsg = szErr;

        // Fall through
        case (1): // An application error occurred in this object

            //  we are going to put our own error in TLS, so if there is one there, clear it
            if (pErrorInfo)
                pErrorInfo -> Release();

            AtlReportError(CLSID_Account, pErrMsg, IID_IAccount, hr);
            break;

        case (2): // ADO error
            //  put the error back in TLS
            SetErrorInfo(NULL, pErrorInfo);
            break;

        default: // Will never reach here
            break;
        }
    }

    return hr;
}

