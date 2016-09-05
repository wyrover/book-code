#if !defined(AFX_DLGsvd_H__D813C37F_8355_4C0B_A37C_ED3911721774__INCLUDED_)
#define AFX_DLGsvd_H__D813C37F_8355_4C0B_A37C_ED3911721774__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlgsvd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgsvd dialog

class CDlgsvd : public CDialog
{
	// Construction
public:
	CDlgsvd(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgsvd)
	enum { IDD = IDD_dlg_svd };
	int		m_svd;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgsvd)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgsvd)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGsvd_H__D813C37F_8355_4C0B_A37C_ED3911721774__INCLUDED_)