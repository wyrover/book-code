// DllTest.h : main header file for the DLLTEST application
//

#if !defined(AFX_DLLTEST_H__C3E6806A_1742_42D4_83BB_B326FFC64027__INCLUDED_)
#define AFX_DLLTEST_H__C3E6806A_1742_42D4_83BB_B326FFC64027__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDllTestApp:
// See DllTest.cpp for the implementation of this class
//

class CDllTestApp : public CWinApp
{
public:
	CDllTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDllTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLTEST_H__C3E6806A_1742_42D4_83BB_B326FFC64027__INCLUDED_)
