// VcExcel.h : main header file for the VCEXCEL application
//

#if !defined(AFX_VCEXCEL_H__1187BB30_E493_4216_A0B4_FF018E4544FD__INCLUDED_)
#define AFX_VCEXCEL_H__1187BB30_E493_4216_A0B4_FF018E4544FD__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVcExcelApp:
// See VcExcel.cpp for the implementation of this class
//

class CVcExcelApp : public CWinApp
{
public:
    CVcExcelApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVcExcelApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CVcExcelApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCEXCEL_H__1187BB30_E493_4216_A0B4_FF018E4544FD__INCLUDED_)
