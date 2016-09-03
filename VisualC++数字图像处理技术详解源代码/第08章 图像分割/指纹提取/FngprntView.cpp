// FngprntView.cpp : CFngprntView 类的实现
//

#include "stdafx.h"
#include "Fngprnt.h"

#include "FngprntDoc.h"
#include "FngprntView.h"

#include "Threshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFngprntView

IMPLEMENT_DYNCREATE(CFngprntView, CView)

BEGIN_MESSAGE_MAP(CFngprntView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_COMMAND(ID_FNGPRNT, &CFngprntView::OnFngprnt)
END_MESSAGE_MAP()

// CFngprntView 构造/析构

CFngprntView::CFngprntView()
{
	// TODO: 在此处添加构造代码

}

CFngprntView::~CFngprntView()
{
}

BOOL CFngprntView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFngprntView 绘制

void CFngprntView::OnDraw(CDC* pDC)
{
	CFngprntDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

    // 获取位图指针
    CDib *pDib = pDoc->GetDib();

    // 如果位图有效
    if(pDib->IsValid())
    {
        // 获取位图大小
        CSize size = pDib->GetDimension();

        // 在客户区内显示位图
        pDib->Draw(pDC, CPoint(0,0), size);
    }
}


// CFngprntView 打印

BOOL CFngprntView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFngprntView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFngprntView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CFngprntView 诊断

#ifdef _DEBUG
void CFngprntView::AssertValid() const
{
	CView::AssertValid();
}

void CFngprntView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFngprntDoc* CFngprntView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFngprntDoc)));
	return (CFngprntDoc*)m_pDocument;
}
#endif //_DEBUG


// CFngprntView 消息处理程序

void CFngprntView::OnFngprnt()
{
    // TODO: 在此添加命令处理程序代码

    // 获取文档类的指针
    CFngprntDoc *pDoc = GetDocument();

    // 获取位图指针
    CDib *pDib = pDoc->GetDib();

    // 如果位图有效
    if(pDib->IsValid())
    {
        // 调用CThreshold类中的最大方差阈值分割方法
        CThreshold fngprnt(pDib);
        fngprnt.OtusThreshold();

        // 客户区重绘
        Invalidate();
    }
}
