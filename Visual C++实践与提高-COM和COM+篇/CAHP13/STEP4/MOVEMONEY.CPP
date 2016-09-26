// MoveMoney.cpp : Implementation of CMoveMoney
#include "stdafx.h"
#include "Bank.h"

#include "MoveMoney.h"
#include "Account.h"
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
    long lErrFlag = 0;
    TCHAR* pErrMsg = NULL;
//  BSTR bstrResult2 = NULL;
    BSTR bstrCall1 = NULL;
    BSTR bstrCall2 = NULL;
    *pbstrResult = NULL;

    try {
        // Get the object context
        THROW_ERR(GetObjectContext(&pObjectContext), "GetObjectContext");
        // Create the account object using our context
        THROW_ERR(pObjectContext->CreateInstance(CLSID_Account, IID_IAccount, (void**)&pObjAccount),
                  "CreateInstance(CLSID_CAccount)");

        // Call the post function based on the transaction type
        switch (lTranType) {
        case (1):   // debit
            RETHROW_ERR(pObjAccount->Post(lPrimeAccount, 0 - lAmount, pbstrResult));
            break;

        case (2):   // credit
            RETHROW_ERR(pObjAccount->Post(lPrimeAccount, lAmount, pbstrResult));
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
            *pbstrResult = TCHAR2BSTR(szBuf);
            break;

        default:
            THROW_STR(_T("Invalid Transaction Type"));
        }

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

    if (bstrCall1)
        ::SysFreeString(bstrCall1);

    if (bstrCall2)
        ::SysFreeString(bstrCall2);

    return hr;
}
