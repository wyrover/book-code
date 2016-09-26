#if !defined(AFX_ACCOUNT_H__657F06D2_80D5_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_ACCOUNT_H__657F06D2_80D5_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000
// Account.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CAccount command target

class CAccount : public CCmdTarget
{
    DECLARE_DYNCREATE(CAccount)

    CAccount();           // protected constructor used by dynamic creation

private:
    long m_balance;

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAccount)
public:
    virtual void OnFinalRelease();
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CAccount();

    // Generated message map functions
    //{{AFX_MSG(CAccount)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CAccount)

    // Generated OLE dispatch map functions
    //{{AFX_DISPATCH(CAccount)
    afx_msg long GetBalance();
    afx_msg void SetBalance(long nNewValue);
    afx_msg BSTR Post(long lAmount);
    //}}AFX_DISPATCH
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNT_H__657F06D2_80D5_11D4_A23D_02608CACEDBB__INCLUDED_)
