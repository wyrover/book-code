// VCAutoCom.h : main header file for the VCAUTOCOM application
//

#if !defined(AFX_VCAUTOCOM_H__55DB95C5_8631_11D4_8C1F_0000E83EBA18__INCLUDED_)
#define AFX_VCAUTOCOM_H__55DB95C5_8631_11D4_8C1F_0000E83EBA18__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVCAutoComApp:
// See VCAutoCom.cpp for the implementation of this class
//

class CVCAutoComApp : public CWinApp
{
public:
    CVCAutoComApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVCAutoComApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CVCAutoComApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCAUTOCOM_H__55DB95C5_8631_11D4_8C1F_0000E83EBA18__INCLUDED_)
