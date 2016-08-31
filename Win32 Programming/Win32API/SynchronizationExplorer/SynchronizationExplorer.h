// SynchronizationExplorer.h : main header file for the SYNCHRONIZATIONEXPLORER application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerApp:
// See SynchronizationExplorer.cpp for the implementation of this class
//

class CSynchronizationExplorerApp : public CWinApp
{
public:
	CSynchronizationExplorerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynchronizationExplorerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	CMultiDocTemplate * panelTemplate;

	//{{AFX_MSG(CSynchronizationExplorerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
