// ASC.h : main header file for the ASC application
//

#if !defined(AFX_ASC_H__693485D3_8E35_4E79_B916_65B826BF109B__INCLUDED_)
#define AFX_ASC_H__693485D3_8E35_4E79_B916_65B826BF109B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CASCApp:
// See ASC.cpp for the implementation of this class
//

class CASCApp : public CWinApp
{
private:
	BOOL SetGlobleConnect();
public:
	CASCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CASCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CASCApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASC_H__693485D3_8E35_4E79_B916_65B826BF109B__INCLUDED_)
