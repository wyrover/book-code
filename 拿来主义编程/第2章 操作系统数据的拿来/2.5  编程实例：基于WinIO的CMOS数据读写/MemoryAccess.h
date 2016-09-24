// MemoryAccess.h : main header file for the MEMORYACCESS application
//

#if !defined(AFX_MEMORYACCESS_H__D82AA2E3_68C8_4EF9_9C42_C6FF5CEBCDDE__INCLUDED_)
#define AFX_MEMORYACCESS_H__D82AA2E3_68C8_4EF9_9C42_C6FF5CEBCDDE__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMemoryAccessApp:
// See MemoryAccess.cpp for the implementation of this class
//

class CMemoryAccessApp : public CWinApp
{
public:
    CMemoryAccessApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMemoryAccessApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CMemoryAccessApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYACCESS_H__D82AA2E3_68C8_4EF9_9C42_C6FF5CEBCDDE__INCLUDED_)
