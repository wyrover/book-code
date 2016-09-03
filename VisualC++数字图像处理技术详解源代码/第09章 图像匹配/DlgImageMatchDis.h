#if !defined(AFX_DLGIMAGEMATCHDIS_H__4210084B_6668_4892_AA54_35F20AD6902F__INCLUDED_)
#define AFX_DLGIMAGEMATCHDIS_H__4210084B_6668_4892_AA54_35F20AD6902F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchDis.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchDis dialog

class CDlgImageMatchDis : public CDialog
{
// Construction
public:
	BOOL ImageMatchDis(CDib* pDibSrc, CDib* pDibTemplate);
	CDib* m_pDibResult;
	CDib* m_pDibModel;
	CDib* m_pDibInit;
	BOOL m_bCalImgLoc;
	CRect m_rectResltImage;
	CRect m_rectModelImage;
	CRect m_rectInitImage;
	CImageMatchingDoc* m_pDoc;
	void CalImageLocation();
	CDlgImageMatchDis(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchDis)
	enum { IDD = IDD_DIALOG_DIS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchDis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchDis)
	afx_msg void OnPaint();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnOpentemplate();
	afx_msg void OnImagematch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHDIS_H__4210084B_6668_4892_AA54_35F20AD6902F__INCLUDED_)
