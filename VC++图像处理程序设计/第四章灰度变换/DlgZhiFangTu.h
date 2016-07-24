#if !defined(AFX_DLGZHIFANGTU_H__7152DAB7_0D61_4A00_998E_174F32A6C01F__INCLUDED_)
#define AFX_DLGZHIFANGTU_H__7152DAB7_0D61_4A00_998E_174F32A6C01F__INCLUDED_

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
	float m_fIntensity[256];

	 

	CDlgZhiFangTu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgZhiFangTu)
	enum { IDD = IDD_DLG_zhifangtufenbu };
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

#endif // !defined(AFX_DLGZHIFANGTU_H__7152DAB7_0D61_4A00_998E_174F32A6C01F__INCLUDED_)
