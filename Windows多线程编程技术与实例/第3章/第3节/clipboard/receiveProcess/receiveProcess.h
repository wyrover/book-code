// receiveProcess.h : main header file for the RECEIVEPROCESS application
//

#if !defined(AFX_RECEIVEPROCESS_H__EF581037_526D_47E7_87A2_1DBE4425D745__INCLUDED_)
#define AFX_RECEIVEPROCESS_H__EF581037_526D_47E7_87A2_1DBE4425D745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CReceiveProcessApp:
// See receiveProcess.cpp for the implementation of this class
//

class CReceiveProcessApp : public CWinApp
{
public:
	CReceiveProcessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReceiveProcessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CReceiveProcessApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECEIVEPROCESS_H__EF581037_526D_47E7_87A2_1DBE4425D745__INCLUDED_)
