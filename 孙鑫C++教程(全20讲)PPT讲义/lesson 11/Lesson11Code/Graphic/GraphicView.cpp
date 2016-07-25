// GraphicView.cpp : implementation of the CGraphicView class
//

#include "stdafx.h"
#include "Graphic.h"

#include "GraphicDoc.h"
#include "GraphicView.h"

#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicView

IMPLEMENT_DYNCREATE(CGraphicView, CScrollView)

BEGIN_MESSAGE_MAP(CGraphicView, CScrollView)
	//{{AFX_MSG_MAP(CGraphicView)
	ON_COMMAND(IDM_DOT, OnDot)
	ON_COMMAND(IDM_LINE, OnLine)
	ON_COMMAND(IDM_RECTANGLE, OnRectangle)
	ON_COMMAND(IDM_ELLIPSE, OnEllipse)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicView construction/destruction

CGraphicView::CGraphicView()
{
	// TODO: add construction code here
	m_nDrawType=0;
	m_ptOrigin=0;
	m_dcMetaFile.Create();
}

CGraphicView::~CGraphicView()
{
}

BOOL CGraphicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView drawing

void CGraphicView::OnDraw(CDC* pDC)
{
	CGraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
/*	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(pBrush);

	for(int i=0;i<m_ptrArray.GetSize();i++)
	{
		switch(((CGraph*)m_ptrArray.GetAt(i))->m_nDrawType)
		{
		case 1:
			pDC->SetPixel(((CGraph*)m_ptrArray.GetAt(i))->m_ptEnd,RGB(0,0,0));
			break;
		case 2:
			pDC->MoveTo(((CGraph*)m_ptrArray.GetAt(i))->m_ptOrigin);
			pDC->LineTo(((CGraph*)m_ptrArray.GetAt(i))->m_ptEnd);
			break;
		case 3:
			pDC->Rectangle(CRect(((CGraph*)m_ptrArray.GetAt(i))->m_ptOrigin,
				((CGraph*)m_ptrArray.GetAt(i))->m_ptEnd));
			break;
		case 4:
			pDC->Ellipse(CRect(((CGraph*)m_ptrArray.GetAt(i))->m_ptOrigin,
				((CGraph*)m_ptrArray.GetAt(i))->m_ptEnd));
			break;
		}
	}*/
/*	HMETAFILE hmetaFile;
	hmetaFile=m_dcMetaFile.Close();
	pDC->PlayMetaFile(hmetaFile);
	m_dcMetaFile.Create();
	m_dcMetaFile.PlayMetaFile(hmetaFile);
	DeleteMetaFile(hmetaFile);*/
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&m_dcCompatible,0,0,SRCCOPY);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView printing

BOOL CGraphicView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGraphicView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGraphicView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView diagnostics

#ifdef _DEBUG
void CGraphicView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGraphicView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGraphicDoc* CGraphicView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicDoc)));
	return (CGraphicDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphicView message handlers

void CGraphicView::OnDot() 
{
	// TODO: Add your command handler code here
	m_nDrawType=1;
}

void CGraphicView::OnLine() 
{
	// TODO: Add your command handler code here
	m_nDrawType=2;
}

void CGraphicView::OnRectangle() 
{
	// TODO: Add your command handler code here
	m_nDrawType=3;
}

void CGraphicView::OnEllipse() 
{
	// TODO: Add your command handler code here
	m_nDrawType=4;
}

void CGraphicView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptOrigin=point;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CGraphicView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//dc.SelectObject(pBrush);
	m_dcMetaFile.SelectObject(pBrush);

	if(!m_dcCompatible.m_hDC)
	{
		m_dcCompatible.CreateCompatibleDC(&dc);
		CRect rect;
		GetClientRect(&rect);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
		m_dcCompatible.SelectObject(&bitmap);
		m_dcCompatible.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);
		m_dcCompatible.SelectObject(pBrush);
	}
	switch(m_nDrawType)
	{
	case 1:
		//dc.SetPixel(point,RGB(0,0,0));
		//m_dcMetaFile.SetPixel(point,RGB(0,0,0));
		m_dcCompatible.SetPixel(point,RGB(0,0,0));
		break;
	case 2:
		//dc.MoveTo(m_ptOrigin);
		//dc.LineTo(point);
		//m_dcMetaFile.MoveTo(m_ptOrigin);
		//m_dcMetaFile.LineTo(point);
		m_dcCompatible.MoveTo(m_ptOrigin);
		m_dcCompatible.LineTo(point);
		break;
	case 3:
		//dc.Rectangle(CRect(m_ptOrigin,point));
		//m_dcMetaFile.Rectangle(CRect(m_ptOrigin,point));
		m_dcCompatible.Rectangle(CRect(m_ptOrigin,point));
		break;
	case 4:
		//dc.Ellipse(CRect(m_ptOrigin,point));
		//m_dcMetaFile.Ellipse(CRect(m_ptOrigin,point));
		m_dcCompatible.Ellipse(CRect(m_ptOrigin,point));
		break;
	}
	//CGraph graph(m_nDrawType,m_ptOrigin,point);
	//m_ptrArray.Add(&graph);
/*	OnPrepareDC(&dc);
	dc.DPtoLP(&m_ptOrigin);
	dc.DPtoLP(&point);
	CGraph *pGraph=new CGraph(m_nDrawType,m_ptOrigin,point);
	m_ptrArray.Add(pGraph);*/
	CScrollView::OnLButtonUp(nFlags, point);
}

void CGraphicView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	OnPrepareDC(&dc);
	OnDraw(&dc);
	// Do not call CScrollView::OnPaint() for painting messages
}

void CGraphicView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT,CSize(800,600));
}

void CGraphicView::OnFileSave() 
{
	// TODO: Add your command handler code here
	HMETAFILE hmetaFile;
	hmetaFile=m_dcMetaFile.Close();
	CopyMetaFile(hmetaFile,"meta.wmf");
	m_dcMetaFile.Create();
	DeleteMetaFile(hmetaFile);
}

void CGraphicView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	HMETAFILE hmetaFile;
	hmetaFile=GetMetaFile("meta.wmf");
	m_dcMetaFile.PlayMetaFile(hmetaFile);
	DeleteMetaFile(hmetaFile);
	Invalidate();
}
