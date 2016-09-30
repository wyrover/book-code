// PiSvr.cpp : Implementation of DLL Exports.


#include "stdafx.h"
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
{
public:
// Override CAtlDllModuleT members
};

