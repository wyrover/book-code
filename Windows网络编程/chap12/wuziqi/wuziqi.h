// wuziqi.h : main header file for the WUZIQI application
//

#if !defined(AFX_WUZIQI_H__18B2336F_1CC8_4A2C_9AC1_9BC6E80A3CA8__INCLUDED_)
#define AFX_WUZIQI_H__18B2336F_1CC8_4A2C_9AC1_9BC6E80A3CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWuziqiApp:
// See wuziqi.cpp for the implementation of this class
//

class CWuziqiApp : public CWinApp
{
public:
	CWuziqiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWuziqiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWuziqiApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WUZIQI_H__18B2336F_1CC8_4A2C_9AC1_9BC6E80A3CA8__INCLUDED_)
