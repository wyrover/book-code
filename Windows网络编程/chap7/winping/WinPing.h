// WinPing.h : main header file for the WINPING application
//

#if !defined(AFX_WINPING_H__DF2A85FD_E570_4347_9B1E_6A5E35A575DA__INCLUDED_)
#define AFX_WINPING_H__DF2A85FD_E570_4347_9B1E_6A5E35A575DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinPingApp:
// See WinPing.cpp for the implementation of this class
//

class CWinPingApp : public CWinApp
{
public:
	CWinPingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinPingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINPING_H__DF2A85FD_E570_4347_9B1E_6A5E35A575DA__INCLUDED_)
