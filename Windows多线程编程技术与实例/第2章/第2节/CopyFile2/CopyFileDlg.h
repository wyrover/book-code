// CopyFileDlg.h : header file
//

#if !defined(AFX_COPYFILEDLG_H__9E539AFD_DF98_4779_9996_38E70D38FBFF__INCLUDED_)
#define AFX_COPYFILEDLG_H__9E539AFD_DF98_4779_9996_38E70D38FBFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCopyFileDlg dialog

class CCopyFileDlg : public CDialog
{
// Construction
public:
	CString m_strFileName;
	CCopyFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyFileDlg)
	enum { IDD = IDD_COPYFILE_DIALOG };
	CProgressCtrl	m_progressCtrl;
	CString	m_strSourse;
	CString	m_strDest;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCopyFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSelfile();
	afx_msg void OnButtonDest();
	afx_msg void OnButtonCopy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYFILEDLG_H__9E539AFD_DF98_4779_9996_38E70D38FBFF__INCLUDED_)
