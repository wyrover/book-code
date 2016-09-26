// ADOSampClient.h : main header file for the ADOSAMPCLIENT application
//

#if !defined(AFX_ADOSAMPCLIENT_H__504685A7_825B_11D4_8C0E_446260000000__INCLUDED_)
#define AFX_ADOSAMPCLIENT_H__504685A7_825B_11D4_8C0E_446260000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CADOSampClientApp:
// See ADOSampClient.cpp for the implementation of this class
//

class CADOSampClientApp : public CWinApp
{
public:
    CADOSampClientApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CADOSampClientApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CADOSampClientApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADOSAMPCLIENT_H__504685A7_825B_11D4_8C0E_446260000000__INCLUDED_)
