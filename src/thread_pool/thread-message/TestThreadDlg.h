
// TestThreadDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTestThreadDlg dialog
class CTestThreadDlg : public CDialog
{
// Construction
public:
    CTestThreadDlg(CWnd* pParent = NULL);   // standard constructor
    DWORD m_idThread;

// Dialog Data
    enum { IDD = IDD_TESTTHREAD_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonCreatethread();
    afx_msg void OnBnClickedButtonThreadHello();
    afx_msg void OnBnClickedButtonQuitthread();
    CStatic m_stCaption;
    LRESULT OnThreadQuitFunction(WPARAM wParam, LPARAM lParam);
};
DWORD threadProc(LPVOID lParam);