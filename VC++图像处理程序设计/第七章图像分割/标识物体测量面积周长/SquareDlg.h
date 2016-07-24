#if !defined(AFX_SQUAREDLG_H__5AC48587_92FF_4368_990C_70EB9EBB34B2__INCLUDED_)
#define AFX_SQUAREDLG_H__5AC48587_92FF_4368_990C_70EB9EBB34B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SquareDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SquareDlg dialog

class SquareDlg : public CDialog
{
// Construction
public:
	SquareDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SquareDlg)
	enum { IDD = IDD_SQUARE_DLG };
	int		m_number;
	int		m_squareALL;
	CString	m_ShuChu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SquareDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SquareDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQUAREDLG_H__5AC48587_92FF_4368_990C_70EB9EBB34B2__INCLUDED_)
