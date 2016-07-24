// 62_s1_clientDlg.h : header file
//

#if !defined(AFX_62_S1_CLIENTDLG_H__EF4702A8_ACDB_11D3_BDE9_FC695D1D960B__INCLUDED_)
#define AFX_62_S1_CLIENTDLG_H__EF4702A8_ACDB_11D3_BDE9_FC695D1D960B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy62_s1_clientDlg dialog

class CMy62_s1_clientDlg : public CDialog
{
// Construction
public:
	CMy62_s1_clientDlg(CWnd* pParent = NULL);	// standard constructor
	CSocket m_sockSend;
// Dialog Data
	//{{AFX_DATA(CMy62_s1_clientDlg)
	enum { IDD = IDD_MY62_S1_CLIENT_DIALOG };
	CString	m_szSend;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy62_s1_clientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy62_s1_clientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_62_S1_CLIENTDLG_H__EF4702A8_ACDB_11D3_BDE9_FC695D1D960B__INCLUDED_)
