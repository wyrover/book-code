// Scribble.h : main header file for the SCRIBBLE application
//

#if !defined(AFX_SCRIBBLE_H__7D1606FC_5202_424F_9A25_29F37B280146__INCLUDED_)
#define AFX_SCRIBBLE_H__7D1606FC_5202_424F_9A25_29F37B280146__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CScribbleApp:
// See Scribble.cpp for the implementation of this class
//

class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScribbleApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    COleTemplateServer m_server;
    // Server object for document creation
    //{{AFX_MSG(CScribbleApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIBBLE_H__7D1606FC_5202_424F_9A25_29F37B280146__INCLUDED_)
