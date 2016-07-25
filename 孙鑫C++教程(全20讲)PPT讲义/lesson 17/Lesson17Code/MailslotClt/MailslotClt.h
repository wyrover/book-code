// MailslotClt.h : main header file for the MAILSLOTCLT application
//

#if !defined(AFX_MAILSLOTCLT_H__F5DAFE3F_65A7_48B3_BB3F_339F47F9248C__INCLUDED_)
#define AFX_MAILSLOTCLT_H__F5DAFE3F_65A7_48B3_BB3F_339F47F9248C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltApp:
// See MailslotClt.cpp for the implementation of this class
//

class CMailslotCltApp : public CWinApp
{
public:
	CMailslotCltApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailslotCltApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMailslotCltApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILSLOTCLT_H__F5DAFE3F_65A7_48B3_BB3F_339F47F9248C__INCLUDED_)
