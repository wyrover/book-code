// SendProcessDlg.h : header file
//

#if !defined(AFX_SENDPROCESSDLG_H__E46D5CDE_2665_440B_9A70_57F2A8F076C0__INCLUDED_)
#define AFX_SENDPROCESSDLG_H__E46D5CDE_2665_440B_9A70_57F2A8F076C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSendProcessDlg dialog

class CSendProcessDlg : public CDialog
{
// Construction
public:
	CSendProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSendProcessDlg)
	enum { IDD = IDD_SENDPROCESS_DIALOG };
	CString	m_strText1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COleDataSource* saveText();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSendProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDPROCESSDLG_H__E46D5CDE_2665_440B_9A70_57F2A8F076C0__INCLUDED_)
