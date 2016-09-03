// CellDetectionView.h : CCellDetectionView 类的接口
//


#pragma once

#include "Dib.h"
#include "BinaryMorphDib.h"


class CCellDetectionView : public CView
{
protected: // 仅从序列化创建
	CCellDetectionView();
	DECLARE_DYNCREATE(CCellDetectionView)

// 属性
public:
	CCellDetectionDoc* GetDocument() const;
	CDib m_CellImage; //定义一个CDib类的对象

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCellDetectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenImage();
public:
	afx_msg void OnCellDetection();
};

#ifndef _DEBUG  // CellDetectionView.cpp 中的调试版本
inline CCellDetectionDoc* CCellDetectionView::GetDocument() const
   { return reinterpret_cast<CCellDetectionDoc*>(m_pDocument); }
#endif

