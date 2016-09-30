// Created by Microsoft (R) C/C++ Compiler Version 14.00.50727.42
//
// c:\home\chris\projects\atlinternals2e\src\appendixd\pisvr\pisvr\pisvr.mrg.cpp
// compiler-generated file created 02/13/06 at 22:12:20
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//



//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" const __declspec(selectany) GUID __LIBID_ = {0x5247b726, 0x8cb9, 0x450c, {0x96, 0x36, 0x9c, 0x57, 0x81, 0xb6, 0x97, 0x29}};
struct __declspec(uuid("5247b726-8cb9-450c-9636-9c5781b69729")) PiSvr;

//--- End Injected Code For Attribute 'module'
// PiSvr.cpp : Implementation of DLL Exports.


#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "resource.h"


// The module attribute causes DllMain,
// DllRegisterServer and DllUnregisterServer
//to be automatically implemented for you
[ module(dll,
         uuid = "{5247B726-8CB9-450C-9636-9C5781B69729}",
         name = "PiSvr",
         helpstring = "PiSvr 1.0 Type Library",
         resource_name = "IDR_PISVR") ]
class CPiSvrModule
    :
/*+++ Added Baseclass */ public ATL::CAtlDllModuleT<CPiSvrModule>
{
public:
// Override CAtlDllModuleT members

    //+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"

    DECLARE_LIBID(__uuidof(PiSvr))
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PISVR, "{5247B726-8CB9-450C-9636-9C5781B69729}")


    //--- End Injected Code For Attribute 'module'
};

//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"

extern CPiSvrModule _AtlModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"

#ifndef DLLMAIN_HINSTANCE
    #ifndef _WIN32_WCE
        #define DLLMAIN_HINSTANCE   HINSTANCE
    #else
        #define DLLMAIN_HINSTANCE   HANDLE
    #endif
#endif

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllRegisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllUnregisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllCanUnloadNow(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
BOOL WINAPI DllMain(DLLMAIN_HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"

CPiSvrModule _AtlModule;

#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllRegisterServer(void)
{
    return _AtlModule.DllRegisterServer();
}
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllUnregisterServer(void)
{
    return _AtlModule.DllUnregisterServer();
}
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"

#pragma managed(push, off)
BOOL WINAPI DllMain(DLLMAIN_HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}
#pragma managed(pop)
#injected_line 11 "c:\\home\\chris\\projects\\atlinternals2e\\src\\appendixd\\pisvr\\pisvr\\pisvr.cpp"

#if defined(_M_IX86)
    #pragma comment(linker, "/EXPORT:DllMain=_DllMain@12,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#elif defined(_M_IA64) || defined(_M_AMD64)
    #pragma comment(linker, "/EXPORT:DllMain,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllRegisterServer,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllUnregisterServer,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#endif

//--- End Injected Code For Attribute 'module'


