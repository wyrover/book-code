// NamedPipeSrv.h : main header file for the NAMEDPIPESRV application
//

#if !defined(AFX_NAMEDPIPESRV_H__65A1E69B_197A_4CE9_B291_7A3658B687B5__INCLUDED_)
#define AFX_NAMEDPIPESRV_H__65A1E69B_197A_4CE9_B291_7A3658B687B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvApp:
// See NamedPipeSrv.cpp for the implementation of this class
//

class CNamedPipeSrvApp : public CWinApp
{
public:
	CNamedPipeSrvApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedPipeSrvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNamedPipeSrvApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDPIPESRV_H__65A1E69B_197A_4CE9_B291_7A3658B687B5__INCLUDED_)
