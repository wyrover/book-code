// WaterShedView.cpp : implementation of the CWaterShedView class
//

#include "stdafx.h"
#include "WaterShed.h"

#include "WaterShedDoc.h"
#include "WaterShedView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaterShedView

IMPLEMENT_DYNCREATE(CWaterShedView, CScrollView)

BEGIN_MESSAGE_MAP(CWaterShedView, CScrollView)
	//{{AFX_MSG_MAP(CWaterShedView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterShedView construction/destruction

CWaterShedView::CWaterShedView()
{
	// TODO: add construction code here

}

CWaterShedView::~CWaterShedView()
{
}

BOOL CWaterShedView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWaterShedView drawing

void CWaterShedView::OnDraw(CDC* pDC)
{
// 	CWaterShedDoc* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CMainFrame *pFrame;
	pFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();             
	pFrame->pImageView = this;
	
	CWaterShedDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	

	//Ë¢ÐÂ±³¾°£»
	CBrush bkBrush( RGB(58,110,165) );
	CRect clientRect ;
	GetClientRect(&clientRect);
	int w = 0;
	int h = 0;
	if( pDoc->myImageObject!=NULL)
	{
		w = pDoc->myImageObject->GetWidth();
        h = pDoc->myImageObject->GetHeight();
	}
	w = w > clientRect.Width()  ? w : clientRect.Width();
	h = h > clientRect.Height() ? h : clientRect.Height();
	clientRect.right = w;
	clientRect.bottom = h;
	pDC->FillRect( &clientRect, &bkBrush );
	
	if( !pDoc->isImageLoaded )
	{
		return;
	}
	
	CSize sizet;
   	sizet.cx = pDoc->myImageObject->GetWidth();
	sizet.cy = pDoc->myImageObject->GetHeight();
	SetScrollSizes (MM_TEXT, sizet);
	
	//OnPrepareDC (pDC);
	CRect rect;
	GetClientRect(&rect);
	int width, height;
	width = pDoc->myImageObject->GetWidth();
	height = pDoc->myImageObject->GetHeight();
	
	if(width<rect.Width())
	{
		iX = (rect.Width()-width) / 2;
	}else
	{
		iX = 0;
	}		
	if(height<rect.Height())
	{
		iY = (rect.Height()-height) / 2;
	}else
	{
		iY = 0;
	}	
	
	if(GetFocus()==this)
	{
		pDoc->myImageObject->SetPalette( pDC );
	}	    
	pDoc->myImageObject->Draw(pDC, iX, iY);

}

void CWaterShedView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CWaterShedView printing

BOOL CWaterShedView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWaterShedView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWaterShedView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWaterShedView diagnostics

#ifdef _DEBUG
void CWaterShedView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWaterShedView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWaterShedDoc* CWaterShedView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaterShedDoc)));
	return (CWaterShedDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaterShedView message handlers
