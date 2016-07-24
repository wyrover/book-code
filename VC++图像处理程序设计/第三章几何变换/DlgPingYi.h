#if !defined(AFX_DLGPINGYI_H__EFDF98E6_DBB4_431E_85DB_0093A691D560__INCLUDED_)
#define AFX_DLGPINGYI_H__EFDF98E6_DBB4_431E_85DB_0093A691D560__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPingYi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPingYi dialog

class CDlgPingYi : public CDialog
{
// Construction
public:
	CDlgPingYi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPingYi)
	enum { IDD = IDD_DIALOG_Pingyi };
	long	m_Xmove;
	long	m_Ymove;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPingYi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPingYi)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPINGYI_H__EFDF98E6_DBB4_431E_85DB_0093A691D560__INCLUDED_)
