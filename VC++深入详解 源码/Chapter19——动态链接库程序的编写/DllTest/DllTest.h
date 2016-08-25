// DllTest.h : main header file for the DLLTEST application
//

#if !defined(AFX_DLLTEST_H__9E79003D_59B1_4B1F_9E89_9B40323FAE4F__INCLUDED_)
#define AFX_DLLTEST_H__9E79003D_59B1_4B1F_9E89_9B40323FAE4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDllTestApp:
// See DllTest.cpp for the implementation of this class
//

class CDllTestApp : public CWinApp
{
public:
	CDllTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDllTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLTEST_H__9E79003D_59B1_4B1F_9E89_9B40323FAE4F__INCLUDED_)
