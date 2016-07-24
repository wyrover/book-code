// TcpSpyDlg.h : header file
//
#include <Iprtrmib.h>

#if !defined(AFX_TCPSPYDLG_H__ECCF8FA7_7FA7_11D6_B72C_0050BAED0C65__INCLUDED_)
#define AFX_TCPSPYDLG_H__ECCF8FA7_7FA7_11D6_B72C_0050BAED0C65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTcpSpyDlg dialog
typedef DWORD (WINAPI *GetTcpTableFun)(PMIB_TCPTABLE,PDWORD,BOOL);
typedef DWORD (WINAPI *SetTcpEntryFun) (PMIB_TCPROW );
typedef DWORD (WINAPI *GetUdpTableFun)(PMIB_UDPTABLE,PDWORD,BOOL);


class CTcpSpyDlg : public CDialog
{
// Construction
public:
	CTcpSpyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTcpSpyDlg)
	enum { IDD = IDD_TCPSPY_DIALOG };
	CEdit	m_txtTimer;
	CButton	m_chkAuto;
	CListCtrl	m_tcp;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpSpyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
private:
	PMIB_TCPTABLE mibtcp; //TCP
	PMIB_UDPTABLE mibUdp; //UDP
	BYTE	pTcpBuf[100*20+4];
	char    *DisConIPAddr[10];
	int     iDisCon;

	HINSTANCE hInst;
	GetTcpTableFun pGetTcp;
	GetUdpTableFun pGetUdp;
	SetTcpEntryFun pEtyTcp;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTcpSpyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRefresh();
	afx_msg void OnExit();
	afx_msg void OnDisconnect();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListTcp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAuto();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPSPYDLG_H__ECCF8FA7_7FA7_11D6_B72C_0050BAED0C65__INCLUDED_)
