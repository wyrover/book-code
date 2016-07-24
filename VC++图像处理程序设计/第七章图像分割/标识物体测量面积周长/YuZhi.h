#if !defined(AFX_YUZHI_H__7691AA94_997A_4577_AC40_B815D2B10016__INCLUDED_)
#define AFX_YUZHI_H__7691AA94_997A_4577_AC40_B815D2B10016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YuZhi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYuZhi dialog

class CYuZhi : public CDialog
{
// Construction
public:
	CYuZhi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYuZhi)
	enum { IDD = IDD_DIALOG2 };
	int		m_gray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYuZhi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYuZhi)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YUZHI_H__7691AA94_997A_4577_AC40_B815D2B10016__INCLUDED_)
