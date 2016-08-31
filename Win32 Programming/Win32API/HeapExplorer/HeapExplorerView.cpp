// HeapExplorerView.cpp : implementation of the CHeapExplorerView class
//

#include "stdafx.h"
#include "HeapExplorer.h"

#include "HeapExplorerDoc.h"
#include "HeapExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerView

IMPLEMENT_DYNCREATE(CHeapExplorerView, CView)

BEGIN_MESSAGE_MAP(CHeapExplorerView, CView)
        //{{AFX_MSG_MAP(CHeapExplorerView)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerView construction/destruction

CHeapExplorerView::CHeapExplorerView()
{
        // TODO: add construction code here

}

CHeapExplorerView::~CHeapExplorerView()
{
}

BOOL CHeapExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerView drawing

void CHeapExplorerView::OnDraw(CDC* pDC)
{
        CHeapExplorerDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerView printing

BOOL CHeapExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CHeapExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CHeapExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerView diagnostics

#ifdef _DEBUG
void CHeapExplorerView::AssertValid() const
{
        CView::AssertValid();
}

void CHeapExplorerView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CHeapExplorerDoc* CHeapExplorerView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHeapExplorerDoc)));
        return (CHeapExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerView message handlers
