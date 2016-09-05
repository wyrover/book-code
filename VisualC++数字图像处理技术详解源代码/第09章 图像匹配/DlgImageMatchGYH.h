#if !defined(AFX_DLGIMAGEMATCHGYH_H__E98CA2E0_F9C4_4525_9B13_477F299BDC1E__INCLUDED_)
#define AFX_DLGIMAGEMATCHGYH_H__E98CA2E0_F9C4_4525_9B13_477F299BDC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchGYH.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchGYH dialog

class CDlgImageMatchGYH : public CDialog
{
// Construction
public:
	BOOL ImageMatchGYH(CDib* pDibSrc, CDib* pDibTemplate);    // 模板匹配
	void CalImageLocation();    // 设置图象等控件的位置大小
	CRect m_rectResltImage;     // 匹配后图象显示区域
	CRect m_rectModelImage;     // 模板图象显示区域
	CRect m_rectInitImage;      // 初始图象显示区域
	CImageMatchingDoc* m_pDoc;  // 文档类指针
	CDib* m_pDibResult;         // 匹配后的图象
	CDib* m_pDibModel;          // 模板图象
	CDib* m_pDibInit;           // 初始图象
	BOOL m_bCalImgLoc;          // 计算图象位置的标志位。FALSE表示还没有计算图象位置
	CDlgImageMatchGYH(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchGYH)
	enum { IDD = IDD_DIALOG_GYH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchGYH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchGYH)
	afx_msg void OnOpentemplate();
	afx_msg void OnImagematch();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHGYH_H__E98CA2E0_F9C4_4525_9B13_477F299BDC1E__INCLUDED_)
