// CtlColorExplorerView.cpp : implementation of the CCtlColorExplorerView class
//

#include "stdafx.h"
#include "CtlColorExplorer.h"

#include "CtlColorExplorerDoc.h"
#include "CtlColorExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerView

IMPLEMENT_DYNCREATE(CCtlColorExplorerView, CView)

BEGIN_MESSAGE_MAP(CCtlColorExplorerView, CView)
        //{{AFX_MSG_MAP(CCtlColorExplorerView)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerView construction/destruction

CCtlColorExplorerView::CCtlColorExplorerView()
{
        // TODO: add construction code here

}

CCtlColorExplorerView::~CCtlColorExplorerView()
{
}

BOOL CCtlColorExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerView drawing

void CCtlColorExplorerView::OnDraw(CDC* pDC)
{
        CCtlColorExplorerDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerView printing

BOOL CCtlColorExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CCtlColorExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CCtlColorExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerView diagnostics

#ifdef _DEBUG
void CCtlColorExplorerView::AssertValid() const
{
        CView::AssertValid();
}

void CCtlColorExplorerView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CCtlColorExplorerDoc* CCtlColorExplorerView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCtlColorExplorerDoc)));
        return (CCtlColorExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerView message handlers
