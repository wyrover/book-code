// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__5496F79B_24C3_4942_9200_FF5382CDFA22__INCLUDED_)
#define AFX_STDAFX_H__5496F79B_24C3_4942_9200_FF5382CDFA22__INCLUDED_

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
#include <map>
#include <string>

typedef std::map<std::string, double> QUOTES;

class CServiceModule : public CComModule
{
public:
    HRESULT RegisterServer(BOOL bRegTypeLib, BOOL bService);
    HRESULT UnregisterServer();
    void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid = NULL);
    void Start();
    void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    void Handler(DWORD dwOpcode);
    void Run();
    BOOL IsInstalled();
    BOOL Install();
    BOOL Uninstall();
    LONG Unlock();
    void LogEvent(LPCTSTR pszFormat, ...);
    void SetServiceStatus(DWORD dwState);
    void SetupAsLocalServer();

//Implementation
private:
    static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI _Handler(DWORD dwOpcode);
    QUOTES theQMap;
    QUOTES::iterator theIterator;
// data members
public:
    void CacheQuotes();
    void GetQuote(BSTR bstrTicker, double *pPrice);
    TCHAR m_szServiceName[256];
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_status;
    DWORD dwThreadID;
    BOOL m_bService;
};

extern CServiceModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5496F79B_24C3_4942_9200_FF5382CDFA22__INCLUDED)
