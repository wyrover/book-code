// EmbedScreen.h : main header file for the EMBEDSCREEN application
//

#if !defined(AFX_EMBEDSCREEN_H__AA4F4637_BC8F_49C2_89E6_64F01384D1F4__INCLUDED_)
#define AFX_EMBEDSCREEN_H__AA4F4637_BC8F_49C2_89E6_64F01384D1F4__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEmbedScreenApp:
// See EmbedScreen.cpp for the implementation of this class
//

class CEmbedScreenApp : public CWinApp
{
public:
    CEmbedScreenApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEmbedScreenApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CEmbedScreenApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMBEDSCREEN_H__AA4F4637_BC8F_49C2_89E6_64F01384D1F4__INCLUDED_)
