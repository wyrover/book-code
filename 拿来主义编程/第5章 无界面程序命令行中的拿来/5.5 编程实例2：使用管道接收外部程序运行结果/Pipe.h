// Pipe.h : main header file for the PIPE application
//

#if !defined(AFX_PIPE_H__0EF0AD66_95F4_4D8D_B25F_9F328B36D3E5__INCLUDED_)
#define AFX_PIPE_H__0EF0AD66_95F4_4D8D_B25F_9F328B36D3E5__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPipeApp:
// See Pipe.cpp for the implementation of this class
//

class CPipeApp : public CWinApp
{
public:
    CPipeApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPipeApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CPipeApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPE_H__0EF0AD66_95F4_4D8D_B25F_9F328B36D3E5__INCLUDED_)
