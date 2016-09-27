#if !defined(AFX_USERDIALOG_H__AFDC1598_F993_11D1_96ED_F05F03C10000__INCLUDED_)
#define AFX_USERDIALOG_H__AFDC1598_F993_11D1_96ED_F05F03C10000__INCLUDED_

#if _MSC_VER >= 1000
    #pragma once
#endif // _MSC_VER >= 1000
// UserDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserDialog dialog

class CUserDialog : public CDialog
{
// Construction
public:
    CUserDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CUserDialog)
    enum { IDD = IDD_USER_DIALOG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUserDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CUserDialog)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERDIALOG_H__AFDC1598_F993_11D1_96ED_F05F03C10000__INCLUDED_)
