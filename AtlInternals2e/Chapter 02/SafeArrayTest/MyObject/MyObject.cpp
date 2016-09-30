// MyObject.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{12C2969A-1E90-4FA7-AC88-A2D1C402AD40}",
         name = "MyObject",
         helpstring = "MyObject 1.0 Type Library",
         resource_name = "IDR_MYOBJECT") ];
