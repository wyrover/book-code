// Menu2.h : main header file for the MENU2 application
//

#if !defined(AFX_MENU2_H__FEFB1BD3_CB6C_4C97_A3C0_DA42D5D925FD__INCLUDED_)
#define AFX_MENU2_H__FEFB1BD3_CB6C_4C97_A3C0_DA42D5D925FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMenu2App:
// See Menu2.cpp for the implementation of this class
//

class CMenu2App : public CWinApp
{
public:
	CMenu2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenu2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMenu2App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENU2_H__FEFB1BD3_CB6C_4C97_A3C0_DA42D5D925FD__INCLUDED_)
