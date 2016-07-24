// CTelnet.h : main header file for the CTELNET application
//

#if !defined(AFX_CTELNET_H__FE95BC87_9860_11D3_8CD5_00C0F0405B24__INCLUDED_)
#define AFX_CTELNET_H__FE95BC87_9860_11D3_8CD5_00C0F0405B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTelnetApp:
// See CTelnet.cpp for the implementation of this class
//

class CTelnetApp : public CWinApp
{
public:
	CTelnetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelnetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTelnetApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTELNET_H__FE95BC87_9860_11D3_8CD5_00C0F0405B24__INCLUDED_)
