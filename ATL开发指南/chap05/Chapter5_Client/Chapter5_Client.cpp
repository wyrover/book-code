//
// Chapter5_Client.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

const IID IID_IAdvancedMath
    = {0x380F79F9, 0x4098, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

const IID IID_IMath
    = {0x380F79EA, 0x4098, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

// This gets us the Vtable layouts
// For IMath and IAdvancedMath
#include "..\Chapter5_Simple\Chapter5_Simple.h"
#include "..\Chapter5_Aggregate\Chapter5_Aggregate.h"

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    CLSID clsid;
    HRESULT hr;
#define AGGREGATE
#ifdef AGGREGATE
    cout << "Creating aggregate component" << endl;
    hr = CLSIDFromProgID(L"Chapter5.Aggregate.1", &clsid);
#else
    hr = CLSIDFromProgID(L"Chapter5.Containment.1", &clsid);
    cout << "Creating containment component" << endl;
#endif

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to convert Progid. HR = " << hr << endl;
        return -1;
    }

    // Use CoCreateInstance
    IMath* pMath;
    hr = CoCreateInstance(clsid,
                          NULL,
                          CLSCTX_INPROC,
                          IID_IMath,
                          (void**) &pMath);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        return -1;
    }

    cout << "Instance created" << endl;
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
