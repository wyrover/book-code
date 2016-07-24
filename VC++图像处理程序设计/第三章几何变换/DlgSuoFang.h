#if !defined(AFX_DLGSUOFANG_H__C75012E1_9DDF_4602_894E_AB6F49E98B4F__INCLUDED_)
#define AFX_DLGSUOFANG_H__C75012E1_9DDF_4602_894E_AB6F49E98B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuoFang.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuoFang dialog

class CDlgSuoFang : public CDialog
{
// Construction
public:
	CDlgSuoFang(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuoFang)
	enum { IDD = IDD_DIALOG_Suofang };
	float	m_XZoom;
	float	m_YZoom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuoFang)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuoFang)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUOFANG_H__C75012E1_9DDF_4602_894E_AB6F49E98B4F__INCLUDED_)
