// POPCheckerDlg.h : header file
//

#if !defined(AFX_POPCHECKERDLG_H__F712AA7E_7086_11D1_8833_00001C302581__INCLUDED_)
#define AFX_POPCHECKERDLG_H__F712AA7E_7086_11D1_8833_00001C302581__INCLUDED_

#include "TrayIcon.h"	// Added by ClassView
#include "pop.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CPOPCheckerDlg dialog

class CPOPCheckerDlg : public CDialog
{
// Construction
public:
	CPOPCheckerDlg(CWnd* pParent = NULL);	// standard constructor
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);

// Dialog Data
	//{{AFX_DATA(CPOPCheckerDlg)
	enum { IDD = IDD_POPCHECKER_DIALOG };
	int		m_Duration;
	int		m_NoMail;
	CString	m_Password;
	CString	m_POPServer;
	CString	m_UserName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPOPCheckerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPOPCheckerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnConfiguration();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_Error;
	CTrayIcon m_TrayIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPCHECKERDLG_H__F712AA7E_7086_11D1_8833_00001C302581__INCLUDED_)
