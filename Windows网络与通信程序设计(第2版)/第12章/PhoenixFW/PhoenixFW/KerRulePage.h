#if !defined(AFX_KERRULEPAGE_H__AC6E2053_8D7F_4995_A46A_7C162DCD5C24__INCLUDED_)
#define AFX_KERRULEPAGE_H__AC6E2053_8D7F_4995_A46A_7C162DCD5C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KerRulePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKerRulePage dialog

class CKerRulePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CKerRulePage)

// Construction
public:
	CKerRulePage();
	~CKerRulePage();

	void AddRuleToList(PPassthruFilter pItem, int nEditIndex = -1);

	int InitAddRule();

	void UpdateList();

// Dialog Data
	//{{AFX_DATA(CKerRulePage)
	enum { IDD = IDD_KERRULE_CONTROL };
	CListCtrl	m_kerrules;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKerRulePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKerRulePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeradd();
	afx_msg void OnKerdel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KERRULEPAGE_H__AC6E2053_8D7F_4995_A46A_7C162DCD5C24__INCLUDED_)
