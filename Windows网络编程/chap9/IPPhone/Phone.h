// Phone.h : main header file for the PHONE application
//

#if !defined(AFX_PHONE_H__8F8DC625_0576_11D4_8D3A_000021E6766A__INCLUDED_)
#define AFX_PHONE_H__8F8DC625_0576_11D4_8D3A_000021E6766A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPhoneApp:
// See Phone.cpp for the implementation of this class
//

class CPhoneApp : public CWinApp
{
public:
	CPhoneApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPhoneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONE_H__8F8DC625_0576_11D4_8D3A_000021E6766A__INCLUDED_)
