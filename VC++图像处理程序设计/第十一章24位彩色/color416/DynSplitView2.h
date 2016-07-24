#if !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)
#define AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DynSplitView2.h : header file
//
#include"DSplitDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 view
#include "MakeColorDib.h"
#include "CDIB.h"

class CDynSplitView2 : public CView
{
protected:
	CDynSplitView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDynSplitView2)

// Attributes
public:
	CDSplitDoc* GetDocument();
int 	 m_byRed,m_byGreen,m_byBlue;
// Operations
public:
		CPalette *CreateBitmapPalette( MakeColorDib* pBitmap);
 		CString filename;
		MakeColorDib *CDibNew1;
		CDib *CDib1;
		CPalette hPalette;
		int state2;
		void clearmem();
		int m_GaoTong;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynSplitView2)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDynSplitView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDynSplitView2)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFilesave();
	afx_msg void OnMakegGray();
	afx_msg void OnLightAlter();
	afx_msg void OnLightReverse();
	afx_msg void OnContrastAlter();
	afx_msg void OnExposal();
	afx_msg void OnPaintColor();
	afx_msg void OnSmoothness();
	afx_msg void OnNeonLight();
	afx_msg void OnEmbossment();
	afx_msg void OnSpread();
	afx_msg void OnSharp();
	afx_msg void OnHighLVBO();
	afx_msg void OnLowLVBO();
	afx_msg void OnShuiPingGROW();
	afx_msg void OnChuiZhiGROW();
	afx_msg void OnShuangXiangGROW();
	afx_msg void OnHighLVBOnormal();
	afx_msg void OnHighLVBOexcessive();
	afx_msg void OnLowLVBObig();
	afx_msg void OnMosaic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)
