// MathDll.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll,
         name = "MathDll",
         helpstring = "MathDll 1.0 Type Library",
         resource_name = "IDR_MATHDLL") ];
