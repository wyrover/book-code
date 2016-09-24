#if !defined(AFX_DIALOGSELECTIP_H__A18D449B_9451_48A5_80DB_6B476C951757__INCLUDED_)
#define AFX_DIALOGSELECTIP_H__A18D449B_9451_48A5_80DB_6B476C951757__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000
// DialogSelectIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectIP dialog

class CDialogSelectIP : public CDialog
{
// Construction
public:
    int m_iCurSel;
    int m_iTotalIP;
    CString m_strIP[MAXIP];
    CDialogSelectIP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDialogSelectIP)
    enum { IDD = IDD_DIALOG_IP };
    CListBox    m_ctlListIP;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDialogSelectIP)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDialogSelectIP)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSELECTIP_H__A18D449B_9451_48A5_80DB_6B476C951757__INCLUDED_)
