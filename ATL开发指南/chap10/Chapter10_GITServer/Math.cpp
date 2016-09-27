// Math.cpp : Implementation of CMath
#include "stdafx.h"
#include "Chapter10_GITServer.h"
#include "Math.h"

/////////////////////////////////////////////////////////////////////////////
// CMath

STDMETHODIMP CMath::Add(long lOp1, long lOp2)
{
    SimulateLongComputation(mathAdd, lOp1, lOp2);
    return S_OK;
}

STDMETHODIMP CMath::Subtract(long lOp1, long lOp2)
{
    SimulateLongComputation(mathSubtract, lOp1, lOp2);
    return S_OK;
}
STDMETHODIMP CMath::Multiply(long lOp1, long lOp2)
{
    SimulateLongComputation(mathMultiply, lOp1, lOp2);
    return S_OK;
}

STDMETHODIMP CMath::Divide(long lOp1, long lOp2)
{
    SimulateLongComputation(mathDivide, lOp1, lOp2);
    return S_OK;
}

STDMETHODIMP CMath::Advise(ICallBack * pCallBack)
{
    pCallBack->AddRef();
    m_pCallBack = pCallBack;
    // Add the pointer to the GIT
    extern DWORD gdwCookie;
    extern IGlobalInterfaceTable* gpGIT;
    HRESULT hr = gpGIT->RegisterInterfaceInGlobal(m_pCallBack,
                 IID_ICallBack,
                 &gdwCookie);
    return S_OK;
}

STDMETHODIMP CMath::Unadvise()
{
    extern DWORD gdwCookie;
    extern IGlobalInterfaceTable* gpGIT;
    HRESULT hr = gpGIT->RevokeInterfaceFromGlobal(gdwCookie);
    m_pCallBack->Release();
    m_pCallBack = 0;
    return S_OK;
}

DWORD WINAPI PerformComputation(void *p)
{
    CoInitialize(0);
    // Get the thread parameters
    ThreadParameters* pTP = (ThreadParameters*) p;
    // Get the ICallBack interface from the GIT
    HRESULT hr;
    extern IGlobalInterfaceTable* gpGIT;
    extern DWORD gdwCookie;
    ICallBack* pCallBack = 0;
    hr = gpGIT->GetInterfaceFromGlobal(gdwCookie,
                                       IID_ICallBack,
                                       (void**) & pCallBack);

    // We successfully retrieved the ICallBack interface
    if (SUCCEEDED(hr)) {
        // Perform the calculation and delay
        // to simulate doing some real work
        long lResult;

        switch (pTP->op) {
        case mathAdd:
            lResult = pTP->lOp1 + pTP->lOp2;
            break;

        case mathSubtract:
            lResult = pTP->lOp1 - pTP->lOp2;
            break;

        case mathMultiply:
            lResult = pTP->lOp1 * pTP->lOp2;
            break;

        case mathDivide:
            lResult = pTP->lOp1 / pTP->lOp2;
            break;
        }

        // Delay, but not too long
        Sleep(min(lResult, 5000));
        // Notify the client with the result of the computation
        pCallBack->ComputationComplete(lResult);
        // Release the pointer
        // GetInterfaceFromGlobal did an AddRef
        pCallBack->Release();
    }

    // Delete our thread parameters structure
    delete pTP;
    CoUninitialize();
    return hr;
}

HRESULT CMath::SimulateLongComputation(mathOPERATOR op, long lOp1, long lOp2)
{
    // Create the thread parameters object and
    // fill it out with our parameters
    ThreadParameters* pTP = new ThreadParameters;
    pTP->op = op;
    pTP->lOp1 = lOp1;
    pTP->lOp2 = lOp2;
    // Create the thread
    HANDLE  hThread;
    DWORD   dwThreadID;
    hThread = CreateThread(0, 0,
                           PerformComputation,
                           pTP,
                           0,
                           &dwThreadID);

    // If we have a hande, then everything
    // worked so close the handle
    if (hThread) {
        CloseHandle(hThread);
    } else {
        // Thread creation failed.
        delete pTP;
        // Notify the client that an error occurred
        CComBSTR bstrMsg("Unable to start computation thread");
        m_pCallBack->Error(bstrMsg);
    }

    return S_OK;
}
