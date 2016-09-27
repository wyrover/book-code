//
// Client.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

#include <initguid.h>
#include "..\server\imath.h"


int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    // Get the unique CLSID from the ProgID
    CLSID clsid;
    HRESULT hr = ::CLSIDFromProgID(L"Chapter2.Math.1", &clsid);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Unable to get CLSID from ProgID. HR = " << hr << endl;
        return -1;
    }

    // Get the class factory for the Math class
    IClassFactory* pCF;
    hr = CoGetClassObject(clsid,
                          CLSCTX_INPROC,
                          NULL,
                          IID_IClassFactory,
                          (void**) &pCF);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to GetClassObject server instance. HR = " << hr << endl;
        return -1;
    }

    // using the class factory interface create an instance of the
    // component and return the IUnknown interface.
    IUnknown* pUnk;
    hr = pCF->CreateInstance(NULL, IID_IUnknown, (void**) &pUnk);
    // Release the class factory
    pCF->Release();

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        return -1;
    }

    cout << "Instance created" << endl;
    IMath* pMath = NULL;
    hr = pUnk->QueryInterface(IID_IMath, (LPVOID*)&pMath);
    pUnk->Release();

    if (FAILED(hr)) {
        cout << "QueryInterface() for IMath failed" << endl;
        CoUninitialize();
        return -1;
    }

    long result;
    pMath->Multiply(100, 8, &result);
    cout << "100 * 8 is " << result << endl;
    pMath->Subtract(1000, 333, &result);
    cout << "1000 - 333 is " << result << endl;
    // Try IAdvancedMath, QI through IMath
    IAdvancedMath* pAdvMath = NULL;
    hr = pMath->QueryInterface(IID_IAdvancedMath, (LPVOID*)&pAdvMath);

    if (FAILED(hr)) {
        cout << "QueryInterface() for IAdvancedMath failed" << endl;
        pMath->Release();
        CoUninitialize();
        return -1;
    }

    pAdvMath->Factorial(10, &result);
    cout << "10! is " << result << endl;
    pAdvMath->Fibonacci(10, &result);
    cout << "The Fibonacci of 10 is " << result << endl;
    cout << "Releasing IMath interface" << endl;
    pMath->Release();
    cout << "Releasing IAdvancedMath interface" << endl;
    pAdvMath->Release();
    cout << "Shuting down COM" << endl;
    CoUninitialize();
    return 0;
}
