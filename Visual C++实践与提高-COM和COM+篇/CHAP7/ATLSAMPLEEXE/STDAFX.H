// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__6EA21C23_D1C4_11D4_A239_503748C10000__INCLUDED_)
#define AFX_STDAFX_H__6EA21C23_D1C4_11D4_A239_503748C10000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>

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

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
    LONG Unlock();
    DWORD dwThreadID;
    HANDLE hEventShutdown;
    void MonitorShutdown();
    bool StartMonitor();
    bool bActivity;
};
extern CExeModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6EA21C23_D1C4_11D4_A239_503748C10000__INCLUDED)
