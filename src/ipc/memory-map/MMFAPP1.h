// MMFAPP1.h : main header file for the MMFAPP1 application
//

#if !defined(AFX_MMFAPP1_H__F7DF4E9D_CBE1_44BB_B642_0F6ACAC3DC3F__INCLUDED_)
#define AFX_MMFAPP1_H__F7DF4E9D_CBE1_44BB_B642_0F6ACAC3DC3F__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMMFAPP1App:
// See MMFAPP1.cpp for the implementation of this class
//

class CMMFAPP1App : public CWinApp
{
public:
    CMMFAPP1App();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMMFAPP1App)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CMMFAPP1App)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMFAPP1_H__F7DF4E9D_CBE1_44BB_B642_0F6ACAC3DC3F__INCLUDED_)
