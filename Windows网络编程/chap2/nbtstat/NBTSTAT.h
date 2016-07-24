// NBTSTAT.h : main header file for the NBTSTAT application
//

#if !defined(AFX_NBTSTAT_H__DE6CDDC5_A43B_11D5_A956_0050BA0F0366__INCLUDED_)
#define AFX_NBTSTAT_H__DE6CDDC5_A43B_11D5_A956_0050BA0F0366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNBTSTATApp:
// See NBTSTAT.cpp for the implementation of this class
//

class CNBTSTATApp : public CWinApp
{
public:
	CNBTSTATApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNBTSTATApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNBTSTATApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NBTSTAT_H__DE6CDDC5_A43B_11D5_A956_0050BA0F0366__INCLUDED_)
