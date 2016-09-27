//
// Chapter9_EnumClient.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

// Include component category support
#include <comcat.h>

// Include basic ATL support for its conversion macros
#include <atlbase.h>

// Include support for our basic EnumServer
#include "..\Chapter9_EnumServer\Chapter9_EnumServer_i.c"
#include "..\Chapter9_EnumServer\Chapter9_EnumServer.h"


int main(int argc, char* argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

//#define COMPCAT_CLIENT
#ifdef COMPCAT_CLIENT
    HRESULT hr;
    ICatInformation* pci = 0;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICatInformation,
                          (void**) &pci);
    CATID catidImpl[1];
    catidImpl[0] = CATID_Control;
    IEnumCLSID* pEnum = 0;
    hr = pci->EnumClassesOfCategories(1,
                                      catidImpl,
                                      0, 0,
                                      &pEnum);
    // Spin through the list and add them to the listbox
    GUID guids[1];
    ULONG ulNum;
    hr = pEnum->Next(1, guids, &ulNum);

    while (hr == S_OK) {
        // Look up the ProgID
        WCHAR* pProgID = 0;
        ProgIDFromCLSID(guids[0], &pProgID);
        WCHAR* pCLSIDString = 0;
        StringFromCLSID(guids[0], &pCLSIDString);
        // Display controls ProgID and CLSID
        USES_CONVERSION;
        cout << W2A(pProgID) << " - " << W2A(pCLSIDString) << endl;
        // Free up any memory
        CoTaskMemFree(pProgID);
        CoTaskMemFree(pCLSIDString);
        hr = pEnum->Next(1, guids, &ulNum);
    }

    // Release the interface pointers
    pEnum->Release();
    pci->Release();
#else
    HRESULT hr;
    IBasicEnum* pbe = 0;
    hr = CoCreateInstance(CLSID_BasicEnum,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IBasicEnum,
                          (void**) &pbe);
    IEnumString* pEnum = 0;
    hr = pbe->get_Enum(&pEnum);

    if (SUCCEEDED(hr)) {
        LPOLESTR string[1];
        ULONG ulNum;
        hr = pEnum->Next(1, string, &ulNum);

        while (hr == S_OK) {
            // Display string
            USES_CONVERSION;
            cout << W2A(string[0]) << endl;
            hr = pEnum->Next(1, string, &ulNum);
        }

        pEnum->Release();
    }

    pbe->Release();
#endif
    CoUninitialize();
    return 0;
}
