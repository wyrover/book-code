// Clipboard.h : main header file for the CLIPBOARD application
//

#if !defined(AFX_CLIPBOARD_H__04C9C59A_AEA1_4825_9AA9_04007BA92350__INCLUDED_)
#define AFX_CLIPBOARD_H__04C9C59A_AEA1_4825_9AA9_04007BA92350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClipboardApp:
// See Clipboard.cpp for the implementation of this class
//

class CClipboardApp : public CWinApp
{
public:
	CClipboardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClipboardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClipboardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIPBOARD_H__04C9C59A_AEA1_4825_9AA9_04007BA92350__INCLUDED_)
