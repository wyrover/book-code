// Chat.h : main header file for the CHAT application
//

#if !defined(AFX_CHAT_H__17F8EED2_7C8B_46B3_9CC3_86D5DD3ACB07__INCLUDED_)
#define AFX_CHAT_H__17F8EED2_7C8B_46B3_9CC3_86D5DD3ACB07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChatApp:
// See Chat.cpp for the implementation of this class
//

class CChatApp : public CWinApp
{
public:
	CChatApp();
	~CChatApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CChatApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAT_H__17F8EED2_7C8B_46B3_9CC3_86D5DD3ACB07__INCLUDED_)
