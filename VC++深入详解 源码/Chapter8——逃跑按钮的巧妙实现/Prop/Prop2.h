#if !defined(AFX_PROP2_H__4E07A01F_C9F7_405B_93A2_7C034C0AC97B__INCLUDED_)
#define AFX_PROP2_H__4E07A01F_C9F7_405B_93A2_7C034C0AC97B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Prop2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProp2 dialog

class CProp2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CProp2)

// Construction
public:
	CProp2();
	~CProp2();

// Dialog Data
	//{{AFX_DATA(CProp2)
	enum { IDD = IDD_PROP2 };
	BOOL	m_football;
	BOOL	m_basketball;
	BOOL	m_volleyball;
	BOOL	m_swim;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProp2)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProp2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROP2_H__4E07A01F_C9F7_405B_93A2_7C034C0AC97B__INCLUDED_)
