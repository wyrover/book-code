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
#include "PinYuLuBoDib.h"
#include "CDIB.h"

class CDynSplitView2 : public CView
{
protected:
	CDynSplitView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDynSplitView2)

// Attributes
public:
	CDSplitDoc* GetDocument();
//    PinYuLuBoDib* m_pBitmap2;
    int	c1;

// Operations
public:
	void clearmem2();
		CPalette *CreateBitmapPalette( PinYuLuBoDib* pBitmap);
 		CString filename;
		PinYuLuBoDib *CDibNew1;
		CDib *CDib1;
		CPalette hPalette;
		int state2;
		void clearmem();

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
	afx_msg void OnButterworthL();
	afx_msg void OnButterworthH();
	afx_msg void OnQuickForuier();
	afx_msg void OnBox();
	afx_msg void OnPerfectl();
	afx_msg void OnPerfecth();
	afx_msg void OnTl();
	afx_msg void OnTh();
	afx_msg void OnZl();
	afx_msg void OnZh();
	afx_msg void OnFirstQuickForuier();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)
