// FtpDlg.h : header file
//

#if !defined(AFX_FTPDLG_H__5786A2AC_3D7B_11D4_8C39_0000E839EC5E__INCLUDED_)
#define AFX_FTPDLG_H__5786A2AC_3D7B_11D4_8C39_0000E839EC5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFtpDlg dialog

class CFtpDlg : public CDialog
{
// Construction
public:
	CFtpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpDlg)
	enum { IDD = IDD_FTPFILE_DIALOG };
	CButton	m_BtnQuery;
	CButton	m_BtnUpLoad;
	CStatic	m_StaPwd;
	CStatic	m_StaName;
	CStatic	m_StaFtp;
	CEdit	m_EditPwd;
	CEdit	m_EditName;
	CEdit	m_EditFtp;
	CButton	m_BtnDownLoad;
	CListBox	m_ListFile;
	CString	m_strFtpSite;
	CString	m_strName;
	CString	m_strPwd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFtpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuery();
	afx_msg void OnSelchangeListFile();
	afx_msg void OnDownload();
	afx_msg void OnUpload();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL PutFile(CString strSourceName,CString strDestName);
	BOOL GetFile(CString strSourceName,CString strDestName);
	void ListContent();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPDLG_H__5786A2AC_3D7B_11D4_8C39_0000E839EC5E__INCLUDED_)
