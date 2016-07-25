// namedPipeDlg.h : header file
//

#if !defined(AFX_NAMEDPIPEDLG_H__B322B8F1_D1FE_4B2E_A517_10AAAD6840EB__INCLUDED_)
#define AFX_NAMEDPIPEDLG_H__B322B8F1_D1FE_4B2E_A517_10AAAD6840EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NamedPipe1.h"
/////////////////////////////////////////////////////////////////////////////
// CNamedPipeDlg dialog

class CNamedPipeDlg : public CDialog
{
// Construction
public:
	CNamedPipeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNamedPipeDlg)
	enum { IDD = IDD_NAMEDPIPE_DIALOG };
	CEdit	m_ctrlServerName;
	CButton	m_ctrlSend;
	CButton	m_ctrlConnect;
	CString	m_szPipeName;
	CString	m_sz2Send;
	CString	m_szServerName;
	CString	m_szRX;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedPipeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


public:

	static void WINAPI funcCallBack(string buf);

	afx_msg LRESULT CallUpdateData(WPARAM, LPARAM);
// Implementation
protected:
	HICON m_hIcon;

	bool m_bIsServer;

	CNamedPipe m_np;

	bool m_bReady;


	// Generated message map functions
	//{{AFX_MSG(CNamedPipeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBConnect();
	afx_msg void OnBSend();
	afx_msg void OnRbClient();
	afx_msg void OnRbServer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDPIPEDLG_H__B322B8F1_D1FE_4B2E_A517_10AAAD6840EB__INCLUDED_)
