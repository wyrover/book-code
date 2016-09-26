// PersistText.h : main header file for the PERSISTTEXT DLL
//

#if !defined(AFX_PERSISTTEXT_H__5310B2FB_76A5_11D4_8BDA_0EB916000000__INCLUDED_)
#define AFX_PERSISTTEXT_H__5310B2FB_76A5_11D4_8BDA_0EB916000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPersistTextApp
// See PersistText.cpp for the implementation of this class
//

class CPersistTextApp : public CWinApp
{
public:
    CPersistTextApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPersistTextApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CPersistTextApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSISTTEXT_H__5310B2FB_76A5_11D4_8BDA_0EB916000000__INCLUDED_)
