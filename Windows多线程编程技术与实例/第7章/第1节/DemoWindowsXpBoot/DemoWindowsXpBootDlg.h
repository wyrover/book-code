// DemoWindowsXpBootDlg.h : header file
//

#if !defined(AFX_DEMOWINDOWSXPBOOTDLG_H__989D8B26_E374_47A1_97D4_3091CC823D2B__INCLUDED_)
#define AFX_DEMOWINDOWSXPBOOTDLG_H__989D8B26_E374_47A1_97D4_3091CC823D2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DynBar.h"



/////////////////////////////////////////////////////////////////////////////
// CDemoWindowsXpBootDlg dialog

class CDemoWindowsXpBootDlg : public CDialog
{
// Construction
public:
	CDemoWindowsXpBootDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoWindowsXpBootDlg)
	enum { IDD = IDD_DEMOWINDOWSXPBOOT_DIALOG };
	CDynBar	m_ctrlDynBar;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoWindowsXpBootDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemoWindowsXpBootDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButBoot();
	afx_msg void OnButStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOWINDOWSXPBOOTDLG_H__989D8B26_E374_47A1_97D4_3091CC823D2B__INCLUDED_)
