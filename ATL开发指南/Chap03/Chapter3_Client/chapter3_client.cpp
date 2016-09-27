//
// Chapter3_Client.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

#include <initguid.h>

#include "..\Chapter3_Server\Chapter3_Server_i.c"
#include "..\Chapter3_Server\Chapter3_Server.h"

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    // Use CoCreateInstance
    IMath* pMath;
    HRESULT hr = CoCreateInstance(CLSID_Math,
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
