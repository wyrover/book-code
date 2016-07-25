// OpenglExa.h : main header file for the OPENGLEXA application
//

#if !defined(AFX_OPENGLEXA_H__774CA61D_489A_479D_A533_1838F4E41350__INCLUDED_)
#define AFX_OPENGLEXA_H__774CA61D_489A_479D_A533_1838F4E41350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COpenglExaApp:
// See OpenglExa.cpp for the implementation of this class
//

class COpenglExaApp : public CWinApp
{
public:
	COpenglExaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenglExaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COpenglExaApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLEXA_H__774CA61D_489A_479D_A533_1838F4E41350__INCLUDED_)
