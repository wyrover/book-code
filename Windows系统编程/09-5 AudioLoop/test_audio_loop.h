// test_audio_loop.h : main header file for the TEST_AUDIO_LOOP application
//

#if !defined(AFX_TEST_AUDIO_LOOP_H__7F109505_FC88_11D1_8334_00104B7B317F__INCLUDED_)
#define AFX_TEST_AUDIO_LOOP_H__7F109505_FC88_11D1_8334_00104B7B317F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopApp:
// See test_audio_loop.cpp for the implementation of this class
//

class CTest_audio_loopApp : public CWinApp
{
public:
	CTest_audio_loopApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_audio_loopApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTest_audio_loopApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_AUDIO_LOOP_H__7F109505_FC88_11D1_8334_00104B7B317F__INCLUDED_)
