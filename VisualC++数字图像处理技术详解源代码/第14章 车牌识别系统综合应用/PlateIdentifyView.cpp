// PlateIdentifyView.cpp : CPlateIdentifyView 类的实现
//

#include "stdafx.h"
#include "PlateIdentify.h"

#include "PlateIdentifyDoc.h"
#include "PlateIdentifyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlateIdentifyView

IMPLEMENT_DYNCREATE(CPlateIdentifyView, CView)

BEGIN_MESSAGE_MAP(CPlateIdentifyView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPlateIdentifyView 构造/析构

CPlateIdentifyView::CPlateIdentifyView()
{
	// TODO: 在此处添加构造代码

}

CPlateIdentifyView::~CPlateIdentifyView()
{
}

BOOL CPlateIdentifyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlateIdentifyView 绘制

void CPlateIdentifyView::OnDraw(CDC* pDC)
{
	CPlateIdentifyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

    myimg & img=pDoc->m_Cimage;
    CRect m_rect;
    GetClientRect(&m_rect) ;
    //计算图像的宽度和高度，能够按照原始大小显示
    m_rect.right=img.Width();
    m_rect.bottom=img.Height();
    img.DrawToHDC(pDC->GetSafeHdc(),m_rect);




}


// CPlateIdentifyView 打印

BOOL CPlateIdentifyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlateIdentifyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlateIdentifyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CPlateIdentifyView 诊断

#ifdef _DEBUG
void CPlateIdentifyView::AssertValid() const
{
	CView::AssertValid();
}

void CPlateIdentifyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlateIdentifyDoc* CPlateIdentifyView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlateIdentifyDoc)));
	return (CPlateIdentifyDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlateIdentifyView 消息处理程序
