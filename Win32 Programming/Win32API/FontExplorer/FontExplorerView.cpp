// FontExplorerView.cpp : implementation of the CFontExplorerView class
//

#include "stdafx.h"
#include "FontExplorer.h"

#include "FontExplorerDoc.h"
#include "FontExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerView

IMPLEMENT_DYNCREATE(CFontExplorerView, CView)

BEGIN_MESSAGE_MAP(CFontExplorerView, CView)
        //{{AFX_MSG_MAP(CFontExplorerView)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerView construction/destruction

CFontExplorerView::CFontExplorerView()
{
        // TODO: add construction code here

}

CFontExplorerView::~CFontExplorerView()
{
}

BOOL CFontExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerView drawing

void CFontExplorerView::OnDraw(CDC* pDC)
{
        CFontExplorerDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerView printing

BOOL CFontExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CFontExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CFontExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerView diagnostics

#ifdef _DEBUG
void CFontExplorerView::AssertValid() const
{
        CView::AssertValid();
}

void CFontExplorerView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CFontExplorerDoc* CFontExplorerView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontExplorerDoc)));
        return (CFontExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerView message handlers
