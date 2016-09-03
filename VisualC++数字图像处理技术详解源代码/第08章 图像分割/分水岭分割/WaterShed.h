// WaterShed.h : main header file for the WATERSHED application
//

#if !defined(AFX_WATERSHED_H__C965E715_95C5_48E6_992E_8B355EC2EDBF__INCLUDED_)
#define AFX_WATERSHED_H__C965E715_95C5_48E6_992E_8B355EC2EDBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWaterShedApp:
// See WaterShed.cpp for the implementation of this class
//

class CWaterShedApp : public CWinApp
{
public:
	CWaterShedApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterShedApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWaterShedApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERSHED_H__C965E715_95C5_48E6_992E_8B355EC2EDBF__INCLUDED_)
