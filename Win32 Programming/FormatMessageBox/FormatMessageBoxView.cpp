// FormatMessageBoxView.cpp : implementation of the CFormatMessageBoxView class
//

#include "stdafx.h"
#include "FormatMessageBox.h"

#include "FormatMessageBoxDoc.h"
#include "FormatMessageBoxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxView

IMPLEMENT_DYNCREATE(CFormatMessageBoxView, CView)

BEGIN_MESSAGE_MAP(CFormatMessageBoxView, CView)
	//{{AFX_MSG_MAP(CFormatMessageBoxView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxView construction/destruction

CFormatMessageBoxView::CFormatMessageBoxView()
{
	// TODO: add construction code here

}

CFormatMessageBoxView::~CFormatMessageBoxView()
{
}

BOOL CFormatMessageBoxView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxView drawing

void CFormatMessageBoxView::OnDraw(CDC* pDC)
{
	CFormatMessageBoxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxView printing

BOOL CFormatMessageBoxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFormatMessageBoxView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFormatMessageBoxView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxView diagnostics

#ifdef _DEBUG
void CFormatMessageBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CFormatMessageBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFormatMessageBoxDoc* CFormatMessageBoxView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormatMessageBoxDoc)));
	return (CFormatMessageBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxView message handlers
