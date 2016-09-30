// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// c:\kirk\atl internals 2e\second edition\src\chapter 01\pisvrwebservice\pisvr\pisvr.mrg.cpp
// compiler-generated file created 05/12/02 at 00:28:51
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// PiSvr.cpp : Defines the entry point for the DLL application.
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

//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"

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

extern "C" const __declspec(selectany) GUID __LIBID_ = {0xae8c53f7, 0x0852, 0x3a2b, {0xa9, 0x7c, 0xfd, 0xe5, 0x1c, 0xa6, 0xa3, 0x16}};
struct __declspec(uuid("ae8c53f7-0852-3a2b-a97c-fde51ca6a316")) MyPiSvr;

//--- End Injected Code For Attribute 'module'

#endif

#include "PiSvr.h"
[ module(name = "MyPiSvr", type = "dll") ]
class CDllMainOverride
    :
/*+++ Added Baseclass */ public CAtlDllModuleT<CDllMainOverride>
{
public:
    BOOL WINAPI DllMain(DWORD dwReason, LPVOID lpReserved)
    {
#if defined(_M_IX86)

        if (dwReason == DLL_PROCESS_ATTACH) {
            // stack overflow handler
            _set_security_error_handler(AtlsSecErrHandlerFunc);
        }

#endif
        return __super::DllMain(dwReason, lpReserved);
    }

    //+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"

    DECLARE_LIBID(__uuidof(MyPiSvr))

    //--- End Injected Code For Attribute 'module'
};

//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"

extern CDllMainOverride _AtlModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllRegisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllUnregisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllCanUnloadNow(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"

CDllMainOverride _AtlModule;

#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllRegisterServer(void)
{
    return _AtlModule.DllRegisterServer();
}
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllUnregisterServer(void)
{
    return _AtlModule.DllUnregisterServer();
}
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
extern "C" STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}
#injected_line 11 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.cpp"

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
