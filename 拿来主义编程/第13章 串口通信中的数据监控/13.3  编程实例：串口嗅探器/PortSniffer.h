// PortSniffer.h : main header file for the PORTSNIFFER application
//

#if !defined(AFX_PORTSNIFFER_H__D375D3C1_844E_477D_B959_2C8A2B06AF46__INCLUDED_)
#define AFX_PORTSNIFFER_H__D375D3C1_844E_477D_B959_2C8A2B06AF46__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPortSnifferApp:
// See PortSniffer.cpp for the implementation of this class
//

class CPortSnifferApp : public CWinApp
{
public:
    CPortSnifferApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPortSnifferApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CPortSnifferApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTSNIFFER_H__D375D3C1_844E_477D_B959_2C8A2B06AF46__INCLUDED_)
