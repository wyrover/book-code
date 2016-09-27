// PersistClient.h : main header file for the PERSISTCLIENT application
//

#if !defined(AFX_PERSISTCLIENT_H__5D49D005_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_)
#define AFX_PERSISTCLIENT_H__5D49D005_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPersistClientApp:
// See PersistClient.cpp for the implementation of this class
//

class CPersistClientApp : public CWinApp
{
public:
    CPersistClientApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPersistClientApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CPersistClientApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSISTCLIENT_H__5D49D005_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_)
