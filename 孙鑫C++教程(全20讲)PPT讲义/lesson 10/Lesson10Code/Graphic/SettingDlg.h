#if !defined(AFX_SETTINGDLG_H__CD802ADF_6285_42E2_AB60_C44F8A92D064__INCLUDED_)
#define AFX_SETTINGDLG_H__CD802ADF_6285_42E2_AB60_C44F8A92D064__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
#include "TestBtn.h"
#include "SXBtn.h"
#include "BtnST.h"
class CSettingDlg : public CDialog
{
// Construction
public:
	COLORREF m_clr;
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_DLG_SETTING };
	CButtonST	m_btnST;
	CSXBtn	m_btnCancel;
	CTestBtn	m_btnTest;
	UINT	m_nLineWidth;
	int		m_nLineStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	afx_msg void OnChangeLineWidth();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFont m_font;
	CBrush m_brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__CD802ADF_6285_42E2_AB60_C44F8A92D064__INCLUDED_)
