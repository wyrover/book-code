#if !defined(AFX_DIALOG3_H__86EBD8A6_860C_45C2_AE5B_674DE1AAD279__INCLUDED_)
#define AFX_DIALOG3_H__86EBD8A6_860C_45C2_AE5B_674DE1AAD279__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dialog3 dialog

class Dialog3 : public CDialog
{
// Construction
public:
	Dialog3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dialog3)
	enum { IDD = IDD_DIALOG3 };
	int		m_x;
	int		m_y;
	int		m_x1;
	int		m_y1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dialog3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dialog3)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG3_H__86EBD8A6_860C_45C2_AE5B_674DE1AAD279__INCLUDED_)
