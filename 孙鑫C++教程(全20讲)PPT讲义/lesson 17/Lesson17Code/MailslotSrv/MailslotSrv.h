// MailslotSrv.h : main header file for the MAILSLOTSRV application
//

#if !defined(AFX_MAILSLOTSRV_H__44D091B2_E75D_4D96_A502_49D3913E8C0D__INCLUDED_)
#define AFX_MAILSLOTSRV_H__44D091B2_E75D_4D96_A502_49D3913E8C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvApp:
// See MailslotSrv.cpp for the implementation of this class
//

class CMailslotSrvApp : public CWinApp
{
public:
	CMailslotSrvApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailslotSrvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMailslotSrvApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILSLOTSRV_H__44D091B2_E75D_4D96_A502_49D3913E8C0D__INCLUDED_)
