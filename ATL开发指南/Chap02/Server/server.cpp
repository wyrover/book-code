//
// server.cpp -
//      Implements the external entry points for our DLL housing
//

#include <windows.h>

// Component category support
#include <comcat.h>

#include <initguid.h>
#include "math.h"

// Our component category GUID
#include "ATLDevGuide.h"

// Used to store the instance handle
// We need this to call the GetModuleFileName API
HINSTANCE g_hinstDLL = 0;

// Global instance and lock counts
long    g_lObjs = 0;
long    g_lLocks = 0;

//
// DllMain -
//    Called by Windows when important events happen
//    to the DLL. In particular, we are interested when
//    the DLL is initially loaded. When this ocurrs, we
//    save the instance handle for later use.
//
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        g_hinstDLL = hinstDLL;

    return TRUE;
}

//
// DllGetClassObject -
//      Called by COM to retrieve a component's class factory
//      implementation.
//
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    HRESULT             hr;
    MathClassFactory    *pCF;
    pCF = 0;

    // Make sure the CLSID is for our Expression component
    if (rclsid != CLSID_Math)
        return (E_FAIL);

    pCF = new MathClassFactory;

    if (pCF == 0)
        return (E_OUTOFMEMORY);

    hr = pCF->QueryInterface(riid, ppv);

    // Check for failure of QueryInterface
    if (FAILED(hr)) {
        delete pCF;
        pCF = 0;
    }

    return hr;
}

//
// DllCanUnloadNow -
//     Called by COM to determine if this housing (DLL)
//     can be unloaded
//
STDAPI DllCanUnloadNow(void)
{
    if (g_lObjs || g_lLocks)
        return (S_FALSE);
    else
        return (S_OK);
}

//
// SetRegKeyValue - Private function that updates the registry
//
static BOOL SetRegKeyValue(
    LPTSTR pszKey,
    LPTSTR pszSubkey,
    LPTSTR pszValue)
{
    BOOL bOk = FALSE;
    LONG ec;
    HKEY hKey;
    TCHAR szKey[128];
    lstrcpy(szKey, pszKey);

    if (NULL != pszSubkey) {
        lstrcat(szKey, "\\");
        lstrcat(szKey, pszSubkey);
    }

    ec = RegCreateKeyEx(
             HKEY_CLASSES_ROOT,
             szKey,
             0,
             NULL,
             REG_OPTION_NON_VOLATILE,
             KEY_ALL_ACCESS,
             NULL,
             &hKey,
             NULL);

    if (ERROR_SUCCESS == ec) {
        if (NULL != pszValue) {
            ec = RegSetValueEx(
                     hKey,
                     NULL,
                     0,
                     REG_SZ,
                     (BYTE *)pszValue,
                     (lstrlen(pszValue) + 1) * sizeof(TCHAR));
        }

        if (ERROR_SUCCESS == ec)
            bOk = TRUE;

        RegCloseKey(hKey);
    }

    return bOk;
}

//
// CreateComponentCategory - Uses the component categories manager
// to register a specific component category on the local machine.
//
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
    ICatRegister* pcr = 0;
    HRESULT hr;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICatRegister,
                          (void**)&pcr);

    if (FAILED(hr))
        return hr;

    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409;
    int len = wcslen(catDescription);
    wcsncpy(catinfo.szDescription, catDescription, wcslen(catDescription));
    catinfo.szDescription[len] = '\0';
    hr = pcr->RegisterCategories(1, &catinfo);
    pcr->Release();
    return hr;
}

//
// RegisterCLSIDInCategory - Uses the component categories
// manager to specify that the given component implements
// the specified component category
//
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    // Register your component categories information.
    ICatRegister* pcr = 0;
    HRESULT hr;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICatRegister,
                          (void**)&pcr);

    if (SUCCEEDED(hr)) {
        CATID rgcatid[1] ;
        rgcatid[0] = catid;
        hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
        pcr->Release();
    }

    return hr;
}

//
// DllRegisterServer - Entry point called by utilities such as
// REGSVR32.EXE to update the registry with the appropriate
// values for each component type in this DLL housing.
//
STDAPI DllRegisterServer(void)
{
    HRESULT  hr = NOERROR;
    CHAR    szModulePath[MAX_PATH];
    CHAR    szID[128];
    CHAR    szCLSID[128];
    WCHAR   wszID[128];
    WCHAR   wszCLSID[128];
    GetModuleFileName(
        g_hinstDLL,
        szModulePath,
        sizeof(szModulePath) / sizeof(CHAR));
    StringFromGUID2(CLSID_Math, wszID, sizeof(wszID));
    wcscpy(wszCLSID, L"CLSID\\");
    wcscat(wszCLSID, wszID);
    wcstombs(szID, wszID, sizeof(szID));
    wcstombs(szCLSID, wszCLSID, sizeof(szID));
    // Create the ProgID keys.
    SetRegKeyValue(
        "Chapter2.Math.1",
        NULL,
        "Chapter2 Math Component");
    SetRegKeyValue(
        "Chapter2.Math.1",
        "CLSID",
        szID);
    // Create version independent ProgID keys.
    SetRegKeyValue(
        "Chapter2.Math",
        NULL,
        "Chapter2 Math Component");
    SetRegKeyValue(
        "Chapter2.Math",
        "CurVer",
        "Chapter2.Math.1");
    SetRegKeyValue(
        "Chapter2.Math",
        "CLSID",
        szID);
    // Create entries under CLSID.
    SetRegKeyValue(
        szCLSID,
        NULL,
        "Chapter 2 Math Component");
    SetRegKeyValue(
        szCLSID,
        "ProgID",
        "Chapter2.Math.1");
    SetRegKeyValue(
        szCLSID,
        "VersionIndependentProgID",
        "Chapter2.Math");
    SetRegKeyValue(
        szCLSID,
        "InprocServer32",
        szModulePath);
    // Register our component category
    CreateComponentCategory(CATID_ATLDevGuide, L"ATL Developer's Guide Examples");
    RegisterCLSIDInCategory(CLSID_Math, CATID_ATLDevGuide);
    return S_OK;
}

//
// DllUnregisterServer - Entry point called by utilities such as
//                       REGSVR32.EXE to remove any registry values
//                       for the COM components in this housing.
//
STDAPI DllUnregisterServer(void)
{
    // For now, don't do anything...
    return S_OK;
}