
// ServerDlg.h : header file
//

#pragma once
#include "Inc.h"
#include <Mswsock.h>


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
	BOOL WinSockInit();
	void ShowMsg(CString strMsg);
	static UINT __cdecl ThreadProc(LPVOID pParam);
	BOOL AddSocketOlpToArray(CSocketOverlap *hSocketOlp);
	void DelSocketOlpFromArray(CSocketOverlap *hSocketOlp);
	void DelEventFromArray(WSAEVENT hEvent);
	SOCKET FindSocketFromEvent(WSAEVENT hEvent);
	CSocketOverlap *AllocSocketOverlap(SOCKET mSocket);
	void FreeSocketOverlap(CSocketOverlap *pItem);
	CSocketOverlap *GetOverlapFromEvent(WSAEVENT hEvent);
	BOOL PostAccept(SOCKET sListenSock);
	BOOL PostAccept(CSocketOverlap *sListenOs);
	BOOL PostRecv(CSocketOverlap *pItem);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnSend();
};
