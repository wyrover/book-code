// sequenceMult.h : main header file for the SEQUENCEMULT application
//

#if !defined(AFX_SEQUENCEMULT_H__58DEB89E_8BA3_47D7_949E_0E4D4D0DB1F4__INCLUDED_)
#define AFX_SEQUENCEMULT_H__58DEB89E_8BA3_47D7_949E_0E4D4D0DB1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSequenceMultApp:
// See sequenceMult.cpp for the implementation of this class
//

class CSequenceMultApp : public CWinApp
{
public:
	CSequenceMultApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSequenceMultApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSequenceMultApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEQUENCEMULT_H__58DEB89E_8BA3_47D7_949E_0E4D4D0DB1F4__INCLUDED_)
