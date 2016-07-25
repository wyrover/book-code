// Philosophers.h : main header file for the PHILOSOPHERS application
//

#if !defined(AFX_PHILOSOPHERS_H__BBF5FD4C_5CD5_4ACE_B6F7_E2C58B6A25D2__INCLUDED_)
#define AFX_PHILOSOPHERS_H__BBF5FD4C_5CD5_4ACE_B6F7_E2C58B6A25D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersApp:
// See Philosophers.cpp for the implementation of this class
//

class CPhilosophersApp : public CWinApp
{
public:
	CPhilosophersApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhilosophersApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPhilosophersApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHILOSOPHERS_H__BBF5FD4C_5CD5_4ACE_B6F7_E2C58B6A25D2__INCLUDED_)
