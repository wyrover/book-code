// HttpServerDlg.h : header file
//

#if !defined(AFX_HTTPSERVERDLG_H__745FF0A7_8B0C_11D4_8DCA_0000E839EC5E__INCLUDED_)
#define AFX_HTTPSERVERDLG_H__745FF0A7_8B0C_11D4_8DCA_0000E839EC5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHttpServerDlg dialog

class CHttpServerDlg : public CDialog
{
// Construction
public:
	CHttpServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHttpServerDlg)
	enum { IDD = IDD_HTTPSERVER_DIALOG };
	CStatic	m_StaPath;
	CStatic	m_StaStatus;
	CButton	m_BtnStop;
	CButton	m_BtnStart;
	CListBox	m_ListInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHttpServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStop();
	afx_msg void OnStartServer();
	afx_msg void OnBtnSetpath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPSERVERDLG_H__745FF0A7_8B0C_11D4_8DCA_0000E839EC5E__INCLUDED_)
