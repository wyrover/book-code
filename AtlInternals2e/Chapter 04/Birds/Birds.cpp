// Birds.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "Birds.h"

class CBirdsModule : public CAtlDllModuleT< CBirdsModule >
{
public :
    DECLARE_LIBID(LIBID_BirdsLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BIRDS, "{A4A4F11F-482C-46FB-8C2D-2D5048D71DB5}")
};

CBirdsModule _AtlModule;


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
