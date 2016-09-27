// OleClip.h : main header file for the OLECLIP application
//

#if !defined(AFX_OLECLIP_H__C4CDBEC6_616B_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_OLECLIP_H__C4CDBEC6_616B_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COleClipApp:
// See OleClip.cpp for the implementation of this class
//
#define WM_VIEWPALETTECHANGED   WM_USER + 5

class COleClipApp : public CWinApp
{
public:
    COleClipApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COleClipApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(COleClipApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLECLIP_H__C4CDBEC6_616B_11D4_A23D_02608CACEDBB__INCLUDED_)
