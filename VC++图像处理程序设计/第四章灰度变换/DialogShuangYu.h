#if !defined(AFX_DIALOGSHUANGYU_H__F9E47A24_5C88_482E_A9A6_5278A6F15B27__INCLUDED_)
#define AFX_DIALOGSHUANGYU_H__F9E47A24_5C88_482E_A9A6_5278A6F15B27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogShuangYu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogShuangYu dialog

class CDialogShuangYu : public CDialog
{
// Construction
public:
	CDialogShuangYu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogShuangYu)
	enum { IDD = IDD_DIALOG_Shuangyu };
	int		m_ZhiYu1;
	int		m_ZhiYu2;
	int m_mode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogShuangYu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogShuangYu)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSHUANGYU_H__F9E47A24_5C88_482E_A9A6_5278A6F15B27__INCLUDED_)
