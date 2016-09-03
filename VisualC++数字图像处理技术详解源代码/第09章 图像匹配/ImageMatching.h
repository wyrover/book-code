// ImageMatching.h : main header file for the IMAGEMATCHING application
//

#if !defined(AFX_IMAGEMATCHING_H__37F004A1_302D_47E1_9B2F_546097C2F258__INCLUDED_)
#define AFX_IMAGEMATCHING_H__37F004A1_302D_47E1_9B2F_546097C2F258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingApp:
// See ImageMatching.cpp for the implementation of this class
//

class CImageMatchingApp : public CWinApp
{
public:
	CImageMatchingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageMatchingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageMatchingApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMATCHING_H__37F004A1_302D_47E1_9B2F_546097C2F258__INCLUDED_)
