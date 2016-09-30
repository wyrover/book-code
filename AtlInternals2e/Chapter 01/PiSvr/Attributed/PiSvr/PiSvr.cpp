// PiSvr.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{4D7B835B-E5A9-47C4-B48F-E359FFC464BB}",
         name = "PiSvr",
         helpstring = "PiSvr 1.0 Type Library",
         resource_name = "IDR_PISVR") ];
