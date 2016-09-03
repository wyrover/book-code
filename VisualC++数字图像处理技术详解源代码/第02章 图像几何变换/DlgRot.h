#if !defined(AFX_DLGROT_H__D813C37F_8355_4C0B_A37C_ED3911721774__INCLUDED_)
#define AFX_DLGROT_H__D813C37F_8355_4C0B_A37C_ED3911721774__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRot dialog

class CDlgRot : public CDialog
{
	// Construction
public:
	CDlgRot(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgRot)
	enum { IDD = IDD_dlg_rotation };
	int		m_rotAngle;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRot)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRot)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROT_H__D813C37F_8355_4C0B_A37C_ED3911721774__INCLUDED_)
