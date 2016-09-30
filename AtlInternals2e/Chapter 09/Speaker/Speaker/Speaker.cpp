// Speaker.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{C46C911C-AC08-4B50-925A-0342801AA934}",
         name = "Speaker",
         helpstring = "Speaker 1.0 Type Library",
         resource_name = "IDR_SPEAKER") ];
