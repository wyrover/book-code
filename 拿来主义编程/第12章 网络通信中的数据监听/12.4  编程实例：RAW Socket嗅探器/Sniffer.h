// Sniffer.h : main header file for the SNIFFER application
//

#if !defined(AFX_SNIFFER_H__8DDD14FE_0C7E_4C21_ACFF_9B7E4DBC3C6C__INCLUDED_)
#define AFX_SNIFFER_H__8DDD14FE_0C7E_4C21_ACFF_9B7E4DBC3C6C__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSnifferApp:
// See Sniffer.cpp for the implementation of this class
//

class CSnifferApp : public CWinApp
{
public:
    CSnifferApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSnifferApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CSnifferApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNIFFER_H__8DDD14FE_0C7E_4C21_ACFF_9B7E4DBC3C6C__INCLUDED_)
