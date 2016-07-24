// GetNetSettingDlg.h : header file
//

#if !defined(AFX_GETNETSETTINGDLG_H__0629FB2B_3F85_4E41_9084_2141E0EF2228__INCLUDED_)
#define AFX_GETNETSETTINGDLG_H__0629FB2B_3F85_4E41_9084_2141E0EF2228__INCLUDED_

#include "lassNetSetting.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGetNetSettingDlg dialog
#include "CardInfoDlg.h"

class CGetNetSettingDlg : public CDialog
{
// Construction
public:
	void SplitString(LPCTSTR lpszString, CStringArray &strArray);
	CCardInfoDlg dlg;
	ClassNetSetting m_clsNetSetting;
	CGetNetSettingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGetNetSettingDlg)
	enum { IDD = IDD_GETNETSETTING_DIALOG };
	CListBox	m_listNetcard;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetNetSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGetNetSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQueryinfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETNETSETTINGDLG_H__0629FB2B_3F85_4E41_9084_2141E0EF2228__INCLUDED_)
