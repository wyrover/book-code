// UnsafeStringHandling.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "UnsafeStringHandling.h"
#include <string.h>

class CUnsafeStringHandlingModule : public CAtlDllModuleT< CUnsafeStringHandlingModule >
{
public :
    DECLARE_LIBID(LIBID_UnsafeStringHandlingLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_UNSAFESTRINGHANDLING, "{B0D1067B-13E8-4515-B84B-8E770490573A}")
};

CUnsafeStringHandlingModule _AtlModule;


#ifdef _MANAGED
    #pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}

#ifdef _MANAGED
    #pragma managed(pop)
#endif




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


///////
// Sample unsafe string handling function

void BuildUrl(char *dest, size_t dest_len, char *page)
{
    strncpy(dest, "http://localhost/Sample", dest_len);
    strncat(dest, page, dest_len);
    strncat(dest, "/default.srf", dest_len);
}

void SafeBuildUrl(char * dest, size_t dest_len, char *page)
{
    // _TRUNCATE is a special value that tells the function to
    // copy as much of source as will fit in dest and still have a
    // \0 terminator.
    strncpy_s(dest, dest_len, "http://localhost/Sample", _TRUNCATE);
    strncat_s(dest, dest_len, page, _TRUNCATE);
    strncat_s(dest, dest_len, "/default.srf", _TRUNCATE);
}
