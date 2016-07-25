// ClockTest.h : main header file for the CLOCKTEST application
//

#if !defined(AFX_CLOCKTEST_H__97C49665_10A4_4DEE_B44C_DF55B1004654__INCLUDED_)
#define AFX_CLOCKTEST_H__97C49665_10A4_4DEE_B44C_DF55B1004654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClockTestApp:
// See ClockTest.cpp for the implementation of this class
//

class CClockTestApp : public CWinApp
{
public:
	CClockTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClockTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKTEST_H__97C49665_10A4_4DEE_B44C_DF55B1004654__INCLUDED_)
