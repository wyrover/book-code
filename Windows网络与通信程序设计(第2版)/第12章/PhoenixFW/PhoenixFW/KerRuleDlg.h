#if !defined(AFX_KERRULEDLG_H__EC299161_1FC0_448A_91CD_64CB2FCBDBD6__INCLUDED_)
#define AFX_KERRULEDLG_H__EC299161_1FC0_448A_91CD_64CB2FCBDBD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KerRuleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKerRuleDlg dialog

class CKerRuleDlg : public CDialog
{
public:
	static PassthruFilter m_RuleItem;	// 要添加的过滤规则

// Construction
public:
	CKerRuleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKerRuleDlg)
	enum { IDD = IDD_KERRULE };
	CEdit	m_SourcePort;
	CIPAddressCtrl	m_SourceMask;
	CIPAddressCtrl	m_SourceIP;
	CComboBox	m_RuleProtocol;
	CComboBox	m_RuleAction;
	CEdit	m_DestPort;
	CIPAddressCtrl	m_DestMask;
	CIPAddressCtrl	m_DestIP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKerRuleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKerRuleDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KERRULEDLG_H__EC299161_1FC0_448A_91CD_64CB2FCBDBD6__INCLUDED_)
