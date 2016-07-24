// PipeTestDlg.h : header file
//

#if !defined(AFX_PIPETESTDLG_H__CD3D8C9C_D258_11D3_8DC4_0010A4FA75E3__INCLUDED_)
#define AFX_PIPETESTDLG_H__CD3D8C9C_D258_11D3_8DC4_0010A4FA75E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NamedPipe.h"

/////////////////////////////////////////////////////////////////////////////
// CPipeTestDlg dialog

class CPipeTestDlg : public CDialog
{
// Construction
public:
	CPipeTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPipeTestDlg)
	enum { IDD = IDD_PIPETEST_DIALOG };
	CButton	m_ctrlConnect;
	CButton	m_ctrlSend;
	CEdit	m_ctrlServerName;
	CString	m_szPipeName;
	CString	m_szRX;
	CString	m_sz2Send;
	CString	m_szServerName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPipeTestDlg)
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

	bool m_bMagic;

	// Generated message map functions
	//{{AFX_MSG(CPipeTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBConnect();
	afx_msg void OnRbClient();
	afx_msg void OnRbServer();
	afx_msg void OnBSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPETESTDLG_H__CD3D8C9C_D258_11D3_8DC4_0010A4FA75E3__INCLUDED_)
