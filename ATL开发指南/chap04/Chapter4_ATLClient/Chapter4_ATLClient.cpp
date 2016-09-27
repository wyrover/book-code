//
// Chapter4_ATLClient.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

#include <atlbase.h>
#include <atlimpl.cpp>

#include "..\Chapter4_Server\Chapter4_Server_i.c"
#include "..\Chapter4_Server\Chapter4_Server.h"

void HandleError(IUnknown* pUnk, REFIID riid)
{
    HRESULT hr;
    // See if the object supports rich error info
    ISupportErrorInfo* pSEI = 0;
    hr = pUnk->QueryInterface(IID_ISupportErrorInfo, (void**) &pSEI);

    if (SUCCEEDED(hr)) {
        hr = pSEI->InterfaceSupportsErrorInfo(riid);

        if (SUCCEEDED(hr)) {
            // Get the error info
            IErrorInfo* pEI;

            if (SUCCEEDED(GetErrorInfo(0, &pEI))) {
                USES_CONVERSION;
                BSTR bstrDescription = 0;
                BSTR bstrSource = 0;
                pEI->GetDescription(&bstrDescription);
                pEI->GetSource(&bstrSource);
                cout << OLE2T(bstrDescription) << endl;
                cout << OLE2T(bstrSource) << endl;
                ::SysFreeString(bstrDescription);
                ::SysFreeString(bstrSource);
                pEI->Release();
            }
        }

        pSEI->Release();
    }
}

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    CComPtr< IMath > ptrMath;
    HRESULT hr;
    // This time use CoCreateInstance
    hr = CoCreateInstance(CLSID_Math,
                          NULL,
                          CLSCTX_LOCAL_SERVER,
                          IID_IMath,
                          (void**) &ptrMath);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    // Access the IMath interface
    long lResult;
    ptrMath->Add(134, 353, &lResult);
    cout << "134 + 353 = " << lResult << endl;
    // Try to divide by zero
    hr = ptrMath->Divide(0, 0, &lResult);

    if (FAILED(hr)) {
        HandleError(ptrMath, IID_IMath);
    }

#if 0
    // Access IMath2
    CComQIPtr<IMath2,
              &IID_IMath2> ptrMath2(ptrMath);

    if (ptrMath2) {
        ptrMath2->Compute(mathAdd,
                          100,
                          200,
                          &lResult);
        cout << "100 + 200 = " << lResult << endl;
    }

    // Access IAdvancedMath
    CComQIPtr<IAdvancedMath,
              &IID_IAdvancedMath> ptrAdvancedMath(ptrMath);

    if (ptrAdvancedMath) {
        ptrAdvancedMath->Factorial(12, &lResult);
        cout << "12! = " << lResult << endl;
        ptrAdvancedMath->Fibonacci(12, &lResult);
        cout << "The Fibonacci of 12 = " << lResult << endl;
    }

    // Access IComponentInfo
    CComQIPtr<IComponentInfo,
              &IID_IComponentInfo> ptrCompInfo(ptrMath);

    if (ptrCompInfo) {
        COMPONENT_INFO* pInfo = 0;
        ptrCompInfo->get_Info(&pInfo);
        cout << "Component author is " << pInfo->pstrAuthor << endl;
        cout << "Component version is " << pInfo->sMajor << "." << pInfo->sMinor << endl;
        USES_CONVERSION;
        cout << "Component name is " << OLE2T(pInfo->bstrName) << endl;

        if (pInfo->pstrAuthor)
            CoTaskMemFree(pInfo->pstrAuthor);

        if (pInfo->bstrName)
            SysFreeString(pInfo->bstrName);

        if (pInfo)
            CoTaskMemFree(pInfo);
    }

#endif

    // Release the smart pointer before CoUninitialize
    if (ptrMath)
        ptrMath = 0;

#if 0

    if (ptrMath2)
        ptrMath2->Release();

    if (ptrAdvancedMath)
        ptrAdvancedMath->Release();

    if (ptrCompInfo)
        ptrCompInfo->Release();

#endif
    CoUninitialize();
    return 0;
}
