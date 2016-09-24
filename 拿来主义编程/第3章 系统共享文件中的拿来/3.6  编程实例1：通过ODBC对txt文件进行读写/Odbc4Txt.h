// Odbc4Txt.h : main header file for the ODBC4TXT application
//

#if !defined(AFX_ODBC4TXT_H__8E247D1C_B148_4FA9_A92E_70F09E5E6943__INCLUDED_)
#define AFX_ODBC4TXT_H__8E247D1C_B148_4FA9_A92E_70F09E5E6943__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COdbc4TxtApp:
// See Odbc4Txt.cpp for the implementation of this class
//

class COdbc4TxtApp : public CWinApp
{
public:
    COdbc4TxtApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COdbc4TxtApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(COdbc4TxtApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBC4TXT_H__8E247D1C_B148_4FA9_A92E_70F09E5E6943__INCLUDED_)
