
// ClientDlg.h : header file
//

#pragma once


// CClientDlg dialog
class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

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
	SOCKET m_Socket;
	void ShowMsg(CString strMsg);
	BOOL WinSockInit();
	BOOL SOCKET_Select(SOCKET hSocket, int nTimeOut = 100, BOOL bRead = TRUE);
	static UINT __cdecl ThreadProc(LPVOID pParam);
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnSend();
};
