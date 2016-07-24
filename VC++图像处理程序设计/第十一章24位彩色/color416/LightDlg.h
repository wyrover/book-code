#if !defined(AFX_LIGHTDLG_H__EA486950_1538_4B05_B8A0_8FD4CA504B1C__INCLUDED_)
#define AFX_LIGHTDLG_H__EA486950_1538_4B05_B8A0_8FD4CA504B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LightDlg dialog

class LightDlg : public CDialog
{
// Construction
public:
	LightDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LightDlg)
	enum { IDD = IDD_DIALOG2 };
	int		m_Light;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LightDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTDLG_H__EA486950_1538_4B05_B8A0_8FD4CA504B1C__INCLUDED_)
