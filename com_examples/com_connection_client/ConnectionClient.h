// ConnectionClient.h : main header file for the CONNECTIONCLIENT application
//

#if !defined(AFX_CONNECTIONCLIENT_H__ED409F20_454A_4399_8701_CFC6253B032C__INCLUDED_)
#define AFX_CONNECTIONCLIENT_H__ED409F20_454A_4399_8701_CFC6253B032C__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CConnectionClientApp:
// See ConnectionClient.cpp for the implementation of this class
//

class CConnectionClientApp : public CWinApp
{
public:
    CConnectionClientApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CConnectionClientApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CConnectionClientApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTIONCLIENT_H__ED409F20_454A_4399_8701_CFC6253B032C__INCLUDED_)
