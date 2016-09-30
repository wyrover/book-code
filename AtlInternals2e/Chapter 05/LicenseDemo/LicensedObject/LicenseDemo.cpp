// LicenseDemo.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{9B52D164-65E1-4B3A-9B29-D9DF87328EC2}",
         name = "LicenseDemo",
         helpstring = "LicenseDemo 1.0 Type Library",
         resource_name = "IDR_LICENSEDEMO") ];
