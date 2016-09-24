// ProcessMonitor.h : main header file for the PROCESSMONITOR application
//

#if !defined(AFX_PROCESSMONITOR_H__87209F2D_E29D_4997_9644_F4DE0BAA9674__INCLUDED_)
#define AFX_PROCESSMONITOR_H__87209F2D_E29D_4997_9644_F4DE0BAA9674__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CProcessMonitorApp:
// See ProcessMonitor.cpp for the implementation of this class
//

class CProcessMonitorApp : public CWinApp
{
public:
    CProcessMonitorApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProcessMonitorApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CProcessMonitorApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMONITOR_H__87209F2D_E29D_4997_9644_F4DE0BAA9674__INCLUDED_)
