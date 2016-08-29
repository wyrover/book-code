// Window ExplorerView.cpp : implementation of the CWndExpView class
//

#include "stdafx.h"
#include "WndExp.h"

#include "WndExpDoc.h"
#include "WndExpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndExpView

IMPLEMENT_DYNCREATE(CWndExpView, CView)

BEGIN_MESSAGE_MAP(CWndExpView, CView)
	//{{AFX_MSG_MAP(CWndExpView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndExpView construction/destruction

CWndExpView::CWndExpView()
{
	// TODO: add construction code here

}

CWndExpView::~CWndExpView()
{
}

BOOL CWndExpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWndExpView drawing

void CWndExpView::OnDraw(CDC* pDC)
{
	CWndExpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWndExpView printing

BOOL CWndExpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWndExpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWndExpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWndExpView diagnostics

#ifdef _DEBUG
void CWndExpView::AssertValid() const
{
	CView::AssertValid();
}

void CWndExpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWndExpDoc* CWndExpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWndExpDoc)));
	return (CWndExpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWndExpView message handlers
