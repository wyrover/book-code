// CDCStep3View.cpp : implementation of the CCDCStep3View class
//

#include "stdafx.h"
#include "CDCStep3.h"

#include "CDCStep3Doc.h"
#include "CDCStep3View.h"
//{{seojt
#include <math.h>
//}}seojt

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3View

IMPLEMENT_DYNCREATE(CCDCStep3View, CView)

BEGIN_MESSAGE_MAP(CCDCStep3View, CView)
    //{{AFX_MSG_MAP(CCDCStep3View)
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3View construction/destruction

CCDCStep3View::CCDCStep3View()
{
    // TODO: add construction code here

}

CCDCStep3View::~CCDCStep3View()
{
}

BOOL CCDCStep3View::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3View drawing

void CCDCStep3View::OnDraw(CDC* pDC)
{
    CCDCStep3Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3View printing

BOOL CCDCStep3View::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CCDCStep3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CCDCStep3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3View diagnostics

#ifdef _DEBUG
void CCDCStep3View::AssertValid() const
{
    CView::AssertValid();
}

void CCDCStep3View::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CCDCStep3Doc* CCDCStep3View::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCDCStep3Doc)));
    return (CCDCStep3Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3View message handlers

void CCDCStep3View::OnSize(UINT nType, int cx, int cy) 
{
    CView::OnSize(nType, cx, cy);
    
    // TODO: Add your message handler code here
    cxClient=cx;
    cyClient=cy;
    //}}seojt
    
}

void CCDCStep3View::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    CClientDC dc(this);				// F12
    int i;

    dc.MoveTo (0, cyClient / 2) ;//Why MoveToEx() isn't exist?
    dc.LineTo(cxClient, cyClient / 2) ;

    for (i = 0 ; i < NUM ; i++) {
        pt[i].x = i * cxClient / NUM ;
        pt[i].y = (int) (cyClient / 2 *
            (1 - sin (TWOPI * i / NUM))) ;
    }//for
    dc.Polyline (pt, NUM) ;
    //}}seojt
    
    CView::OnLButtonDown(nFlags, point);
}
