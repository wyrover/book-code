// Ado4Excel.h : main header file for the ADO4EXCEL application
//

#if !defined(AFX_ADO4EXCEL_H__B66E7F7A_A907_4E26_A8D4_2707DAA24AC3__INCLUDED_)
#define AFX_ADO4EXCEL_H__B66E7F7A_A907_4E26_A8D4_2707DAA24AC3__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAdo4ExcelApp:
// See Ado4Excel.cpp for the implementation of this class
//

class CAdo4ExcelApp : public CWinApp
{
public:
    CAdo4ExcelApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAdo4ExcelApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CAdo4ExcelApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADO4EXCEL_H__B66E7F7A_A907_4E26_A8D4_2707DAA24AC3__INCLUDED_)
