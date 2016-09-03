// Exp_13_3_2.h : main header file for the EXP_13_3_2 application
//

#if !defined(AFX_EXP_13_3_2_H__0F2A795C_4059_47F5_8879_60E1CDA2FFDF__INCLUDED_)
#define AFX_EXP_13_3_2_H__0F2A795C_4059_47F5_8879_60E1CDA2FFDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExp_13_3_2App:
// See Exp_13_3_2.cpp for the implementation of this class
//

class CExp_13_3_2App : public CWinApp
{
public:
	CExp_13_3_2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExp_13_3_2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExp_13_3_2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXP_13_3_2_H__0F2A795C_4059_47F5_8879_60E1CDA2FFDF__INCLUDED_)
