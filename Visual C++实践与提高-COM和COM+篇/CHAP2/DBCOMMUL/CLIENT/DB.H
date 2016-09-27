// DB.h : main header file for the DB application
//

#if !defined(AFX_DB_H__84FCEE7C_5F09_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_DB_H__84FCEE7C_5F09_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDBApp:
// See DB.cpp for the implementation of this class
//

class CDBApp : public CWinApp
{
public:
    CDBApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDBApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual BOOL OnIdle(LONG lCount);
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CDBApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DB_H__84FCEE7C_5F09_11D4_A23D_02608CACEDBB__INCLUDED_)
