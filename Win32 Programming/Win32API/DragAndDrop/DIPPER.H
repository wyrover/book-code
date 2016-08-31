// dipper.h : main header file for the DIPPER application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDipperApp:
// See dipper.cpp for the implementation of this class
//

class CDipperApp : public CWinApp
{
public:
	CDipperApp();
	static int Status;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDipperApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDipperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
