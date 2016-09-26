// ATL_OLEDB_ConsumerDlg.h : header file
//

#if !defined(AFX_ATL_OLEDB_CONSUMERDLG_H__E4658767_97AE_11D3_851B_00400530CFB7__INCLUDED_)
#define AFX_ATL_OLEDB_CONSUMERDLG_H__E4658767_97AE_11D3_851B_00400530CFB7__INCLUDED_

#include "Temperatures.H"   // Added by ClassView
#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CATL_OLEDB_ConsumerDlg dialog

class CATL_OLEDB_ConsumerDlg : public CDialog
{
// Construction
public:
    CATL_OLEDB_ConsumerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CATL_OLEDB_ConsumerDlg)
    enum { IDD = IDD_ATL_OLEDB_CONSUMER_DIALOG };
    CListCtrl   m_temperaturesList;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CATL_OLEDB_ConsumerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CTemperatures m_temperatures;
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CATL_OLEDB_ConsumerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATL_OLEDB_CONSUMERDLG_H__E4658767_97AE_11D3_851B_00400530CFB7__INCLUDED_)
