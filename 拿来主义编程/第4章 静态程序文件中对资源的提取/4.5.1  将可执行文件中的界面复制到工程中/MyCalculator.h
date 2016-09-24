// MyCalculator.h : main header file for the MYCALCULATOR application
//

#if !defined(AFX_MYCALCULATOR_H__1BC44B6A_5B0E_4DF4_B7A9_1921349E3D02__INCLUDED_)
#define AFX_MYCALCULATOR_H__1BC44B6A_5B0E_4DF4_B7A9_1921349E3D02__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyCalculatorApp:
// See MyCalculator.cpp for the implementation of this class
//

class CMyCalculatorApp : public CWinApp
{
public:
    CMyCalculatorApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyCalculatorApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CMyCalculatorApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCALCULATOR_H__1BC44B6A_5B0E_4DF4_B7A9_1921349E3D02__INCLUDED_)
