// Odbc4TxtDlg.h : header file
//

#if !defined(AFX_ODBC4TXTDLG_H__6FABBB1E_64D6_4D12_8912_8788DFDFF3F9__INCLUDED_)
#define AFX_ODBC4TXTDLG_H__6FABBB1E_64D6_4D12_8912_8788DFDFF3F9__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COdbc4TxtDlg dialog

//使用ODBC API之前要用到的头文件和LIB库
//#include "sql.h"              // This is the the main include for ODBC Core functions.
//#include "sqlext.h"         // This is the include for applications using the Microsoft SQL Extensions
//#include "sqltypes.h"    // This file defines the types used in ODBC
//#include "sqlucode.h"   // This is the the unicode include for ODBC Core functions
//#include "odbcss.h"      // This is the application include file for the SQL Server driver specific defines.

class COdbc4TxtDlg : public CDialog
{
// Construction
public:
    COdbc4TxtDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(COdbc4TxtDlg)
    enum { IDD = IDD_ODBC4TXT_DIALOG };
    CListCtrl   m_ctlListMember;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COdbc4TxtDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(COdbc4TxtDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBUTTONRead();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBC4TXTDLG_H__6FABBB1E_64D6_4D12_8912_8788DFDFF3F9__INCLUDED_)
