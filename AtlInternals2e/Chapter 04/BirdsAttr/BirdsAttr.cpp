// BirdsAttr.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{1D172D25-115D-42C8-B8BD-B001B23AA715}",
         name = "BirdsAttr",
         helpstring = "BirdsAttr 1.0 Type Library",
         resource_name = "IDR_BIRDSATTR") ];
