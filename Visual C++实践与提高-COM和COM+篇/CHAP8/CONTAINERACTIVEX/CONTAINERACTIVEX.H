// ContainerActiveX.h : main header file for the CONTAINERACTIVEX application
//

#if !defined(AFX_CONTAINERACTIVEX_H__0E340322_930B_11D4_896A_9349D3D7A679__INCLUDED_)
#define AFX_CONTAINERACTIVEX_H__0E340322_930B_11D4_896A_9349D3D7A679__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CContainerActiveXApp:
// See ContainerActiveX.cpp for the implementation of this class
//

class CContainerActiveXApp : public CWinApp
{
public:
    CContainerActiveXApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContainerActiveXApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CContainerActiveXApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINERACTIVEX_H__0E340322_930B_11D4_896A_9349D3D7A679__INCLUDED_)
