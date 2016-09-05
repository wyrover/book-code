// Exp_13_3_2Dlg.h : header file
//

#if !defined(AFX_EXP_13_3_2DLG_H__14906A68_F897_43F0_9973_2EBD7611D2A3__INCLUDED_)
#define AFX_EXP_13_3_2DLG_H__14906A68_F897_43F0_9973_2EBD7611D2A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlab.hpp"

/////////////////////////////////////////////////////////////////////////////
// CExp_13_3_2Dlg dialog

class CExp_13_3_2Dlg : public CDialog
{
// Construction
public:
	CExp_13_3_2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExp_13_3_2Dlg)
	enum { IDD = IDD_EXP_13_3_2_DIALOG };
	CString	m_data;
	CString	m_data2;
	int		m_num;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExp_13_3_2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExp_13_3_2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnExecute();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	mxArray *m_pA;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXP_13_3_2DLG_H__14906A68_F897_43F0_9973_2EBD7611D2A3__INCLUDED_)
