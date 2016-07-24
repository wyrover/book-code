// NBTSTATDlg.h : header file
//

#if !defined(AFX_NBTSTATDLG_H__DE6CDDC7_A43B_11D5_A956_0050BA0F0366__INCLUDED_)
#define AFX_NBTSTATDLG_H__DE6CDDC7_A43B_11D5_A956_0050BA0F0366__INCLUDED_

#include "UDP.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNBTSTATDlg dialog

class CNBTSTATDlg : public CDialog
{
// Construction
public:
	void OnReceive();
	CUDP m_UDPSocket;
	CString	m_strIP;
	//CString	m_strIP2;

	CNBTSTATDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNBTSTATDlg)
	enum { IDD = IDD_NBTSTAT_DIALOG };
	CSpinButtonCtrl	m_spin;
	CListBox	m_ListBox;
	CIPAddressCtrl	m_IPEdit2;
	CListCtrl	m_ListView;
	CIPAddressCtrl	m_IPEdit1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNBTSTATDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNBTSTATDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	afx_msg void OnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnExit();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnAbout();
	afx_msg void OnDblclkListView(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NBTSTATDLG_H__DE6CDDC7_A43B_11D5_A956_0050BA0F0366__INCLUDED_)
