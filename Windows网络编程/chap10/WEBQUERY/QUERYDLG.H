// QueryDlg.h : header file
//

#if !defined(AFX_QUERYDLG_H__E3FD7D30_3D47_11D4_8C39_0000E839EC5E__INCLUDED_)
#define AFX_QUERYDLG_H__E3FD7D30_3D47_11D4_8C39_0000E839EC5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQueryDlg dialog

class CQueryDlg : public CDialog
{
// Construction
public:
	CQueryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQueryDlg)
	enum { IDD = IDD_WEBQUERY_DIALOG };
	CEdit	m_EditPwd;
	CEdit	m_EditName;
	CStatic	m_StaPwd;
	CStatic	m_StaName;
	CListBox	m_ListFile;
	CString	m_strHost;
	int		m_nType;
	CString	m_strName;
	CString	m_strPwd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQueryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuery();
	afx_msg void OnRadioType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void TryHttpSite();
	void TryFtpSite();
	void TryGopherSite();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUERYDLG_H__E3FD7D30_3D47_11D4_8C39_0000E839EC5E__INCLUDED_)
