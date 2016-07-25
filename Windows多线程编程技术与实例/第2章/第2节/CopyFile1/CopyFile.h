// CopyFile.h : main header file for the COPYFILE application
//

#if !defined(AFX_COPYFILE_H__1CFA5720_5448_41A1_A4A7_3C4B45F82A37__INCLUDED_)
#define AFX_COPYFILE_H__1CFA5720_5448_41A1_A4A7_3C4B45F82A37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCopyFileApp:
// See CopyFile.cpp for the implementation of this class
//

class CCopyFileApp : public CWinApp
{
public:
	CCopyFileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyFileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCopyFileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYFILE_H__1CFA5720_5448_41A1_A4A7_3C4B45F82A37__INCLUDED_)
