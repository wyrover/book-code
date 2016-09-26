// MoveMoney.cpp : Implementation of CMoveMoney
#include "stdafx.h"
#include "Bank.h"

#include "MoveMoney.h"
#include "Account.h"
#include "Receipt.h"

#include <mtx.h>
#include <comdef.h>

/////////////////////////////////////////////////////////////////////////////
// CMoveMoney

STDMETHODIMP CMoveMoney::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IMoveMoney
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CMoveMoney::Perform(long lPrimeAccount, long lSecondAccount, long lAmount,
                                 long lTranType, BSTR* pbstrResult)
{
    USES_CONVERSION;
    HRESULT hr = S_OK;
    IObjectContext* pObjectContext = NULL;
    IAccount* pObjAccount = NULL;
    IReceipt* pObjReceipt = NULL;
    long lErrFlag = 0;
    TCHAR* pErrMsg = NULL;
    BSTR bstrResult2 = NULL;
    BSTR bstrCall1 = NULL;
    BSTR bstrCall2 = NULL;
    *pbstrResult = NULL;

    try {
        // Get the object context
        THROW_ERR(GetObjectContext(&pObjectContext), "GetObjectContext");

        // Check security for large transfers
        if (lAmount > 500 || lAmount < -500) {
            BOOL bInRole;
            BSTR bstrRole = ::SysAllocString(L"Managers");
            hr = pObjectContext->IsCallerInRole(bstrRole, &bInRole);
            ::SysFreeString(bstrRole);

            if (!SUCCEEDED(hr)) {
                THROW_STR(_T("IsCallerInRole() call failed!  Please add the 'Managers' Roll to the package."));
            }

            if (!bInRole) {
                THROW_STR(_T("Need 'Managers' role for amounts over $500"));
            }
        }

        // Create the account object using our context
        THROW_ERR(pObjectContext->CreateInstance(CLSID_Account, IID_IAccount, (void**)&pObjAccount),
                  "CreateInstance(CLSID_CAccount)");

        // Call the post function based on the transaction type
        switch (lTranType) {
        case (1):   // debit
            RETHROW_ERR(pObjAccount->Post(lPrimeAccount, 0 - lAmount, &bstrResult2));
            break;

        case (2):   // credit
            RETHROW_ERR(pObjAccount->Post(lPrimeAccount, lAmount, &bstrResult2));
            break;

        case (3):   // transfer
            // Do the credit
            RETHROW_ERR(pObjAccount->Post(lSecondAccount, lAmount, &bstrCall1));
            // Then do the debit
            RETHROW_ERR(pObjAccount->Post(lPrimeAccount, 0 - lAmount, &bstrCall2));
            // Prepare return string
            TCHAR szBuf [512];
            _tcscpy(szBuf, W2T(bstrCall1));
            _tcscat(szBuf, _T("; "));
            _tcscat(szBuf, W2T(bstrCall2));
            bstrResult2 = TCHAR2BSTR(szBuf);
            break;

        default:
            THROW_STR(_T("Invalid Transaction Type"));
        }

        // Get Receipt Number for the transaction
        THROW_ERR(pObjectContext->CreateInstance(CLSID_Receipt, IID_IReceipt, (void**)&pObjReceipt),
                  "CreateInstance(CLSID_CGetReceipt)");
        long lngReceiptNo;
        RETHROW_ERR(pObjReceipt->GetNextReceipt(&lngReceiptNo));
        // Tag receipt information onto return string
        TCHAR szBuffer [512];
        wsprintf(szBuffer, _T("; Receipt No:  %li"), lngReceiptNo);
        BSTR bstrTemp = TCHAR2BSTR(szBuffer);
        TCHAR szBuffer2 [512];
        _tcscpy(szBuffer2, W2T(bstrResult2));
        _tcscat(szBuffer2, W2T(bstrTemp));
        ::SysFreeString(bstrTemp);
        *pbstrResult = TCHAR2BSTR(szBuffer2);
        // We are finished and happy
        pObjectContext->SetComplete();
        hr = S_OK;
    } catch (HRESULT hr) {
        //store error info locally
        IErrorInfo * pErrorInfo = NULL;
        GetErrorInfo(NULL, &pErrorInfo);

        // Fill in error information
        switch (lErrFlag) {
        case (0):// Unknown error occurred in this object
            TCHAR szErr [512];
            wsprintf(szErr, _T("Error 0x%x from CMoveMoney calling %s."), hr, pErrMsg);
            pErrMsg = szErr;

        // Fall through
        case (1):// An application error occurred in this object

            //we are going to put our own error in TLS, so if there is one there, clear it
            if (pErrorInfo)
                pErrorInfo -> Release();

            AtlReportError(CLSID_MoveMoney, pErrMsg, IID_IMoveMoney, hr);
            break;

        case (2):// An error occurred in a called object
            SetErrorInfo(NULL, pErrorInfo);//   put the error back in TLS
            break;

        default:// Will never reach here
            break;
        }

        // Indicate our unhappiness
        if (pObjectContext)
            pObjectContext->SetAbort();
    }

    // Release resources
    if (pObjAccount)
        pObjAccount->Release();

    if (pObjectContext)
        pObjectContext->Release();

    if (pObjReceipt)
        pObjReceipt->Release();

    if (bstrResult2)
        ::SysFreeString(bstrResult2);

    if (bstrCall1)
        ::SysFreeString(bstrCall1);

    if (bstrCall2)
        ::SysFreeString(bstrCall2);

    return hr;
}
