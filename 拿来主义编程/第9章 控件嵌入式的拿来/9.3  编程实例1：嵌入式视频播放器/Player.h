// Player.h : main header file for the PLAYER application
//

#if !defined(AFX_PLAYER_H__98F06CFE_F67B_4AF3_9859_A775C45F5943__INCLUDED_)
#define AFX_PLAYER_H__98F06CFE_F67B_4AF3_9859_A775C45F5943__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlayerApp:
// See Player.cpp for the implementation of this class
//

class CPlayerApp : public CWinApp
{
public:
    CPlayerApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPlayerApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CPlayerApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYER_H__98F06CFE_F67B_4AF3_9859_A775C45F5943__INCLUDED_)
