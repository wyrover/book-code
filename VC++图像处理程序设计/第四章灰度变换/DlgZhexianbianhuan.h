#if !defined(AFX_DLGZHEXIANBIANHUAN_H__6C1EA273_766F_4428_8F09_4BDF7CD85D86__INCLUDED_)
#define AFX_DLGZHEXIANBIANHUAN_H__6C1EA273_766F_4428_8F09_4BDF7CD85D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgZhexianbianhuan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgZhexianbianhuan dialog

class CDlgZhexianbianhuan : public CDialog
{
// Construction
public:
	CDlgZhexianbianhuan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgZhexianbianhuan)
	enum { IDD = IDD_DLG_Zhexian };
	BYTE	m_bX1;
	BYTE	m_bX2;
	BYTE	m_bY1;
	BYTE	m_bY2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgZhexianbianhuan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgZhexianbianhuan)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditX1();
	afx_msg void OnKillfocusEditX2();
	afx_msg void OnKillfocusEditY1();
	afx_msg void OnKillfocusEditY2();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGZHEXIANBIANHUAN_H__6C1EA273_766F_4428_8F09_4BDF7CD85D86__INCLUDED_)
