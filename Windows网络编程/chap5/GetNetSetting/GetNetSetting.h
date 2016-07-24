// GetNetSetting.h : main header file for the GETNETSETTING application
//

#if !defined(AFX_GETNETSETTING_H__BA4100FD_7576_4A37_9B51_270A8ECFD8DE__INCLUDED_)
#define AFX_GETNETSETTING_H__BA4100FD_7576_4A37_9B51_270A8ECFD8DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGetNetSettingApp:
// See GetNetSetting.cpp for the implementation of this class
//

class CGetNetSettingApp : public CWinApp
{
public:
	CGetNetSettingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetNetSettingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGetNetSettingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETNETSETTING_H__BA4100FD_7576_4A37_9B51_270A8ECFD8DE__INCLUDED_)
