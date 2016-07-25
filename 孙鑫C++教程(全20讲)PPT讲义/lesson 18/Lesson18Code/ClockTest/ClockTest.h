// ClockTest.h : main header file for the CLOCKTEST application
//

#if !defined(AFX_CLOCKTEST_H__A1B3A4DB_9C34_4719_A021_5E61F85C4179__INCLUDED_)
#define AFX_CLOCKTEST_H__A1B3A4DB_9C34_4719_A021_5E61F85C4179__INCLUDED_

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

#endif // !defined(AFX_CLOCKTEST_H__A1B3A4DB_9C34_4719_A021_5E61F85C4179__INCLUDED_)
