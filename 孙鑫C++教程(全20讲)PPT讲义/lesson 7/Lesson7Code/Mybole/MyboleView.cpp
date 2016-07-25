// MyboleView.cpp : implementation of the CMyboleView class
//

#include "stdafx.h"
#include "Mybole.h"

#include "MyboleDoc.h"
#include "MyboleView.h"

#include "TestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyboleView

IMPLEMENT_DYNCREATE(CMyboleView, CView)

BEGIN_MESSAGE_MAP(CMyboleView, CView)
	//{{AFX_MSG_MAP(CMyboleView)
	ON_COMMAND(IDM_DIALOG, OnDialog)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyboleView construction/destruction

CMyboleView::CMyboleView()
{
	// TODO: add construction code here

}

CMyboleView::~CMyboleView()
{
}

BOOL CMyboleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyboleView drawing

void CMyboleView::OnDraw(CDC* pDC)
{
	CMyboleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMyboleView printing

BOOL CMyboleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyboleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyboleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyboleView diagnostics

#ifdef _DEBUG
void CMyboleView::AssertValid() const
{
	CView::AssertValid();
}

void CMyboleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyboleDoc* CMyboleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyboleDoc)));
	return (CMyboleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyboleView message handlers

void CMyboleView::OnDialog() 
{
	// TODO: Add your command handler code here
	CTestDlg dlg;
	dlg.DoModal();

/*	CTestDlg *pDlg=new CTestDlg();
	pDlg->Create(IDD_DIALOG1,this);
	pDlg->ShowWindow(SW_SHOW);*/ 
}
