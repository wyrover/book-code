// Mybole.h : main header file for the MYBOLE application
//

#if !defined(AFX_MYBOLE_H__81899694_6ED4_4760_AD89_CA63A4FCA09A__INCLUDED_)
#define AFX_MYBOLE_H__81899694_6ED4_4760_AD89_CA63A4FCA09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyboleApp:
// See Mybole.cpp for the implementation of this class
//

class CMyboleApp : public CWinApp
{
public:
	CMyboleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyboleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyboleApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBOLE_H__81899694_6ED4_4760_AD89_CA63A4FCA09A__INCLUDED_)
