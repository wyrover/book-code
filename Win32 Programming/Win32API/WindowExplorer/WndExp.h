// Window Explorer.h : main header file for the WINDOW EXPLORER application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWndExpApp:
// See Window Explorer.cpp for the implementation of this class
//

class CWndExpApp : public CWinApp
{
public:
	CWndExpApp();

	static int UWM_KILL;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndExpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	BOOL registerGenericClass(int id, LPCTSTR cursorid);

	//{{AFX_MSG(CWndExpApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
