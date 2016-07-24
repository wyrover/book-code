#if !defined(AFX_PAINTCOLORDLG_H__12571D16_DF96_4B8E_8FE4_491FB37ABD00__INCLUDED_)
#define AFX_PAINTCOLORDLG_H__12571D16_DF96_4B8E_8FE4_491FB37ABD00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaintColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PaintColorDlg dialog

class PaintColorDlg : public CDialog
{
// Construction
public:
	PaintColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PaintColorDlg)
	enum { IDD = IDD_DIALOG5 };
	int		m_Blue;
	int		m_Green;
	int		m_Red;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PaintColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PaintColorDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAINTCOLORDLG_H__12571D16_DF96_4B8E_8FE4_491FB37ABD00__INCLUDED_)
