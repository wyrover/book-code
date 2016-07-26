// SmtpPop.h : main header file for the SMTPPOP application
//

#if !defined(AFX_SMTPPOP_H__1EC6B3E3_62DD_41D5_B11A_FCA7CBFCABF3__INCLUDED_)
#define AFX_SMTPPOP_H__1EC6B3E3_62DD_41D5_B11A_FCA7CBFCABF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSmtpPopApp:
// See SmtpPop.cpp for the implementation of this class
//

class CSmtpPopApp : public CWinApp
{
public:
	CSmtpPopApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmtpPopApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSmtpPopApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPPOP_H__1EC6B3E3_62DD_41D5_B11A_FCA7CBFCABF3__INCLUDED_)
