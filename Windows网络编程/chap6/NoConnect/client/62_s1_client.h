// 62_s1_client.h : main header file for the 62_S1_CLIENT application
//

#if !defined(AFX_62_S1_CLIENT_H__EF4702A6_ACDB_11D3_BDE9_FC695D1D960B__INCLUDED_)
#define AFX_62_S1_CLIENT_H__EF4702A6_ACDB_11D3_BDE9_FC695D1D960B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy62_s1_clientApp:
// See 62_s1_client.cpp for the implementation of this class
//

class CMy62_s1_clientApp : public CWinApp
{
public:
	CMy62_s1_clientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy62_s1_clientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy62_s1_clientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_62_S1_CLIENT_H__EF4702A6_ACDB_11D3_BDE9_FC695D1D960B__INCLUDED_)
