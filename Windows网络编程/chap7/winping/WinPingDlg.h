// WinPingDlg.h : header file
//

#if !defined(AFX_WINPINGDLG_H__D12A31A9_1D4C_4301_997D_8E8BCC2E54D5__INCLUDED_)
#define AFX_WINPINGDLG_H__D12A31A9_1D4C_4301_997D_8E8BCC2E54D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWinPingDlg dialog

#include "PingThread.h"
#include "ComboBoxEx.h"

class CWinPingDlg : public CDialog
{
// Construction
public:
	CWinPingDlg(CWnd* pParent = NULL);	// standard constructor

	CPingThread	m_pingThread;

// Dialog Data
	//{{AFX_DATA(CWinPingDlg)
	enum { IDD = IDD_WINPING_DIALOG };
	CAutoComplete	m_cboServers;
	CSpinButtonCtrl	m_spinner;
	CListCtrl	m_list;
	CString	m_strRetry;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableControls(BOOL bEnable);
	CImageList	m_img;
	HICON m_hIcon;
	CPoint	m_ptTo;
	CPoint	m_ptFrom;
	CRect	m_rcRect;
	BOOL	m_bProgress;
	int		m_nPos;

	UINT	m_nDir;

	// Generated message map functions
	//{{AFX_MSG(CWinPingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPing();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAbout();
	virtual void OnCancel();
	//}}AFX_MSG
	LRESULT OnStatus(WPARAM wParam, LPARAM lParam);
	LRESULT OnPingEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINPINGDLG_H__D12A31A9_1D4C_4301_997D_8E8BCC2E54D5__INCLUDED_)
