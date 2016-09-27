// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__CDB6A4E2_819B_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_DLGPROXY_H__CDB6A4E2_819B_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CMFCSampleExeDlg;

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeDlgAutoProxy command target

class CMFCSampleExeDlgAutoProxy : public CCmdTarget
{
    DECLARE_DYNCREATE(CMFCSampleExeDlgAutoProxy)

    CMFCSampleExeDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
    CMFCSampleExeDlg* m_pDialog;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMFCSampleExeDlgAutoProxy)
public:
    virtual void OnFinalRelease();
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CMFCSampleExeDlgAutoProxy();

    // Generated message map functions
    //{{AFX_MSG(CMFCSampleExeDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CMFCSampleExeDlgAutoProxy)

    // Generated OLE dispatch map functions
    //{{AFX_DISPATCH(CMFCSampleExeDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_DISPATCH
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__CDB6A4E2_819B_11D4_A23D_02608CACEDBB__INCLUDED_)
