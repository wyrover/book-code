// PhotoProcessView.h : CPhotoProcessView 类的接口
//


#pragma once

#include "Dib.h"
#include "GrayTransformDib.h"
#include "HistogramDib.h"
#include "SmoothProcessDib.h"
#include "SharpenProcessDib.h"
#include "FrequencyFilterDib.h"
#include "ColorEnhanceDib.h"


class CPhotoProcessView : public CView
{
protected: // 仅从序列化创建
	CPhotoProcessView();
	DECLARE_DYNCREATE(CPhotoProcessView)

// 属性
public:
	CPhotoProcessDoc* GetDocument() const;

    CDib m_PhotoImage;//照片图像
    BOOL flag;//打开照片图像标记变量

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
	virtual ~CPhotoProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnOpenPhoto();
public:
    afx_msg void OnLinearTransform();
public:
    afx_msg void OnSegLineTransform();
public:
    afx_msg void OnLogTransform();
public:
    afx_msg void OnHistogramEqual();
public:
    afx_msg void OnHistogramMatch();
public:
    afx_msg void OnGateGrad();
public:
    afx_msg void OnLaplacian();

public:
    afx_msg void OnAverageSmooth();
public:
    afx_msg void OnValueAvrgSmooth();
public:
    afx_msg void OnSelectSmooth();
public:
    afx_msg void OnMiddleSmooth();
public:
    afx_msg void OnPrefectLowFilter();
public:
    afx_msg void OnPrefectHighFilter();
public:
    afx_msg void OnPseudoColorEnhance();
};

#ifndef _DEBUG  // PhotoProcessView.cpp 中的调试版本
inline CPhotoProcessDoc* CPhotoProcessView::GetDocument() const
   { return reinterpret_cast<CPhotoProcessDoc*>(m_pDocument); }
#endif

