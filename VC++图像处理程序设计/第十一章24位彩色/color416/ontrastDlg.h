#if !defined(AFX_ONTRASTDLG_H__2613DE6D_5301_4732_90AA_FDE72E90D321__INCLUDED_)
#define AFX_ONTRASTDLG_H__2613DE6D_5301_4732_90AA_FDE72E90D321__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ontrastDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ContrastDlg dialog

class ContrastDlg : public CDialog
{
// Construction
public:
	ContrastDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ContrastDlg)
	enum { IDD = IDD_DIALOG3 };
	int		m_Increment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ContrastDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ContrastDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONTRASTDLG_H__2613DE6D_5301_4732_90AA_FDE72E90D321__INCLUDED_)
