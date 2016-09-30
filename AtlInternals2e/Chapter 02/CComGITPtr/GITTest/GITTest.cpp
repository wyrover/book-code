// GITTest.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{27929090-3FC0-4BB4-8C81-6FD510EA27E6}",
         name = "GITTest",
         helpstring = "GITTest 1.0 Type Library",
         resource_name = "IDR_GITTEST") ];
