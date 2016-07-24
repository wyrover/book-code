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
#include "FuShiYuPengZhangDib.h"
#include "CDIB.h"

class CDynSplitView2 : public CView
{
protected:
	CDynSplitView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDynSplitView2)

// Attributes
public:
	int start[8];
	CDSplitDoc* GetDocument();

// Operations
public:
		CPalette *CreateBitmapPalette( FuShiYuPengZhangDib* pBitmap);
 		CString filename;   //打开的文件名
		FuShiYuPengZhangDib *CDibNew1;  //处理后图像的引用
		CDib *CDib1;   //原图像的引用
		CPalette hPalette;
		int state2;    //显示状态变量，为1时右视图显示图像
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
	afx_msg void OnChuizhifushi();
	afx_msg void OnShuipingfushi();
	afx_msg void OnQuanfangwei();
	afx_msg void OnShuipingpengzhang();
	afx_msg void OnChuizhipengzhang();
	afx_msg void OnQuanfangxiangpengzhang();
	afx_msg void OnKaiqi();
	afx_msg void OnBihe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)
