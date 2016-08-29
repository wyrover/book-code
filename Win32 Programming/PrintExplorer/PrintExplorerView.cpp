// PrintExplorerView.cpp : implementation of the CPrintExplorerView class
//

#include "stdafx.h"
#include "PrintExplorer.h"

#include "PrintExplorerDoc.h"
#include "PrintExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerView

IMPLEMENT_DYNCREATE(CPrintExplorerView, CView)

BEGIN_MESSAGE_MAP(CPrintExplorerView, CView)
        //{{AFX_MSG_MAP(CPrintExplorerView)
        ON_COMMAND(IDM_PRINTDLG, OnPrintdlg)
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerView construction/destruction

CPrintExplorerView::CPrintExplorerView()
{
        // TODO: add construction code here

}

CPrintExplorerView::~CPrintExplorerView()
{
}

BOOL CPrintExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerView drawing

void CPrintExplorerView::OnDraw(CDC* pDC)
{
        CPrintExplorerDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerView printing

BOOL CPrintExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CPrintExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CPrintExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerView diagnostics

#ifdef _DEBUG
void CPrintExplorerView::AssertValid() const
{
        CView::AssertValid();
}

void CPrintExplorerView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CPrintExplorerDoc* CPrintExplorerView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrintExplorerDoc)));
        return (CPrintExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerView message handlers

void CPrintExplorerView::OnPrintDirect() 
{
        // TODO: Add your command handler code here
        
}

void CPrintExplorerView::OnPrintsetupDirect() 
{
        // TODO: Add your command handler code here
        
}

void CPrintExplorerView::OnPrintdlg() 
{
 ((CPrintExplorerApp *)AfxGetApp())->doPrintDlg(this);  // TODO: pass in file ID
        
}
