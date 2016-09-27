// Filename: stdafx.h
//
// Description: include file for standard system include files,
//              or project specific include files that are used frequently,
//              but are changed infrequently
//
// This file is provided as part of the Microsoft Transaction Server
// Software Development Kit
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT
// WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND/OR FITNESS FOR A  PARTICULAR
// PURPOSE.
//
// Copyright (C) 1997 Microsoft Corporation, All rights reserved

#define _ATL_STATIC_REGISTRY

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#define THROW_ERR(exp,str) {if (FAILED (hr = (exp))) {lErrFlag = 0; pErrMsg = _T(str); throw hr;}}
#define THROW_STR(str) { hr = APP_ERROR; lErrFlag = 1; pErrMsg = str; throw hr;}
#define RETHROW_ERR(exp) { if (FAILED(hr = (exp))) {lErrFlag = 2; throw hr;}}

#define APP_ERROR 0x80004100


inline BSTR TCHAR2BSTR(TCHAR* szBuf)
{
    WCHAR* wszBuf;
#ifndef UNICODE
    wszBuf = new WCHAR [512];
    MultiByteToWideChar(CP_ACP, 0, szBuf, -1, wszBuf, 512);
#else
    wszBuf = szBuf;
#endif
    BSTR bstrNew = ::SysAllocString(wszBuf);
#ifndef UNICODE
    delete [] wszBuf;
#endif
    return bstrNew;
}

