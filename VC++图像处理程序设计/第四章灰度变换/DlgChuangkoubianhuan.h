#if !defined(AFX_DLGCHUANGKOUBIANHUAN_H__6EA33542_50EE_4C8E_868F_B5CA3FCBDE83__INCLUDED_)
#define AFX_DLGCHUANGKOUBIANHUAN_H__6EA33542_50EE_4C8E_868F_B5CA3FCBDE83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChuangkoubianhuan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChuangkoubianhuan dialog

class CDlgChuangkoubianhuan : public CDialog
{
// Construction
public:
	CDlgChuangkoubianhuan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChuangkoubianhuan)
	enum { IDD = IDD_DIALOG3 };
	BYTE	m_bLow;
	BYTE	m_bUp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChuangkoubianhuan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChuangkoubianhuan)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEDITLow();
	afx_msg void OnKillfocusEDITUp();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHUANGKOUBIANHUAN_H__6EA33542_50EE_4C8E_868F_B5CA3FCBDE83__INCLUDED_)
