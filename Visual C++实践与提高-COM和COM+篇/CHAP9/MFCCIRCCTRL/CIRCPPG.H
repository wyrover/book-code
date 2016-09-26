#if !defined(AFX_CIRCPPG_H__7F874C21_7F97_4DF4_AAC3_8F1E2A70E707__INCLUDED_)
#define AFX_CIRCPPG_H__7F874C21_7F97_4DF4_AAC3_8F1E2A70E707__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

// CircPpg.h : Declaration of the CCircPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCircPropPage : See CircPpg.cpp.cpp for implementation.

class CCircPropPage : public COlePropertyPage
{
    DECLARE_DYNCREATE(CCircPropPage)
    DECLARE_OLECREATE_EX(CCircPropPage)

// Constructor
public:
    CCircPropPage();

// Dialog Data
    //{{AFX_DATA(CCircPropPage)
    enum { IDD = IDD_PROPPAGE_CIRC };
    CString m_caption;
    int     m_circleOffset;
    BOOL    m_circleShape;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
    //{{AFX_MSG(CCircPropPage)
    // NOTE - ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIRCPPG_H__7F874C21_7F97_4DF4_AAC3_8F1E2A70E707__INCLUDED)
