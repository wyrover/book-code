// Redirect.h : main header file for the REDIRECT application
//

#if !defined(AFX_REDIRECT_H__FD1BCAF4_7733_4B99_A5BA_B3892CB5170B__INCLUDED_)
#define AFX_REDIRECT_H__FD1BCAF4_7733_4B99_A5BA_B3892CB5170B__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRedirectApp:
// See Redirect.cpp for the implementation of this class
//

class CRedirectApp : public CWinApp
{
public:
    CRedirectApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRedirectApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CRedirectApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDIRECT_H__FD1BCAF4_7733_4B99_A5BA_B3892CB5170B__INCLUDED_)
