// NewPing.h : main header file for the NEWPING application
//

#if !defined(AFX_NEWPING_H__B82CD1F2_FB58_430A_97F5_084F3BB89C4A__INCLUDED_)
#define AFX_NEWPING_H__B82CD1F2_FB58_430A_97F5_084F3BB89C4A__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNewPingApp:
// See NewPing.cpp for the implementation of this class
//

class CNewPingApp : public CWinApp
{
public:
    CNewPingApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewPingApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CNewPingApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPING_H__B82CD1F2_FB58_430A_97F5_084F3BB89C4A__INCLUDED_)
