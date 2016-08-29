// ControlExplorerView.cpp : implementation of the CControlExplorerView class
//

#include "stdafx.h"
#include "ControlExplorer.h"

#include "ControlExplorerDoc.h"
#include "ControlExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerView

IMPLEMENT_DYNCREATE(CControlExplorerView, CView)

BEGIN_MESSAGE_MAP(CControlExplorerView, CView)
	//{{AFX_MSG_MAP(CControlExplorerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerView construction/destruction

CControlExplorerView::CControlExplorerView()
{
	// TODO: add construction code here

}

CControlExplorerView::~CControlExplorerView()
{
}

BOOL CControlExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerView drawing

void CControlExplorerView::OnDraw(CDC* pDC)
{
	CControlExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerView printing

BOOL CControlExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CControlExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CControlExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerView diagnostics

#ifdef _DEBUG
void CControlExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CControlExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CControlExplorerDoc* CControlExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CControlExplorerDoc)));
	return (CControlExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerView message handlers
