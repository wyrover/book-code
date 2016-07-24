// ViewAdapterInfo.h : main header file for the VIEWADAPTERINFO application
//

#if !defined(AFX_VIEWADAPTERINFO_H__FA611197_033B_46E7_8714_58CCB85FBC17__INCLUDED_)
#define AFX_VIEWADAPTERINFO_H__FA611197_033B_46E7_8714_58CCB85FBC17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CViewAdapterInfoApp:
// See ViewAdapterInfo.cpp for the implementation of this class
//

class CViewAdapterInfoApp : public CWinApp
{
public:
	CViewAdapterInfoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAdapterInfoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CViewAdapterInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWADAPTERINFO_H__FA611197_033B_46E7_8714_58CCB85FBC17__INCLUDED_)
