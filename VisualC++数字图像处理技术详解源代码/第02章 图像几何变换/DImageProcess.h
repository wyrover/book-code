// DImageProcess.h : main header file for the DIMAGEPROCESS application
//

#if !defined(AFX_DIMAGEPROCESS_H__3351243B_88A2_4608_820D_909FBCDE6D49__INCLUDED_)
#define AFX_DIMAGEPROCESS_H__3351243B_88A2_4608_820D_909FBCDE6D49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessApp:
// See DImageProcess.cpp for the implementation of this class
//

class CDImageProcessApp : public CWinApp
{
public:
	CDImageProcessApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDImageProcessApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Implementation
	//{{AFX_MSG(CDImageProcessApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIMAGEPROCESS_H__3351243B_88A2_4608_820D_909FBCDE6D49__INCLUDED_)
