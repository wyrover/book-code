// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// c:\kirk\atl internals 2e\second edition\src\chapter 01\pisvrwebapp\pisvrwebappisapi\pisvrwebappisapi.mrg.cpp
// compiler-generated file created 05/10/02 at 23:49:44
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// PiSvrWebAppIsapi.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
    CDebugReportHook g_ReportHook;
#endif
[ module(name = "MyPiSvrWebApp", type = "dll") ];

//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"

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

extern "C" const __declspec(selectany) GUID __LIBID_ = {0xf41a3938, 0x2261, 0x35d3, {0x9e, 0xd7, 0xf5, 0x92, 0x56, 0xf8, 0x8c, 0x0d}};
struct __declspec(uuid("f41a3938-2261-35d3-9ed7-f59256f88c0d")) MyPiSvrWebApp;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"

class CMyPiSvrWebAppModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"

extern CMyPiSvrWebAppModule _AtlModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllRegisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllUnregisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllCanUnloadNow(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"

class CMyPiSvrWebAppModule : public CAtlDllModuleT<CMyPiSvrWebAppModule>

{
public:
    DECLARE_LIBID(__uuidof(MyPiSvrWebApp))

};

#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"

CMyPiSvrWebAppModule _AtlModule;

#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllRegisterServer(void)
{
    return _AtlModule.DllRegisterServer();
}
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllUnregisterServer(void)
{
    return _AtlModule.DllUnregisterServer();
}
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
extern "C" STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}
#injected_line 9 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebappisapi\\pisvrwebappisapi.cpp"

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

[ emitidl(restricted) ];

typedef CIsapiExtension<> ExtensionType;

// The ATL Server ISAPI extension
ExtensionType theExtension;


// Delegate ISAPI exports to theExtension
//
extern "C" DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpECB)
{
    return theExtension.HttpExtensionProc(lpECB);
}

extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
    return theExtension.GetExtensionVersion(pVer);
}

extern "C" BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
    return theExtension.TerminateExtension(dwFlags);
}
