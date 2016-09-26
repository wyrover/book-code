// ATL_OLEDB_Consumer.h : main header file for the ATL_OLEDB_CONSUMER application
//

#if !defined(AFX_ATL_OLEDB_CONSUMER_H__E4658765_97AE_11D3_851B_00400530CFB7__INCLUDED_)
#define AFX_ATL_OLEDB_CONSUMER_H__E4658765_97AE_11D3_851B_00400530CFB7__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ATL_OLEDB_Consumer_i.h"

/////////////////////////////////////////////////////////////////////////////
// CATL_OLEDB_ConsumerApp:
// See ATL_OLEDB_Consumer.cpp for the implementation of this class
//

class CATL_OLEDB_ConsumerApp : public CWinApp
{
public:
    CATL_OLEDB_ConsumerApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CATL_OLEDB_ConsumerApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CATL_OLEDB_ConsumerApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL m_bATLInited;
private:
    BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATL_OLEDB_CONSUMER_H__E4658765_97AE_11D3_851B_00400530CFB7__INCLUDED_)
