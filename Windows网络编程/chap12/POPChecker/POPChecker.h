// POPChecker.h : main header file for the POPCHECKER application
//

#if !defined(AFX_POPCHECKER_H__F712AA7C_7086_11D1_8833_00001C302581__INCLUDED_)
#define AFX_POPCHECKER_H__F712AA7C_7086_11D1_8833_00001C302581__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPOPCheckerApp:
// See POPChecker.cpp for the implementation of this class
//

class CPOPCheckerApp : public CWinApp
{
public:
	CPOPCheckerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPOPCheckerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPOPCheckerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPCHECKER_H__F712AA7C_7086_11D1_8833_00001C302581__INCLUDED_)
