#if !defined(AFX_FORMXPPG_H__AFDC1596_F993_11D1_96ED_F05F03C10000__INCLUDED_)
#define AFX_FORMXPPG_H__AFDC1596_F993_11D1_96ED_F05F03C10000__INCLUDED_

#if _MSC_VER >= 1000
    #pragma once
#endif // _MSC_VER >= 1000

// FormXPpg.h : Declaration of the CFormXPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CFormXPropPage : See FormXPpg.cpp.cpp for implementation.

class CFormXPropPage : public COlePropertyPage
{
    DECLARE_DYNCREATE(CFormXPropPage)
    DECLARE_OLECREATE_EX(CFormXPropPage)

// Constructor
public:
    CFormXPropPage();

// Dialog Data
    //{{AFX_DATA(CFormXPropPage)
    enum { IDD = IDD_PROPPAGE_FORMX };
    // NOTE - ClassWizard will add data members here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
    //{{AFX_MSG(CFormXPropPage)
    // NOTE - ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMXPPG_H__AFDC1596_F993_11D1_96ED_F05F03C10000__INCLUDED)
