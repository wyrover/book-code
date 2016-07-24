// viewNIC.h : main header file for the VIEWNIC application
//

#if !defined(AFX_VIEWNIC_H__F75DE90B_21BA_472E_B663_9EBA101D3BB7__INCLUDED_)
#define AFX_VIEWNIC_H__F75DE90B_21BA_472E_B663_9EBA101D3BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CViewNICApp:
// See viewNIC.cpp for the implementation of this class
//

class CViewNICApp : public CWinApp
{
public:
	CViewNICApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewNICApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CViewNICApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWNIC_H__F75DE90B_21BA_472E_B663_9EBA101D3BB7__INCLUDED_)
