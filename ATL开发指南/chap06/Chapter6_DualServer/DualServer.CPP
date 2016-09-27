//
// Chapter6_DualServer.cpp
//

#include <windows.h>
#include "DualServer.h"
#include "DualServer_i.c"

#include "Math.h"

DWORD       g_dwObjs = 0;
DWORD       g_dwLocks = 0;

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    HRESULT             hr;
    MathClassFactory    *pCF;

    // Make sure the CLSID is for our Expression component
    if (rclsid != CLSID_Math)
        return (E_FAIL);

    pCF = new MathClassFactory;

    if (pCF == NULL)
        return (E_OUTOFMEMORY);

    hr = pCF->QueryInterface(riid, ppv);

    if (FAILED(hr)) {
        delete pCF;
        *ppv = 0;
    }

    return hr;
}

STDAPI DllCanUnloadNow(void)
{
    if (g_dwObjs || g_dwLocks)
        return (S_FALSE);
    else
        return (S_OK);
}

