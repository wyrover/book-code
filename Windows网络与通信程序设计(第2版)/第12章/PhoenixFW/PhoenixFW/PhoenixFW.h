// PhoenixFW.h : main header file for the PHOENIXFW application
//

#if !defined(AFX_PHOENIXFW_H__CFE3B11F_FE91_417C_97CA_EEC655927041__INCLUDED_)
#define AFX_PHOENIXFW_H__CFE3B11F_FE91_417C_97CA_EEC655927041__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPhoenixApp:
// See PhoenixFW.cpp for the implementation of this class
//

class CPhoenixApp : public CWinApp
{
protected:
	HANDLE m_hSemaphore;

public:
	CPhoenixApp();

	static BOOL SetAutoStart(BOOL bStart);
	static BOOL ApplyFileData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoenixApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPhoenixApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOENIXFW_H__CFE3B11F_FE91_417C_97CA_EEC655927041__INCLUDED_)
