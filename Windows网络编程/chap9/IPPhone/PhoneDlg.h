// PhoneDlg.h : header file
//

#if !defined(AFX_PHONEDLG_H__8F8DC627_0576_11D4_8D3A_000021E6766A__INCLUDED_)
#define AFX_PHONEDLG_H__8F8DC627_0576_11D4_8D3A_000021E6766A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "soundin.h"
#include "soundout.h"
#include "winsock.h"
#include "ipaddress.h"
#include "compression.h"

#define  WINSOCK_VERSION  0x0101	 //Socket version over 1.1
#define	 BLOCK			  1024*4
#define  MAX		      5

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg dialog

class CPhoneDlg : public CDialog
{
// Construction
public:
	BOOL Connect(BOOL b);
	int GetLocalIP(unsigned long* ip,int len);
	void ReportWinsockErr(LPSTR lpszErrorMsg);
	CSoundIn  sndIn;
	CSoundOut sndOut;

	SOCKET m_sAccept;
	SOCKADDR_IN addrAccept;
	SOCKET m_sRecv;
	SOCKADDR_IN addrRecv;
	SOCKET m_sSend;
	SOCKADDR_IN addrSend;
	WSADATA   wsaData;
	
	long m_nBlockSize;
	CToolBar m_wndToolBar;
	CPhoneDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPhoneDlg)
	enum { IDD = IDD_PHONE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTest();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnConnect();
	afx_msg void OnOpenserver();
	afx_msg void OnLocalIp();
	afx_msg void OnDisconnect();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnReadReady(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CCompression cmpReceive;
	CCompression cmpSend;
	BOOL m_bClose;
	UINT nID;
	SOCKADDR_IN m_LocalAddr;
	SOCKADDR_IN m_RemoteAddr;
	unsigned long m_RemoteIP;
	BOOL m_bWaiting;
	BOOL m_bPassive;
	char chRecv[BLOCK];
	unsigned char* chSndOut;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONEDLG_H__8F8DC627_0576_11D4_8D3A_000021E6766A__INCLUDED_)
