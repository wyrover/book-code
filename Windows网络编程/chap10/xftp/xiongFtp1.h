// xiongFtp.h : main header file for the XIONGFTP application
//

#if !defined(AFX_XIONGFTP_H__0647C99C_F9F6_4B89_97A0_F406D38A9B29__INCLUDED_)
#define AFX_XIONGFTP_H__0647C99C_F9F6_4B89_97A0_F406D38A9B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpApp:
// See xiongFtp.cpp for the implementation of this class
//

class CXiongFtpApp : public CWinApp
{
public:
	CXiongFtpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXiongFtpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CXiongFtpApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XIONGFTP_H__0647C99C_F9F6_4B89_97A0_F406D38A9B29__INCLUDED_)
