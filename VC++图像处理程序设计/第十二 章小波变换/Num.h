#if !defined(AFX_NUM_H__C6ABD69D_EBCA_441C_A125_D7B22FB9AE5D__INCLUDED_)
#define AFX_NUM_H__C6ABD69D_EBCA_441C_A125_D7B22FB9AE5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Num.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNum dialog

class CNum : public CDialog
{
// Construction
public:
	CNum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNum)
	enum { IDD = IDD_DIALOG3 };
	int		m_Num;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNum)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUM_H__C6ABD69D_EBCA_441C_A125_D7B22FB9AE5D__INCLUDED_)
