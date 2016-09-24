// ReadPDFDlg.h : header file
//

#if !defined(AFX_READPDFDLG_H__DD3ED631_B53E_4051_996C_307699DE564A__INCLUDED_)
#define AFX_READPDFDLG_H__DD3ED631_B53E_4051_996C_307699DE564A__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CReadPDFDlg dialog

#include "acroaxdocshim.h"

class CReadPDFDlg : public CDialog
{
// Construction
public:
    float m_fZoom;
    CAcroAXDocShim m_ctlPdf;
    CReadPDFDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CReadPDFDlg)
    enum { IDD = IDD_READPDF_DIALOG };
    CString m_strFilename;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReadPDFDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CReadPDFDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONBrowser();
    afx_msg void OnBUTTONFirstPage();
    afx_msg void OnBUTTONPreviousPage();
    afx_msg void OnBUTTONNextPage();
    afx_msg void OnBUTTONLastPage();
    afx_msg void OnBUTTONZoomOut();
    afx_msg void OnBUTTONZoomIn();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READPDFDLG_H__DD3ED631_B53E_4051_996C_307699DE564A__INCLUDED_)
