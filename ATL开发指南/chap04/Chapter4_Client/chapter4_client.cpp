//
// Chapter4_Client.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

// We include these for BSTR/ANSI conversions
#include <atlbase.h>
#include <atlimpl.cpp>

#include "..\Chapter4_Server\Chapter4_Server_i.c"
#include "..\Chapter4_Server\Chapter4_Server.h"

// GUIDs, enumerates types, interface declarations, etc.
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

    // BSTR comparison...
    CComBSTR bstrA("Tom");
    CComBSTR bstrB("Tom");

    if (::SysStringByteLen(bstrA) == ::SysStringByteLen(bstrB) &&
        ::memcmp(bstrA, bstrB, ::SysStringByteLen(bstrA)) == 0) {
        cout << "bstrA == bstrB" << endl;
    }

    IMath* pMath;
    HRESULT hr;
    // This time use CoCreateInstance
    hr = CoCreateInstance(CLSID_Math,
                          NULL,
                          CLSCTX_LOCAL_SERVER,
                          IID_IMath,
                          (void**) &pMath);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    // Access the IMath interface
    long lResult;
    pMath->Add(134, 353, &lResult);
    cout << "134 + 353 = " << lResult << endl;
    // Try to divide by zero
    hr = pMath->Divide(0, 0, &lResult);

    if (FAILED(hr)) {
        // Use our new HandleError function to
        // display any rich error information
        HandleError(pMath, IID_IMath);
    }

    // Access IMath2
    IMath2* pMath2;
    hr = pMath->QueryInterface(IID_IMath2,
                               (void**) &pMath2);

    if (SUCCEEDED(hr)) {
        // Here's our new Compute method that
        // uses the mathOPERATOR enumerated type
        pMath2->Compute(mathAdd,
                        100,
                        200,
                        &lResult);
        cout << "100 + 200 = " << lResult << endl;
        short sArray[3] = { 3, 4, 5 };
        pMath2->Sum(3,
                    sArray,
                    &lResult);
        cout << "3 + 4 + 5 = " << lResult << endl;
    }

    // Access IAdvancedMath
    IAdvancedMath* pAdvancedMath = 0;
    hr = pMath->QueryInterface(IID_IAdvancedMath,
                               (void**) &pAdvancedMath);

    if (SUCCEEDED(hr)) {
        pAdvancedMath->Factorial(12, &lResult);
        cout << "12! = " << lResult << endl;
        pAdvancedMath->Fibonacci(12, &lResult);
        cout << "The Fibonacci of 12 = " << lResult << endl;
    }

    // Access IComponentInfo
    IComponentInfo* pCompInfo;
    hr = pMath->QueryInterface(IID_IComponentInfo,
                               (void**) &pCompInfo);

    if (SUCCEEDED(hr)) {
        // Pass in a pointer. The component
        // will allocate and return the structure
        COMPONENT_INFO* pInfo = 0;
        pCompInfo->get_Info(&pInfo);
        // Display the contents of the structure
        cout << "Component author is " << pInfo->pstrAuthor << endl;
        cout << "Component version is " << pInfo->sMajor << "." << pInfo->sMinor << endl;
        USES_CONVERSION;
        cout << "Component name is " << OLE2T(pInfo->bstrName) << endl;

        // Free any memory allocated by the component
        if (pInfo->pstrAuthor)
            CoTaskMemFree(pInfo->pstrAuthor);

        if (pInfo->bstrName)
            SysFreeString(pInfo->bstrName);

        if (pInfo)
            CoTaskMemFree(pInfo);
    }

    // Release all of our interfaces
    if (pMath)
        pMath->Release();

    if (pMath2)
        pMath2->Release();

    if (pAdvancedMath)
        pAdvancedMath->Release();

    if (pCompInfo)
        pCompInfo->Release();

    CoUninitialize();
    return 0;
}
