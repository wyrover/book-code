// namedPipe.h : main header file for the NAMEDPIPE application
//

#if !defined(AFX_NAMEDPIPE_H__EE1D99B9_BE46_407F_B5A3_F3F0120A3423__INCLUDED_)
#define AFX_NAMEDPIPE_H__EE1D99B9_BE46_407F_B5A3_F3F0120A3423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeApp:
// See namedPipe.cpp for the implementation of this class
//

class CNamedPipeApp : public CWinApp
{
public:
	CNamedPipeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedPipeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNamedPipeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDPIPE_H__EE1D99B9_BE46_407F_B5A3_F3F0120A3423__INCLUDED_)
