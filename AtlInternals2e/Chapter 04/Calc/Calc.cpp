// Calc.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "Calc.h"

class CCalcModule : public CAtlDllModuleT< CCalcModule >
{
public :
    DECLARE_LIBID(LIBID_CalcLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CALC, "{D657A2C9-E033-44A5-AF10-9BC3071EA4B7}")
};

CCalcModule _AtlModule;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    BOOL b = _AtlModule.DllMain(dwReason, lpReserved);
#ifdef _ATL_DEBUG_INTERFACES
    _AtlDebugInterfacesModule.m_nIndexBreakAt = 4;
#endif
    return b;
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
