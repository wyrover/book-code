#if !defined(AFX_NNJUNZHI_H__693E9D53_E218_440C_B4DC_14084D87F1C1__INCLUDED_)
#define AFX_NNJUNZHI_H__693E9D53_E218_440C_B4DC_14084D87F1C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NNJUNZHI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NNJUNZHI dialog

class NNJUNZHI : public CDialog
{
// Construction
public:
	NNJUNZHI(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NNJUNZHI)
	enum { IDD = IDD_DIALOG3 };
	int		m_n;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NNJUNZHI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NNJUNZHI)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NNJUNZHI_H__693E9D53_E218_440C_B4DC_14084D87F1C1__INCLUDED_)
