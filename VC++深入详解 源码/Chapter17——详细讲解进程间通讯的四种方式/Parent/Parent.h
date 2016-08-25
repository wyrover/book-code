// Parent.h : main header file for the PARENT application
//

#if !defined(AFX_PARENT_H__747FFCBC_3F18_4D69_BCDD_4A4078E5931C__INCLUDED_)
#define AFX_PARENT_H__747FFCBC_3F18_4D69_BCDD_4A4078E5931C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CParentApp:
// See Parent.cpp for the implementation of this class
//

class CParentApp : public CWinApp
{
public:
	CParentApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParentApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CParentApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARENT_H__747FFCBC_3F18_4D69_BCDD_4A4078E5931C__INCLUDED_)
