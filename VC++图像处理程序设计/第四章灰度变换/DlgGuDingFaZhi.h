#if !defined(AFX_DLGGUDINGFAZHI_H__E986239E_4F9A_4AB2_A278_742B29E895AF__INCLUDED_)
#define AFX_DLGGUDINGFAZHI_H__E986239E_4F9A_4AB2_A278_742B29E895AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGuDingFaZhi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGuDingFaZhi dialog

class CDlgGuDingFaZhi : public CDialog
{
// Construction
public:
	CDlgGuDingFaZhi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGuDingFaZhi)
	 enum { IDD = IDD_DIALOG_Guding};
	int		m_Yuzhi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGuDingFaZhi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGuDingFaZhi)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGUDINGFAZHI_H__E986239E_4F9A_4AB2_A278_742B29E895AF__INCLUDED_)
