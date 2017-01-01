// HttpClient.h : interface of the CmHttpClient class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cactus_curl.h"
#include <atlstr.h>

///////////////////////////////////////////////////////////////////////////////
// CmHttpClient

class CmHttpClient
{
private:
    cactus::CHttpClient m_pClient;

public:
    CmHttpClient();
    ~CmHttpClient();

private:
	int			GetProxyModeConfig();
	BOOL		GetProxyNeedAuthConfig();
	CString		GetProxyHostConfig();
	CString		GetProxyPortConfig();
	CString		GetProxyUserNameConfig();
	CString		GetProxyPasswordConfig();

	BOOL		SetProxy();

public:
	BOOL		Get(LPCTSTR szUrl, CStringA &strReceiveContent, DWORD *pErrorCode = NULL);    
    BOOL		Post2(LPCTSTR szUrl, BYTE *pData, ULONG ulSize, CStringA &strReceiveContent, DWORD *pErrorCode = NULL);	
};

