// HookProgram.h : main header file for the HOOKPROGRAM application
//

#if !defined(AFX_HOOKPROGRAM_H__660218F7_676C_48DE_A2DF_ED51FFDA899E__INCLUDED_)
#define AFX_HOOKPROGRAM_H__660218F7_676C_48DE_A2DF_ED51FFDA899E__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHookProgramApp:
// See HookProgram.cpp for the implementation of this class
//

class CHookProgramApp : public CWinApp
{
public:
    CHookProgramApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHookProgramApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CHookProgramApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKPROGRAM_H__660218F7_676C_48DE_A2DF_ED51FFDA899E__INCLUDED_)
