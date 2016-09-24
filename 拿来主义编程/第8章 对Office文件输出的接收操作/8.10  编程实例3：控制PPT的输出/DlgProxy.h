// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__2F219933_9B19_46E6_BFBA_53FD7E43E5A1__INCLUDED_)
#define AFX_DLGPROXY_H__2F219933_9B19_46E6_BFBA_53FD7E43E5A1__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CVcPPTDlg;

/////////////////////////////////////////////////////////////////////////////
// CVcPPTDlgAutoProxy command target

class CVcPPTDlgAutoProxy : public CCmdTarget
{
    DECLARE_DYNCREATE(CVcPPTDlgAutoProxy)

    CVcPPTDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
    CVcPPTDlg* m_pDialog;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVcPPTDlgAutoProxy)
public:
    virtual void OnFinalRelease();
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CVcPPTDlgAutoProxy();

    // Generated message map functions
    //{{AFX_MSG(CVcPPTDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CVcPPTDlgAutoProxy)

    // Generated OLE dispatch map functions
    //{{AFX_DISPATCH(CVcPPTDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_DISPATCH
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__2F219933_9B19_46E6_BFBA_53FD7E43E5A1__INCLUDED_)
