// TcpSpy.h : main header file for the TCPSPY application
//

#if !defined(AFX_TCPSPY_H__ECCF8FA5_7FA7_11D6_B72C_0050BAED0C65__INCLUDED_)
#define AFX_TCPSPY_H__ECCF8FA5_7FA7_11D6_B72C_0050BAED0C65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTcpSpyApp:
// See TcpSpy.cpp for the implementation of this class
//

class CTcpSpyApp : public CWinApp
{
public:
	CTcpSpyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpSpyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTcpSpyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPSPY_H__ECCF8FA5_7FA7_11D6_B72C_0050BAED0C65__INCLUDED_)
