// HookDll.h : main header file for the HOOKDLL DLL
//

#if !defined(AFX_HOOKDLL_H__EADF19BB_AD55_4592_B5B8_67FB5B64FBD7__INCLUDED_)
#define AFX_HOOKDLL_H__EADF19BB_AD55_4592_B5B8_67FB5B64FBD7__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHookDllApp
// See HookDll.cpp for the implementation of this class
//

class CHookDllApp : public CWinApp
{
public:
    void StopHook();
    void StartHook(CListBox *pList);
    CHookDllApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHookDllApp)
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CHookDllApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKDLL_H__EADF19BB_AD55_4592_B5B8_67FB5B64FBD7__INCLUDED_)
