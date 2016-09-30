// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// c:\kirk\atl internals 2e\src\chapter 05\licensedemo\licensedobject\licensedemo.mrg.cpp
// compiler-generated file created 11/10/02 at 13:40:28
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// LicenseDemo.cpp : Implementation of DLL Exports.

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{9B52D164-65E1-4B3A-9B29-D9DF87328EC2}",
         name = "LicenseDemo",
         helpstring = "LicenseDemo 1.0 Type Library",
         resource_name = "IDR_LICENSEDEMO") ];

//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
}
GUID;
#endif

extern "C" const __declspec(selectany) GUID __LIBID_ = {0x9b52d164, 0x65e1, 0x4b3a, {0x9b, 0x29, 0xd9, 0xdf, 0x87, 0x32, 0x8e, 0xc2}};
struct __declspec(uuid("9b52d164-65e1-4b3a-9b29-d9df87328ec2")) LicenseDemo;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"

class CLicenseDemoModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"

extern CLicenseDemoModule _AtlModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllRegisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllUnregisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllCanUnloadNow(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"

class CLicenseDemoModule : public CAtlDllModuleT<CLicenseDemoModule>

{
public:
    DECLARE_LIBID(__uuidof(LicenseDemo))
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LICENSEDEMO, "{9B52D164-65E1-4B3A-9B29-D9DF87328EC2}")

};

#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"

CLicenseDemoModule _AtlModule;

#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllRegisterServer(void)
{
    return _AtlModule.DllRegisterServer();
}
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllUnregisterServer(void)
{
    return _AtlModule.DllUnregisterServer();
}
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
extern "C" STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}
#injected_line 7 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\licensedemo.cpp"

#if defined(_M_IX86)
    #pragma comment(linker, "/EXPORT:DllMain=_DllMain@12,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#elif defined(_M_IA64)
    #pragma comment(linker, "/EXPORT:DllMain,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllRegisterServer,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllUnregisterServer,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
    #pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#endif

//--- End Injected Code For Attribute 'module'

