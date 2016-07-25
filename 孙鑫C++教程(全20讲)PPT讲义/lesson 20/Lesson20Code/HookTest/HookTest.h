// HookTest.h : main header file for the HOOKTEST application
//

#if !defined(AFX_HOOKTEST_H__D4E7E913_4357_4C66_B6A9_50A3622EA7C0__INCLUDED_)
#define AFX_HOOKTEST_H__D4E7E913_4357_4C66_B6A9_50A3622EA7C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHookTestApp:
// See HookTest.cpp for the implementation of this class
//

class CHookTestApp : public CWinApp
{
public:
	CHookTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHookTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHookTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKTEST_H__D4E7E913_4357_4C66_B6A9_50A3622EA7C0__INCLUDED_)
