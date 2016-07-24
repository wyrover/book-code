// ProxyServerDlg.h : header file
//

#if !defined(AFX_PROXYSERVERDLG_H__C79845C2_03D1_4997_BE37_C09E4BBDE3A0__INCLUDED_)
#define AFX_PROXYSERVERDLG_H__C79845C2_03D1_4997_BE37_C09E4BBDE3A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ProxyServer1.h"

/////////////////////////////////////////////////////////////////////////////
// CProxyServerDlg dialog

class CProxyServerDlg : public CDialog
{
// Construction
public:
	CProxyServerDlg(CWnd* pParent = NULL);	// standard constructor
    CProxyServer m_proxyServer;
	DWORD	m_dwCount;
	FILE	*m_pFile;
    CString m_strPasswd;
// Dialog Data
	//{{AFX_DATA(CProxyServerDlg)
	enum { IDD = IDD_PROXYSERVER_DIALOG };
	CEdit	m_editUsername;
	CEdit	m_editProxyAddr;
	CEdit	m_editPassword;
	CEdit	m_editInfo;
	BOOL	m_bSecondProxy;
	UINT	m_nPort;
	int		m_nProxyPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProxyServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProxyServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSecondproxy();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOutput(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROXYSERVERDLG_H__C79845C2_03D1_4997_BE37_C09E4BBDE3A0__INCLUDED_)
