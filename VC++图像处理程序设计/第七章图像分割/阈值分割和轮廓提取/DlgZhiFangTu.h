#if !defined(AFX_DLGZHIFANGTU_H__2A623A37_0011_46AB_A779_52CD1CF789E8__INCLUDED_)
#define AFX_DLGZHIFANGTU_H__2A623A37_0011_46AB_A779_52CD1CF789E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgZhiFangTu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgZhiFangTu dialog

class CDlgZhiFangTu : public CDialog
{
// Construction
public:
	CDlgZhiFangTu(CWnd* pParent = NULL);   // standard constructor
    float m_fIntensity[256];
// Dialog Data
	//{{AFX_DATA(CDlgZhiFangTu)
	enum { IDD = IDD_DIALOG_ZhiFangTu };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgZhiFangTu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgZhiFangTu)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGZHIFANGTU_H__2A623A37_0011_46AB_A779_52CD1CF789E8__INCLUDED_)
