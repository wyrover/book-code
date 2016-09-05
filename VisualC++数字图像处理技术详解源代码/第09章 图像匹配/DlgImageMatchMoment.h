#if !defined(AFX_DLGIMAGEMATCHMOMENT_H__7762BB53_6B0A_455F_9A03_A76AFAE0980B__INCLUDED_)
#define AFX_DLGIMAGEMATCHMOMENT_H__7762BB53_6B0A_455F_9A03_A76AFAE0980B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchMoment.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchMoment dialog

class CDlgImageMatchMoment : public CDialog
{
// Construction
public:
	double BarycenterMoment(CDib *pDib, int nBarycenterX, int nBarycenterY, int ip, int jq);
	void CalBarycenter(CDib *pDib, int *nBarycenterX, int *nBarycenterY);
	BOOL CalMoment(CDib *pDibSrc, CDib *pDibTemplate);
	void CalImageLocation();
	CRect m_rectModelImage;
	CRect m_rectInitImage;
	CImageMatchingDoc* m_pDoc;
	CDib* m_pDibModel;
	CDib* m_pDibInit;
	BOOL m_bCalImgLoc;
	CDlgImageMatchMoment(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchMoment)
	enum { IDD = IDD_DIALOG_MOMENT };
	double	m_a1_inimage;
	double	m_a1_template;
	double	m_a2_inimage;
	double	m_a2_template;
	double	m_a3_inimage;
	double	m_a3_template;
	double	m_a4_inimage;
	double	m_a4_template;
	double	m_a5_inimage;
	double	m_a5_template;
	double	m_a6_inimage;
	double	m_a6_template;
	double	m_a7_inimage;
	double	m_a7_template;
	double	m_R;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchMoment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchMoment)
	afx_msg void OnCalmoment();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnOpentemplate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHMOMENT_H__7762BB53_6B0A_455F_9A03_A76AFAE0980B__INCLUDED_)
