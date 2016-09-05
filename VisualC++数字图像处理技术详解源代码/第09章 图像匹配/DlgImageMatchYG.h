#if !defined(AFX_DLGIMAGEMATCHYG_H__837E3419_CD11_4416_987E_BB154B449C86__INCLUDED_)
#define AFX_DLGIMAGEMATCHYG_H__837E3419_CD11_4416_987E_BB154B449C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchYG.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchYG dialog

class CDlgImageMatchYG : public CDialog
{
// Construction
public:
	BOOL ImageMatchYG(CDib* pDibSrc, CDib* pDibTemplate);
	void CalImageLocation();
	CRect m_rectResltImage;
	CRect m_rectModelImage;
	CRect m_rectInitImage;
	CImageMatchingDoc* m_pDoc;
	CDib* m_pDibResult;
	CDib* m_pDibModel;
	CDib* m_pDibInit;
	BOOL m_bCalImgLoc;
	CDlgImageMatchYG(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchYG)
	enum { IDD = IDD_DIALOG_YG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchYG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchYG)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnOpentemplate();
	afx_msg void OnImagematch();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHYG_H__837E3419_CD11_4416_987E_BB154B449C86__INCLUDED_)
