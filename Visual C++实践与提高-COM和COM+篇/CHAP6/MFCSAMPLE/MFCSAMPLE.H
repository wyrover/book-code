// MFCSample.h : main header file for the MFCSAMPLE DLL
//

#if !defined(AFX_MFCSAMPLE_H__657F06C8_80D5_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_MFCSAMPLE_H__657F06C8_80D5_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleApp
// See MFCSample.cpp for the implementation of this class
//

class CMFCSampleApp : public CWinApp
{
public:
    CMFCSampleApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMFCSampleApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CMFCSampleApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCSAMPLE_H__657F06C8_80D5_11D4_A23D_02608CACEDBB__INCLUDED_)
