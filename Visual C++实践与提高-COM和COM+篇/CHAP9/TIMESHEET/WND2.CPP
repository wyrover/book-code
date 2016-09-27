/*****************************************************************************
  File: Wnd2.cpp

  Purpose:
     Since the COccManager class is a friend class of CWnd it has access to
     protected member variables and methods of CWnd. My derived version of
     COccManager also needs access to these members and so I had to derive
     this class from CWnd and create accessor functions to the protected data.

  Functions:
     GetControlSite - returns the protected member CWnd::m_pCtrlSite
     GetCtrlCont    - returns the protected member CWnd::m_pCtrlCont

  Development Team: ShawnK

  Written by Microsoft Product Support Services, Languages Developer Support
  Copyright (c) 1993 Microsoft Corporation. All rights reserved.
\****************************************************************************/


#include "stdafx.h"
#include "Wnd2.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWnd2

CWnd2::CWnd2()
{
}

CWnd2::~CWnd2()
{
}


BEGIN_MESSAGE_MAP(CWnd2, CWnd)
    //{{AFX_MSG_MAP(CWnd2)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWnd2 message handlers

COleControlSite* CWnd2::GetControlSite()
{
    return m_pCtrlSite;
}

COleControlContainer* CWnd2::GetCtrlCont()
{
    return m_pCtrlCont;
}
