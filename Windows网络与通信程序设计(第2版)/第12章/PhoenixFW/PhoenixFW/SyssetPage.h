#if !defined(AFX_SYSSETPAGE_H__A58C43DD_CA34_4ED5_AD7A_C0BAC9B95BED__INCLUDED_)
#define AFX_SYSSETPAGE_H__A58C43DD_CA34_4ED5_AD7A_C0BAC9B95BED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SyssetPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSyssetPage dialog

class CSyssetPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSyssetPage)

// Construction
public:
	CSyssetPage();
	~CSyssetPage();

	void SetLspWorkMode(int nWorkMode);
	void SetKerWorkMode(int nWorkMode);

// Dialog Data
	//{{AFX_DATA(CSyssetPage)
	enum { IDD = IDD_SYSSET };
	CButton	m_AutoStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSyssetPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSyssetPage)
	afx_msg void OnInstall();
	afx_msg void OnRemove();
	afx_msg void OnAutostart();
	afx_msg void OnPassAll();
	afx_msg void OnQueryAll();
	afx_msg void OnDenyAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnKerpassAll();
	afx_msg void OnKerstartFilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETPAGE_H__A58C43DD_CA34_4ED5_AD7A_C0BAC9B95BED__INCLUDED_)
