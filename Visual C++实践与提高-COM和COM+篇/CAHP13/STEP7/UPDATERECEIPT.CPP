// UpdateReceipt.cpp : Implementation of CUpdateReceipt
#include "stdafx.h"
#include "Bank.h"
#include "UpdateReceipt.h"

#include <mtx.h>
#include <adoid.h>
#include <adoint.h>
/////////////////////////////////////////////////////////////////////////////
// CUpdateReceipt

STDMETHODIMP CUpdateReceipt::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IUpdateReceipt
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CUpdateReceipt::Update(long *plReceiptNo)
{
    HRESULT hr = S_OK;
    IObjectContext* pObjectContext = NULL;
    ADOConnection* adoCoConnection = NULL;
    ADORecordset* adoRsReceipt = NULL;
    ADOFields* pFields = NULL;
    ADOField* pField = NULL;
    long lErrFlag = 0;
    TCHAR* pErrMsg = NULL;
    *plReceiptNo = 0;

    try {
        // First of all, get the object context
        THROW_ERR(GetObjectContext(&pObjectContext), "GetObjectContext");
        // Create ADOConnection object and initialize the connection
        THROW_ERR(CoCreateInstance(CLSID_CADOConnection, NULL, CLSCTX_INPROC_SERVER,
                                   IID_IADOConnection, (LPVOID *) &adoCoConnection), "CoCreateInstance(CLSID_CADOConnection)");
        BSTR bstrDSN = ::SysAllocString(L"FILEDSN=bank");
        hr = adoCoConnection->Open(bstrDSN, NULL, NULL, adCmdUnspecified);
        ::SysFreeString(bstrDSN);
        RETHROW_ERR(hr);
        // Update the next receipt record
        CComVariant vAdoNull;
        TCHAR szBuffer [512];
        wsprintf(szBuffer, _T("Update Receipt set NextReceipt = NextReceipt + 100"));
        BSTR bstrSQL = TCHAR2BSTR(szBuffer);
        hr = adoCoConnection->Execute(bstrSQL, &vAdoNull, -1, NULL);
        ::SysFreeString(bstrSQL);
        RETHROW_ERR(hr);
        // Obtain the desired recordset with an SQL query
        bstrSQL = ::SysAllocString(L"SELECT NextReceipt FROM Receipt");
        hr = adoCoConnection->Execute(bstrSQL, &vAdoNull, -1, &adoRsReceipt);
        ::SysFreeString(bstrSQL);
        RETHROW_ERR(hr);
        // Get the appropriate fields
        RETHROW_ERR(adoRsReceipt->get_Fields(&pFields));
        // Get the appropriate field
        CComVariant vField = L"NextReceipt";
        RETHROW_ERR(pFields->get_Item(vField, &pField));
        CComVariant vNextReceipt;
        RETHROW_ERR(pField->get_Value(&vNextReceipt));

        // Cleanup resources
        if (pField) pField->Release();

        if (pFields) pFields->Release();

        if (adoRsReceipt) adoRsReceipt->Release();

        if (adoCoConnection) adoCoConnection->Release();

        // We are finished and happy
        pObjectContext->SetComplete();
        // Prepare return value
        *plReceiptNo = vNextReceipt.lVal;
        hr = S_OK;
    } catch (HRESULT hr) {
        //
        //  ErrorInfo is saved here because the following ADO cleanup code
        //  may clear it.
        //
        IErrorInfo * pErrorInfo = NULL;
        GetErrorInfo(NULL, &pErrorInfo);

        if (pField) pField->Release();

        if (pFields) pFields->Release();

        if (adoRsReceipt) adoRsReceipt->Release();

        if (adoCoConnection) adoCoConnection->Release();

        // Fill in error information
        switch (lErrFlag) {
        // Unknown error occurred in this object
        case (0):
            TCHAR szErr [512];
            wsprintf(szErr, _T("Error 0x%x from CUpdateReceipt calling %s."), hr, pErrMsg);
            pErrMsg = szErr;

        // Fall through

        // An application error occurred in this object
        case (1):

            //
            //  we are going to put our own error in TLS, so if there is one there, clear it
            //
            if (pErrorInfo)
                pErrorInfo -> Release();

            AtlReportError(CLSID_UpdateReceipt, pErrMsg, IID_IUpdateReceipt, hr);
            break;

        case (2): // An error occurred in a called object
            //
            //  put the error back in TLS
            //
            SetErrorInfo(NULL, pErrorInfo);
            break;

        // Will never reach here
        default:
            break;
        }

        // Indicate our unhappiness
        if (pObjectContext)
            pObjectContext->SetAbort();
    }

    // Resource cleanup
    if (pObjectContext) pObjectContext->Release();

    return hr;
}
