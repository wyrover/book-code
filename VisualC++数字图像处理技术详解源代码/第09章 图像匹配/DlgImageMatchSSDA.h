#if !defined(AFX_DLGIMAGEMATCHSSDA_H__44A6CF9E_0655_456B_A1A4_D785578E6EE4__INCLUDED_)
#define AFX_DLGIMAGEMATCHSSDA_H__44A6CF9E_0655_456B_A1A4_D785578E6EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchSSDA.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchSSDA dialog

class CDlgImageMatchSSDA : public CDialog
{
// Construction
public:
	BOOL ImageMatchSSDA(CDib* pDibSrc, CDib* pDibTemplate);
	void CalImageLocation();
	CRect m_rectResltImage;
	CRect m_rectModelImage;
	CRect m_rectInitImage;
	CImageMatchingDoc* m_pDoc;
	CDib* m_pDibResult;
	CDib* m_pDibModel;
	CDib* m_pDibInit;
	BOOL m_bCalImgLoc;
	CDlgImageMatchSSDA(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchSSDA)
	enum { IDD = IDD_DIALOG_SSDA };
	long	m_lTk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchSSDA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchSSDA)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnOpentemplate();
	afx_msg void OnPaint();
	afx_msg void OnImagematch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHSSDA_H__44A6CF9E_0655_456B_A1A4_D785578E6EE4__INCLUDED_)
