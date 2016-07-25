// LogModule.h : main header file for the LOGMODULE DLL
//

#if !defined(AFX_LOGMODULE_H__83A4A081_C740_4D5E_9D68_B66B63D3A873__INCLUDED_)
#define AFX_LOGMODULE_H__83A4A081_C740_4D5E_9D68_B66B63D3A873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogModuleApp
// See LogModule.cpp for the implementation of this class
//

class CLogModuleApp : public CWinApp
{
public:
	CLogModuleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogModuleApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLogModuleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGMODULE_H__83A4A081_C740_4D5E_9D68_B66B63D3A873__INCLUDED_)
