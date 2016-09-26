/*****************************************************************************
  File: Wnd2.h

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

#ifndef WND2_H
#define WND2_H

/////////////////////////////////////////////////////////////////////////////
// CWnd2 window

class CWnd2 : public CWnd
{
// Construction
public:
    CWnd2();

// Attributes
public:

// Operations
public:
    COleControlSite* GetControlSite();
    COleControlContainer* GetCtrlCont();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWnd2)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CWnd2();

    // Generated message map functions
protected:
    //{{AFX_MSG(CWnd2)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif //WND2_H