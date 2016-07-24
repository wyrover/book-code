// ipmon.h : main header file for the IPMON application
//

#if !defined(AFX_IPMON_H__47076947_0324_11D5_924F_00010219EDF4__INCLUDED_)
#define AFX_IPMON_H__47076947_0324_11D5_924F_00010219EDF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIpmonApp:
// See ipmon.cpp for the implementation of this class
//

class CIpmonApp : public CWinApp
{
public:
	CIpmonApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpmonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIpmonApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPMON_H__47076947_0324_11D5_924F_00010219EDF4__INCLUDED_)
