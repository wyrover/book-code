// FtpFile.h : main header file for the FTPFILE application
//

#if !defined(AFX_FTPFILE_H__5786A2AA_3D7B_11D4_8C39_0000E839EC5E__INCLUDED_)
#define AFX_FTPFILE_H__5786A2AA_3D7B_11D4_8C39_0000E839EC5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFtpApp:
// See FtpFile.cpp for the implementation of this class
//

class CFtpApp : public CWinApp
{
public:
	CFtpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFtpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPFILE_H__5786A2AA_3D7B_11D4_8C39_0000E839EC5E__INCLUDED_)
