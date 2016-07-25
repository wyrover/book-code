// BallsRace.h : main header file for the BALLSRACE application
//

#if !defined(AFX_BALLSRACE_H__30D4805A_6862_49F9_9225_6284B5AB1839__INCLUDED_)
#define AFX_BALLSRACE_H__30D4805A_6862_49F9_9225_6284B5AB1839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceApp:
// See BallsRace.cpp for the implementation of this class
//

class CBallsRaceApp : public CWinApp
{
public:
	CBallsRaceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBallsRaceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBallsRaceApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALLSRACE_H__30D4805A_6862_49F9_9225_6284B5AB1839__INCLUDED_)
