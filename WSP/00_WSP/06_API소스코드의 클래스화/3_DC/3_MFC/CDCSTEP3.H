// CDCStep3.h : main header file for the CDCSTEP3 application
//

#if !defined(AFX_CDCSTEP3_H__B1D51788_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_)
#define AFX_CDCSTEP3_H__B1D51788_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3App:
// See CDCStep3.cpp for the implementation of this class
//

class CCDCStep3App : public CWinApp
{
public:
	CCDCStep3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDCStep3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCDCStep3App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDCSTEP3_H__B1D51788_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_)
