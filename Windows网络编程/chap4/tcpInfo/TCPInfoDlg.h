// TCPInfoDlg.h : header file
//

#if !defined(AFX_TCPINFODLG_H__DA8AE307_2BBB_4BCC_AE6F_E2FECA2C2072__INCLUDED_)
#define AFX_TCPINFODLG_H__DA8AE307_2BBB_4BCC_AE6F_E2FECA2C2072__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTCPInfoDlg dialog

class CTCPInfoDlg : public CDialog
{
// Construction
public:
	CTCPInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTCPInfoDlg)
	enum { IDD = IDD_TCPINFO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCPInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTCPInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTCPStats();
	afx_msg void OnUPDStats();
	afx_msg void OnIPStats();
	afx_msg void OnICMPStats();
	afx_msg void OnTCPConns();
	afx_msg void OnUDPConnections();
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPINFODLG_H__DA8AE307_2BBB_4BCC_AE6F_E2FECA2C2072__INCLUDED_)
