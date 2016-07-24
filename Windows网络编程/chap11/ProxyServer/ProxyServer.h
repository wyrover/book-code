// ProxyServer.h : main header file for the PROXYSERVER application
//

#if !defined(AFX_PROXYSERVER_H__D7E5F9DC_8BB9_4F8F_AD03_3FBF2A936C16__INCLUDED_)
#define AFX_PROXYSERVER_H__D7E5F9DC_8BB9_4F8F_AD03_3FBF2A936C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProxyServerApp:
// See ProxyServer.cpp for the implementation of this class
//

class CProxyServerApp : public CWinApp
{
public:
	CProxyServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProxyServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProxyServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROXYSERVER_H__D7E5F9DC_8BB9_4F8F_AD03_3FBF2A936C16__INCLUDED_)
