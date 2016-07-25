// DrawView.cpp : implementation of the CDrawView class
//

#include "stdafx.h"
#include "Draw.h"

#include "DrawDoc.h"
#include "DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CView)

BEGIN_MESSAGE_MAP(CDrawView, CView)
	//{{AFX_MSG_MAP(CDrawView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawView construction/destruction

CDrawView::CDrawView()
{
	// TODO: add construction code here
	m_bDraw = FALSE;
}

CDrawView::~CDrawView()
{
}

BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView drawing

void CDrawView::OnDraw(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView printing

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawDoc* CDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDoc)));
	return (CDrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawView message handlers

void CDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptOrigin = point;
	m_ptOld = point;
	m_bDraw = TRUE;
	CView::OnLButtonDown(nFlags, point);
}

void CDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*	// 首先获得窗口的设备描述表
	HDC hdc;
	hdc = ::GetDC(m_hWnd);
	//移动到线条的起点
	MoveToEx(hdc, m_ptOrigin.x, m_ptOrigin.y, NULL);
	//画线
	LineTo(hdc, point.x, point.y);
	//释放设备描述表
	::ReleaseDC(m_hWnd,hdc);
*/
/*	CDC* pDC = GetDC();
	pDC->MoveTo(m_ptOrigin);
	pDC->LineTo(point);
	ReleaseDC(pDC);
*/
//	CClientDC dc(GetParent());
	CWindowDC dc(GetDesktopWindow());
//	CWindowDC dc(GetParent());
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(point);

/*	CPen pen(PS_DOT,1,RGB(255,0,0));
	CClientDC dc(this);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
*/
/*	//创建一个红色画刷
	CBrush brush(RGB(255,0,0));
	//创建并获得设备描述表
	CClientDC dc(this);
	//利用红色画刷填充鼠标拖曳过程中形成的矩形区域
	dc.FillRect(CRect(m_ptOrigin,point),&brush);
*/

/*	//创建位图对象
	CBitmap bitmap;
	//加载位图资源
	bitmap.LoadBitmap(IDB_BITMAP1);
	//创建位图画刷
	CBrush brush(&bitmap);
	//创建并获得设备描述表
	CClientDC dc(this);
	//利用红色画刷填充鼠标拖曳过程中形成的矩形区域
	dc.FillRect(CRect(m_ptOrigin,point),&brush);
*/
	//创建并获得设备描述表
/*	CClientDC dc(this);
	//创建一个空画刷
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//将空画刷选入设备描述表
	CBrush *pOldBrush = dc.SelectObject(pBrush);
	//绘制一个矩形
	dc.Rectangle(CRect(m_ptOrigin,point));
	//恢复先前的画刷
	dc.SelectObject(pOldBrush);
*/
	m_bDraw = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

void CDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
/*	CClientDC dc(this);
	dc.SetROP2(R2_MERGENOTPEN);
	//创建一个红色的、宽度为1的实线画笔
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	//把创建的画笔选入设备描述表
	CPen *pOldPen = dc.SelectObject(&pen);
	if(m_bDraw == TRUE)
	{
//		dc.MoveTo(m_ptOrigin);
//		dc.LineTo(m_ptOld);
		dc.MoveTo(m_ptOrigin);
		dc.LineTo(point);
		dc.LineTo(m_ptOld);
		//修改线段的起点
		//m_ptOrigin = point;
		m_ptOld = point;
	}
	//恢复设备描述表
	dc.SelectObject(pOldPen);
*/	
	CView::OnMouseMove(nFlags, point);
}
