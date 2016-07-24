// PipeTest.h : main header file for the PIPETEST application
//

#if !defined(AFX_PIPETEST_H__CD3D8C9A_D258_11D3_8DC4_0010A4FA75E3__INCLUDED_)
#define AFX_PIPETEST_H__CD3D8C9A_D258_11D3_8DC4_0010A4FA75E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPipeTestApp:
// See PipeTest.cpp for the implementation of this class
//

class CPipeTestApp : public CWinApp
{
public:
	CPipeTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPipeTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPipeTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPETEST_H__CD3D8C9A_D258_11D3_8DC4_0010A4FA75E3__INCLUDED_)
