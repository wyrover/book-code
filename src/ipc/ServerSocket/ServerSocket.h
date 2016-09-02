// ServerSocket.h : main header file for the SERVERSOCKET application
//

#if !defined(AFX_SERVERSOCKET_H__BFD2712F_6DB3_4E82_9FC7_ABEB33496E09__INCLUDED_)
#define AFX_SERVERSOCKET_H__BFD2712F_6DB3_4E82_9FC7_ABEB33496E09__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp:
// See ServerSocket.cpp for the implementation of this class
//

class CServerSocketApp : public CWinApp
{
public:
    CServerSocketApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CServerSocketApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CServerSocketApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
    int  m_nLinkMode;
    void ParseCommandLineArgs();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__BFD2712F_6DB3_4E82_9FC7_ABEB33496E09__INCLUDED_)
