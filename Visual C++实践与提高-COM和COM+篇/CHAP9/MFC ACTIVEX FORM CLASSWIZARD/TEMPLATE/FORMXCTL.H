#if !defined(AFX_FORMXCTL_H__AFDC1594_F993_11D1_96ED_F05F03C10000__INCLUDED_)
#define AFX_FORMXCTL_H__AFDC1594_F993_11D1_96ED_F05F03C10000__INCLUDED_

#if _MSC_VER >= 1000
    #pragma once
#endif // _MSC_VER >= 1000

// FormXCtl.h : Declaration of the CFormXCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl : See FormXCtl.cpp for implementation.

class CFormXCtrl : public CFormControl
{
    DECLARE_DYNCREATE(CFormXCtrl)

// Constructor
public:
    CFormXCtrl();

    //{{AFX_DATA(CTestXView)
    enum { IDD = IDD_FORMX };
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormXCtrl)
public:
    virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
    virtual void DoPropExchange(CPropExchange* pPX);
    virtual void OnResetState();
    //}}AFX_VIRTUAL

// Implementation
protected:
    ~CFormXCtrl();

    DECLARE_OLECREATE_EX(CFormXCtrl)    // Class factory and guid
    DECLARE_OLETYPELIB(CFormXCtrl)      // GetTypeInfo
    DECLARE_PROPPAGEIDS(CFormXCtrl)     // Property page IDs
    DECLARE_OLECTLTYPE(CFormXCtrl)      // Type name and misc status

// Message maps
    //{{AFX_MSG(CFormXCtrl)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

// Dispatch maps
    //{{AFX_DISPATCH(CFormXCtrl)
    // NOTE - ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DISPATCH
    DECLARE_DISPATCH_MAP()

// Event maps
    //{{AFX_EVENT(CFormXCtrl)
    // NOTE - ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_EVENT
    DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
    enum {
        //{{AFX_DISP_ID(CFormXCtrl)
        // NOTE: ClassWizard will add and remove enumeration elements here.
        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_DISP_ID
    };
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMXCTL_H__AFDC1594_F993_11D1_96ED_F05F03C10000__INCLUDED)
