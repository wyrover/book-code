// SegmentView.cpp : CSegmentView 类的实现
//

#include "stdafx.h"
#include "Segment.h"

#include "SegmentDoc.h"
#include "SegmentView.h"

#include "EdgeDetection.h"
#include "Threshold.h"
#include "ContourSegment.h"
#include "RegionSegment.h"
#include "ColorSeg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSegmentView

IMPLEMENT_DYNCREATE(CSegmentView, CView)

BEGIN_MESSAGE_MAP(CSegmentView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_COMMAND(ID_ADAPTHRESH, &CSegmentView::OnAdapthresh)
    ON_COMMAND(ID_CONTOUREXTRACT, &CSegmentView::OnContourextract)
    ON_COMMAND(ID_CONTOURTRACK, &CSegmentView::OnContourtrack)
    ON_COMMAND(ID_REGIONGROW, &CSegmentView::OnRegiongrow)
    ON_WM_LBUTTONDOWN()
    ON_COMMAND(ID_OTUSTHRESHOLD, &CSegmentView::OnOtusthreshold)
    ON_COMMAND(ID_COLORPREWITT, &CSegmentView::OnColorprewitt)
    ON_COMMAND(ID_ROBERTS, &CSegmentView::OnRoberts)
    ON_COMMAND(ID_SOBEL, &CSegmentView::OnSobel)
    ON_COMMAND(ID_PREWITT, &CSegmentView::OnPrewitt)
    ON_COMMAND(ID_KRISCH, &CSegmentView::OnKrisch)
    ON_COMMAND(ID_LAPLACIAN, &CSegmentView::OnLaplacian)
    ON_COMMAND(ID_GUASSLAPLACIAN, &CSegmentView::OnGuasslaplacian)
END_MESSAGE_MAP()

// CSegmentView 构造/析构

CSegmentView::CSegmentView()
{
	// TODO: 在此处添加构造代码
    flag = FALSE;

}

CSegmentView::~CSegmentView()
{
}

BOOL CSegmentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSegmentView 绘制

void CSegmentView::OnDraw(CDC* pDC)
{
	CSegmentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        CSize size = pDib->GetDimension();
        pDib->Draw(pDC, CPoint(0,0), size);
    }
}


// CSegmentView 打印

BOOL CSegmentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSegmentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSegmentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CSegmentView 诊断

#ifdef _DEBUG
void CSegmentView::AssertValid() const
{
	CView::AssertValid();
}

void CSegmentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSegmentDoc* CSegmentView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSegmentDoc)));
	return (CSegmentDoc*)m_pDocument;
}
#endif //_DEBUG


void CSegmentView::OnAdapthresh()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CThreshold threshold(pDib);
        threshold.AdaptiveThreshold();
        Invalidate();
        }        
    }    
}

void CSegmentView::OnContourextract()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CContourSegment contourSeg(pDib);
            contourSeg.ContourExtract();
            Invalidate();
        }        
    }
}

void CSegmentView::OnContourtrack()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CContourSegment contourSeg(pDib);
            contourSeg.ContourTrack();
            Invalidate();
        }        
    }
}

void CSegmentView::OnRegiongrow()
{
    // TODO: 在此添加命令处理程序代码
    AfxMessageBox("请在图上单击鼠标左键选取种子点！");
    flag = TRUE;
}

void CSegmentView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(flag)
    {
        flag = FALSE;
        CSegmentDoc *pDoc = GetDocument();
        CDib *pDib = pDoc->GetDib();
        if(pDib->IsValid())
        {
            if(!pDib->IsGrade())
            {
                AfxMessageBox("本程序只支持灰度位图！");
                return;
            }
            else
            { 
                CRegionSegment regionSeg(pDib);
                regionSeg.RegionGrow(point);
                Invalidate();
            }       
        }
    }

    CView::OnLButtonDown(nFlags, point);
}

void CSegmentView::OnOtusthreshold()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CThreshold threshold(pDib);
            threshold.OtusThreshold();
            Invalidate();
        }       
    }
}

void CSegmentView::OnColorprewitt()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持24位真彩位图！");
            return;
        }
        else
        {
            CColorSeg colorSeg(pDib);
            colorSeg.Prewitt();
            Invalidate();
        }
    }
}

void CSegmentView::OnRoberts()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Roberts();
            Invalidate();
        }       
    }
}

void CSegmentView::OnSobel()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Sobel();
            Invalidate();
        }       
    }
}

void CSegmentView::OnPrewitt()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Prewitt();
            Invalidate();
        }       
    }
}

void CSegmentView::OnKrisch()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Krisch();
            Invalidate();
        }       
    }
}

void CSegmentView::OnLaplacian()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.Laplacian();
            Invalidate();
        }       
    }
}

void CSegmentView::OnGuasslaplacian()
{
    // TODO: 在此添加命令处理程序代码
    CSegmentDoc *pDoc = GetDocument();
    CDib *pDib = pDoc->GetDib();
    if(pDib->IsValid())
    {
        if(!pDib->IsGrade())
        {
            AfxMessageBox("本程序只支持灰度位图！");
            return;
        }
        else
        {
            CEdgeDetection edgeDetection(pDib);
            edgeDetection.GuassLaplacian();
            Invalidate();
        }       
    }
}