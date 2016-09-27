// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__031A0674_825B_11D4_8C0E_446260000000__INCLUDED_)
#define AFX_STDAFX_H__031A0674_825B_11D4_8C0E_446260000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include "adosamp.h"

#import "c:\program files\common files\system\ado\msado15.dll" \
no_namespace rename("EOF","adoEOF")

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
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__031A0674_825B_11D4_8C0E_446260000000__INCLUDED)
