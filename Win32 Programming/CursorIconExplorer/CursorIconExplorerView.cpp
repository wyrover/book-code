// CursorIconExplorerView.cpp : implementation of the CCursorIconExplorerView class
//

#include "stdafx.h"
#include "CursorIconExplorer.h"

#include "CursorIconExplorerDoc.h"
#include "CursorIconExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerView

IMPLEMENT_DYNCREATE(CCursorIconExplorerView, CView)

BEGIN_MESSAGE_MAP(CCursorIconExplorerView, CView)
	//{{AFX_MSG_MAP(CCursorIconExplorerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerView construction/destruction

CCursorIconExplorerView::CCursorIconExplorerView()
{
	// TODO: add construction code here

}

CCursorIconExplorerView::~CCursorIconExplorerView()
{
}

BOOL CCursorIconExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerView drawing

void CCursorIconExplorerView::OnDraw(CDC* pDC)
{
	CCursorIconExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerView printing

BOOL CCursorIconExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCursorIconExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCursorIconExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerView diagnostics

#ifdef _DEBUG
void CCursorIconExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CCursorIconExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCursorIconExplorerDoc* CCursorIconExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCursorIconExplorerDoc)));
	return (CCursorIconExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerView message handlers
