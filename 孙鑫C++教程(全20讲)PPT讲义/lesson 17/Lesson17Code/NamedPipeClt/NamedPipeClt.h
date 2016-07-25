// NamedPipeClt.h : main header file for the NAMEDPIPECLT application
//

#if !defined(AFX_NAMEDPIPECLT_H__E820C732_7684_482B_81D1_07533B635E2F__INCLUDED_)
#define AFX_NAMEDPIPECLT_H__E820C732_7684_482B_81D1_07533B635E2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltApp:
// See NamedPipeClt.cpp for the implementation of this class
//

class CNamedPipeCltApp : public CWinApp
{
public:
	CNamedPipeCltApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedPipeCltApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNamedPipeCltApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDPIPECLT_H__E820C732_7684_482B_81D1_07533B635E2F__INCLUDED_)
