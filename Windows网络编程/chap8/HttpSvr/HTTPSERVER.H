// HttpServer.h : main header file for the HTTPSERVER application
//

#if !defined(AFX_HTTPSERVER_H__745FF0A5_8B0C_11D4_8DCA_0000E839EC5E__INCLUDED_)
#define AFX_HTTPSERVER_H__745FF0A5_8B0C_11D4_8DCA_0000E839EC5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHttpServerApp:
// See HttpServer.cpp for the implementation of this class
//

class CHttpServerApp : public CWinApp
{
public:
	CHttpServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHttpServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPSERVER_H__745FF0A5_8B0C_11D4_8DCA_0000E839EC5E__INCLUDED_)
