// Ado4ExcelDlg.h : header file
//

#if !defined(AFX_ADO4EXCELDLG_H__6B9631C3_F2D1_4D6F_A40B_70ACD5BF2C35__INCLUDED_)
#define AFX_ADO4EXCELDLG_H__6B9631C3_F2D1_4D6F_A40B_70ACD5BF2C35__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAdo4ExcelDlg dialog

class CAdo4ExcelDlg : public CDialog
{
// Construction
public:
    void ReadExcel(CString strFilename);

    //Êý¾Ý¿â¡£
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;
    CString VarToStr(_variant_t var);
    int VarToInt(_variant_t var);
    UINT VarToUInt(_variant_t var);
    UINT atou(CString str);

    CAdo4ExcelDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CAdo4ExcelDlg)
    enum { IDD = IDD_ADO4EXCEL_DIALOG };
    CListCtrl   m_ctlListMember;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAdo4ExcelDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CAdo4ExcelDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONReadExcel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADO4EXCELDLG_H__6B9631C3_F2D1_4D6F_A40B_70ACD5BF2C35__INCLUDED_)
