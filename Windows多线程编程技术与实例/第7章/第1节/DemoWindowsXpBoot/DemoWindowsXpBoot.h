// DemoWindowsXpBoot.h : main header file for the DEMOWINDOWSXPBOOT application
//

#if !defined(AFX_DEMOWINDOWSXPBOOT_H__DD1E5A15_CC06_4A9F_A218_FB3162297F82__INCLUDED_)
#define AFX_DEMOWINDOWSXPBOOT_H__DD1E5A15_CC06_4A9F_A218_FB3162297F82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemoWindowsXpBootApp:
// See DemoWindowsXpBoot.cpp for the implementation of this class
//

class CDemoWindowsXpBootApp : public CWinApp
{
public:
	CDemoWindowsXpBootApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoWindowsXpBootApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemoWindowsXpBootApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOWINDOWSXPBOOT_H__DD1E5A15_CC06_4A9F_A218_FB3162297F82__INCLUDED_)
