// ReadPDF.h : main header file for the READPDF application
//

#if !defined(AFX_READPDF_H__FAB7D2D7_0475_4C13_9774_777CFE0ADE7A__INCLUDED_)
#define AFX_READPDF_H__FAB7D2D7_0475_4C13_9774_777CFE0ADE7A__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CReadPDFApp:
// See ReadPDF.cpp for the implementation of this class
//

class CReadPDFApp : public CWinApp
{
public:
    CReadPDFApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReadPDFApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CReadPDFApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READPDF_H__FAB7D2D7_0475_4C13_9774_777CFE0ADE7A__INCLUDED_)
