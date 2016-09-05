#if !defined(AFX_DLGIMAGEMATCHLSC_H__C91AF5DC_A491_4D68_B28D_B94283A3C066__INCLUDED_)
#define AFX_DLGIMAGEMATCHLSC_H__C91AF5DC_A491_4D68_B28D_B94283A3C066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchLSC.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchLSC dialog

class CDlgImageMatchLSC : public CDialog
{
// Construction
public:
	BOOL ContraryMatrix(double *const pMatrix, double *const _pMatrix, const int dim);
	BOOL ImageMatchLSC(CDib* pDibSrc, CDib* pDibTemplate);
	void CalImageLocation();
	CRect m_rectResltImage;
	CRect m_rectModelImage;
	CRect m_rectInitImage;
	CImageMatchingDoc* m_pDoc;
	CDib* m_pDibResult;
	CDib* m_pDibModel;
	CDib* m_pDibInit;
	BOOL m_bCalImgLoc;
	CDlgImageMatchLSC(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchLSC)
	enum { IDD = IDD_DIALOG_LSC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchLSC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchLSC)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnOpentemplate();
	afx_msg void OnImagematch();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHLSC_H__C91AF5DC_A491_4D68_B28D_B94283A3C066__INCLUDED_)
