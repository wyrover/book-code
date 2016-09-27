// StructStore.h : main header file for the STRUCTSTORE application
//

#if !defined(AFX_STRUCTSTORE_H__BD1E8803_767E_11D4_8BD4_680DDD000000__INCLUDED_)
#define AFX_STRUCTSTORE_H__BD1E8803_767E_11D4_8BD4_680DDD000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CStructStoreApp:
// See StructStore.cpp for the implementation of this class
//

class CStructStoreApp : public CWinApp
{
public:
    CStructStoreApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStructStoreApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CStructStoreApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTSTORE_H__BD1E8803_767E_11D4_8BD4_680DDD000000__INCLUDED_)
