
// ServerDlg.h : header file
//

#pragma once


// CServerDlg dialog
class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
	UINT m_ClientNums;
	SOCKET m_SockListen;
	SOCKET m_SockClient;
	BOOL WinSockInit();
	void ShowMsg(CString strMsg);
	static UINT __cdecl ThreadProc(LPVOID pParam);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnSend();
};
