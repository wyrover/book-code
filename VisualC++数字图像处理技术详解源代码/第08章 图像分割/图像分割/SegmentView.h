// SegmentView.h : CSegmentView 类的接口
//


#pragma once


class CSegmentView : public CView
{
protected: // 仅从序列化创建
	CSegmentView();
	DECLARE_DYNCREATE(CSegmentView)

// 属性
public:
	CSegmentDoc* GetDocument() const;

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

private:
    BOOL flag;

// 实现
public:
	virtual ~CSegmentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnAdapthresh();
public:
    afx_msg void OnContourextract();
public:
    afx_msg void OnContourtrack();
public:
    afx_msg void OnRegiongrow();
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
    afx_msg void OnOtusthreshold();
public:
    afx_msg void OnColorprewitt();
public:
    afx_msg void OnRoberts();
public:
    afx_msg void OnSobel();
public:
    afx_msg void OnPrewitt();
public:
    afx_msg void OnKrisch();
public:
    afx_msg void OnLaplacian();
public:
    afx_msg void OnGuasslaplacian();
};

#ifndef _DEBUG  // SegmentView.cpp 中的调试版本
inline CSegmentDoc* CSegmentView::GetDocument() const
   { return reinterpret_cast<CSegmentDoc*>(m_pDocument); }
#endif

