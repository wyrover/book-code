#if !defined(AFX_THREETHREEJUNZHI_H__BAF549B1_30C3_4E96_B990_B414EA65DB05__INCLUDED_)
#define AFX_THREETHREEJUNZHI_H__BAF549B1_30C3_4E96_B990_B414EA65DB05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// THREETHREEJUNZHI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// THREETHREEJUNZHI dialog

class THREETHREEJUNZHI : public CDialog
{
// Construction
public:
	THREETHREEJUNZHI(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(THREETHREEJUNZHI)
	enum { IDD = IDD_DIALOG2 };
	int		m_T;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(THREETHREEJUNZHI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(THREETHREEJUNZHI)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREETHREEJUNZHI_H__BAF549B1_30C3_4E96_B990_B414EA65DB05__INCLUDED_)
