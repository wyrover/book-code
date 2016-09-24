// VcWord.h : main header file for the VCWORD application
//

#if !defined(AFX_VCWORD_H__1C6B2D3E_D1F9_4AB8_A225_D0D7BFCA20C9__INCLUDED_)
#define AFX_VCWORD_H__1C6B2D3E_D1F9_4AB8_A225_D0D7BFCA20C9__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVcWordApp:
// See VcWord.cpp for the implementation of this class
//

class CVcWordApp : public CWinApp
{
public:
    CVcWordApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVcWordApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CVcWordApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCWORD_H__1C6B2D3E_D1F9_4AB8_A225_D0D7BFCA20C9__INCLUDED_)
