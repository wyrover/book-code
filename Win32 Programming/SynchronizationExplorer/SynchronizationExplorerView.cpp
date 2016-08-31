// SynchronizationExplorerView.cpp : implementation of the CSynchronizationExplorerView class
//

#include "stdafx.h"
#include "SynchronizationExplorer.h"

#include "SynchronizationExplorerDoc.h"
#include "SynchronizationExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerView

IMPLEMENT_DYNCREATE(CSynchronizationExplorerView, CView)

BEGIN_MESSAGE_MAP(CSynchronizationExplorerView, CView)
	//{{AFX_MSG_MAP(CSynchronizationExplorerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerView construction/destruction

CSynchronizationExplorerView::CSynchronizationExplorerView()
{
	// TODO: add construction code here

}

CSynchronizationExplorerView::~CSynchronizationExplorerView()
{
}

BOOL CSynchronizationExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerView drawing

void CSynchronizationExplorerView::OnDraw(CDC* pDC)
{
	CSynchronizationExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerView printing

BOOL CSynchronizationExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSynchronizationExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSynchronizationExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerView diagnostics

#ifdef _DEBUG
void CSynchronizationExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CSynchronizationExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSynchronizationExplorerDoc* CSynchronizationExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSynchronizationExplorerDoc)));
	return (CSynchronizationExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerView message handlers
