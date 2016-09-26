//
// Math.cpp : Implementation of CMath
//

#include "stdafx.h"
#include "Chapter10_Server.h"
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
    m_pCallBack = pCallBack;
    return S_OK;
}

STDMETHODIMP CMath::Unadvise()
{
    m_pCallBack = 0;
    return S_OK;
}

DWORD WINAPI PerformComputation(void *p)
{
    CoInitialize(0);
    // Get the thread parameters
    ThreadParameters* pTP = (ThreadParameters*) p;
    // Unmarshal the ICallBack pointer
    ICallBack* pCallBack = 0;
    HRESULT hr = CoGetInterfaceAndReleaseStream(pTP->pStream,
                 IID_ICallBack,
                 (void**) &pCallBack);

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
    }

    // Delete our thread parameters structure
    delete pTP;
    CoUninitialize();
    return hr;
}

HRESULT CMath::SimulateLongComputation(mathOPERATOR op, long lOp1, long lOp2)
{
    // Marshal the ICallBack interface into a stream
    IStream* pStream = 0;
    HRESULT hr = CoMarshalInterThreadInterfaceInStream(IID_ICallBack,
                 m_pCallBack,
                 &pStream);

    if (SUCCEEDED(hr)) {
        // Create the thread parameters object and
        // fill it out with our parameters
        ThreadParameters* pTP = new ThreadParameters;
        pTP->op = op;
        pTP->lOp1 = lOp1;
        pTP->lOp2 = lOp2;
        pTP->pStream = pStream;
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
            // Thread creation failed. Destroy
            // our parameters and release the stream
            delete pTP;
            pStream->Release();
            // Notify the client that an error occurred
            CComBSTR bstrMsg("Unable to start computation thread");
            m_pCallBack->Error(bstrMsg);
        }
    } else {
        // If the marshaling fails, we probably don't have
        // the proxy/stub registered
        CComBSTR bstrMsg("Unable to marshal the ICallBack interface. \
                         Make sure the proxy/stub is registered");
        m_pCallBack->Error(bstrMsg);
    }

    return hr;
}
