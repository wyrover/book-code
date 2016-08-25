// Draw.h : main header file for the DRAW application
//

#if !defined(AFX_DRAW_H__C7176B9A_0D1C_422A_B88C_55CBBE918AA2__INCLUDED_)
#define AFX_DRAW_H__C7176B9A_0D1C_422A_B88C_55CBBE918AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrawApp:
// See Draw.cpp for the implementation of this class
//

class CDrawApp : public CWinApp
{
public:
	CDrawApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDrawApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAW_H__C7176B9A_0D1C_422A_B88C_55CBBE918AA2__INCLUDED_)
