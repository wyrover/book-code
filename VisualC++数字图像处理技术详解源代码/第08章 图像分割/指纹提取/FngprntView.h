// FngprntView.h : CFngprntView 类的接口
//


#pragma once


class CFngprntView : public CView
{
protected: // 仅从序列化创建
	CFngprntView();
	DECLARE_DYNCREATE(CFngprntView)

// 属性
public:
	CFngprntDoc* GetDocument() const;

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
	virtual ~CFngprntView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFngprnt();
};

#ifndef _DEBUG  // FngprntView.cpp 中的调试版本
inline CFngprntDoc* CFngprntView::GetDocument() const
   { return reinterpret_cast<CFngprntDoc*>(m_pDocument); }
#endif

