// SendProcess.h : main header file for the SENDPROCESS application
//

#if !defined(AFX_SENDPROCESS_H__B6E68EF1_3F34_43C4_985E_865F297780ED__INCLUDED_)
#define AFX_SENDPROCESS_H__B6E68EF1_3F34_43C4_985E_865F297780ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSendProcessApp:
// See SendProcess.cpp for the implementation of this class
//

class CSendProcessApp : public CWinApp
{
public:
	CSendProcessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendProcessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSendProcessApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDPROCESS_H__B6E68EF1_3F34_43C4_985E_865F297780ED__INCLUDED_)
