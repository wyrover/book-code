//
// Chapter10_FreeClient.cpp
//

#include <windows.h>
#include <iostream.h>


// Include ATL
#include <atlbase.h>
#include <atlimpl.cpp>

#define FREE_SERVER 0
#define BOTH_SERVER 0
#define APT_SERVER  1
#if FREE_SERVER
    #include "..\Chapter10_FreeServer\Chapter10_FreeServer.h"
    #include "..\Chapter10_FreeServer\Chapter10_FreeServer_i.c"
#elif BOTH_SERVER
    #include "..\Chapter10_BothServer\Chapter10_BothServer.h"
    #include "..\Chapter10_BothServer\Chapter10_BothServer_i.c"
#elif APT_SERVER
    #include "..\Chapter10_AptServer\Chapter10_AptServer.h"
    #include "..\Chapter10_AptServer\Chapter10_AptServer_i.c"
#endif

int main(int argc, char *argv[])
{
    //CoInitializeEx( 0, COINIT_APARTMENTTHREADED );
    CoInitializeEx(0, COINIT_MULTITHREADED);
    // Create an instance of our math component
    CComPtr<IMath> ptrMath;
    HRESULT hr;
    hr = CoCreateInstance(CLSID_Math,
                          NULL,
                          CLSCTX_SERVER,
                          IID_IMath,
                          (void**) &ptrMath);

    if (FAILED(hr)) {
        cout << "Failed to create server instance" << hr << endl;
        return -1;
    }

    // Access the IMath functionality
    long lResult;
    ptrMath->Add(300, 10, &lResult);
    ptrMath->Subtract(300, 10, &lResult);
    ptrMath->Divide(300, 10, &lResult);
    ptrMath->Multiply(300, 10, &lResult);

    // Release our interface
    if (ptrMath)
        ptrMath = 0;

    CoUninitialize();
    return 0;
}
