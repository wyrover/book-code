// DemagogueNoAttr.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "DemagogueNoAttr.h"

class CDemagogueNoAttrModule : public CAtlDllModuleT< CDemagogueNoAttrModule >
{
public :
    DECLARE_LIBID(LIBID_DemagogueNoAttrLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DEMAGOGUENOATTR, "{2A6E96F0-7A89-462F-9DC1-5EEA3B91EF7D}")
};

CDemagogueNoAttrModule _AtlModule;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
    return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = _AtlModule.DllUnregisterServer();
    return hr;
}
