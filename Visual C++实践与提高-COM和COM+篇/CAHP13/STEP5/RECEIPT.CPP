// Receipt.cpp : Implementation of CReceipt
#include "stdafx.h"
#include "Bank.h"
#include "Receipt.h"

#include <mtx.h>
#include <mtxspm.h>
/////////////////////////////////////////////////////////////////////////////
// CReceipt

STDMETHODIMP CReceipt::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IReceipt
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CReceipt::GetNextReceipt(long *plReceiptNo)
{
    HRESULT hr = S_OK;
    IObjectContext* pObjectContext = NULL;
    ISharedPropertyGroupManager* spmMgr = NULL;
    ISharedPropertyGroup* spmGroup = NULL;
    ISharedProperty* spmPropNextReceipt = NULL;
//  ISharedProperty* spmPropMaxNum = NULL;
    CComVariant vNextReceipt;
    long lErrFlag = 0;
    TCHAR* pErrMsg = NULL;

    try {
        // First of all, get the object context
        THROW_ERR(GetObjectContext(&pObjectContext), "GetObjectContext");
        // Create the SharedPropertyGroupManager
        THROW_ERR(pObjectContext->CreateInstance(CLSID_SharedPropertyGroupManager,
                  IID_ISharedPropertyGroupManager, (void**)&spmMgr), "CreateInstance(CLSID_SharedPropertyGroupManager)");
        // Create the SharedPropertyGroup
        LONG lIsolationMode = LockMethod;
        LONG lReleaseMode = Process;
        VARIANT_BOOL bExists = VARIANT_FALSE;
        THROW_ERR(spmMgr->CreatePropertyGroup(L"Receipt", &lIsolationMode, &lReleaseMode, &bExists, &spmGroup),
                  "CreatePropertyGroup");
        // Create the two shared properties
        THROW_ERR(spmGroup->CreateProperty(L"Next", &bExists, &spmPropNextReceipt), "CreateProperty(Next)");
        //  THROW_ERR ( spmGroup->CreateProperty (L"MaxNum", &bExists, &spmPropMaxNum), "CreateProperty(MaxNum)" );
        // Obtain their current values
//      CComVariant vMaxNum;
        THROW_ERR(spmPropNextReceipt->get_Value(&vNextReceipt), "spmPropNextReceipt->get_Value");
        //  THROW_ERR ( spmPropMaxNum->get_Value (&vMaxNum), "spmPropMaxNum->get_Value" );
        // Increment NextReceipt shared property
        vNextReceipt.lVal ++;
        THROW_ERR(spmPropNextReceipt->put_Value(vNextReceipt), "spmPropNextReceipt->put_Value");
        *plReceiptNo = vNextReceipt.lVal;
        hr = S_OK;
        // We're finished and happy
        pObjectContext->SetComplete();
    } catch (HRESULT hr) {
        // ErrorInfo is saved here because the following
        // ADO cleanup code     may clear it.
        IErrorInfo * pErrorInfo = NULL;
        GetErrorInfo(NULL, &pErrorInfo);

        // Fill in error information
        switch (lErrFlag) {
        case (0):// Unknown error occurred in this object
            TCHAR szErr [512];
            wsprintf(szErr, _T("Error 0x%x from CGetReceipt calling %s."), hr, pErrMsg);
            pErrMsg = szErr;

        // Fall through

        case (1):// An application error occurred in this object

            //  we are going to put our own error in TLS, so if there is one there, clear it
            if (pErrorInfo)
                pErrorInfo -> Release();

            AtlReportError(CLSID_Receipt, pErrMsg, IID_IReceipt, hr);
            break;

        case (2):   // An error occurred in a called object
            //  put the error back in TLS
            SetErrorInfo(NULL, pErrorInfo);
            break;

        default: // Will never reach here
            break;
        }

        // Indicate our unhappiness
        if (pObjectContext)
            pObjectContext->SetAbort();
    }

    // Resource cleanup
    if (pObjectContext)
        pObjectContext->Release();

    if (spmMgr)
        spmMgr->Release();

    if (spmGroup)
        spmGroup->Release();

    if (spmPropNextReceipt)
        spmPropNextReceipt->Release();

//  if (spmPropMaxNum)
//      spmPropMaxNum->Release();
//  if (pObjUpdateReceipt)
//      pObjUpdateReceipt->Release();
    return hr;
}
