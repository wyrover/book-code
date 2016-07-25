// ASCView.cpp : implementation of the CASCView class
//

#include "stdafx.h"
#include "ASC.h"

#include "ASCDoc.h"
#include "ASCView.h"

#include "SearchLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CASCView

IMPLEMENT_DYNCREATE(CASCView, CView)

BEGIN_MESSAGE_MAP(CASCView, CView)
	//{{AFX_MSG_MAP(CASCView)
	ON_COMMAND(IDM_SEARCHLOG, OnSearchlog)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CASCView construction/destruction

CASCView::CASCView()
{
	// TODO: add construction code here

}

CASCView::~CASCView()
{
}

BOOL CASCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CASCView drawing

void CASCView::OnDraw(CDC* pDC)
{
	CASCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CASCView printing

BOOL CASCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CASCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CASCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CASCView diagnostics

#ifdef _DEBUG
void CASCView::AssertValid() const
{
	CView::AssertValid();
}

void CASCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CASCDoc* CASCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CASCDoc)));
	return (CASCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CASCView message handlers

void CASCView::OnSearchlog() 
{
	// TODO: Add your command handler code here
	CSearchLogDlg searchdlg;
	searchdlg.DoModal();
	
}
