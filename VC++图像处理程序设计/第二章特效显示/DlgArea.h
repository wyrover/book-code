#if !defined(AFX_DLGAREA_H__4B91E483_3A49_4DBA_A589_1202DFB5ED48__INCLUDED_)
#define AFX_DLGAREA_H__4B91E483_3A49_4DBA_A589_1202DFB5ED48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgArea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgArea dialog

class CDlgArea : public CDialog
{
// Construction
public:
	CDlgArea(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgArea)
	enum { IDD = IDD_DIALOG_Area };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgArea)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgArea)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAREA_H__4B91E483_3A49_4DBA_A589_1202DFB5ED48__INCLUDED_)
