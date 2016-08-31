// MenuExplorerView.cpp : implementation of the CMenuExplorerView class
//

#include "stdafx.h"

#include "MenuExplorer.h"

#include "MenuExplorerDoc.h"
#include "MenuExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerView

IMPLEMENT_DYNCREATE(CMenuExplorerView, CView)

BEGIN_MESSAGE_MAP(CMenuExplorerView, CView)
        //{{AFX_MSG_MAP(CMenuExplorerView)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerView construction/destruction

CMenuExplorerView::CMenuExplorerView()
{
        // TODO: add construction code here

}

CMenuExplorerView::~CMenuExplorerView()
{
}

BOOL CMenuExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerView drawing

void CMenuExplorerView::OnDraw(CDC* pDC)
{
        CMenuExplorerDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerView printing

BOOL CMenuExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CMenuExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CMenuExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerView diagnostics

#ifdef _DEBUG
void CMenuExplorerView::AssertValid() const
{
        CView::AssertValid();
}

void CMenuExplorerView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CMenuExplorerDoc* CMenuExplorerView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMenuExplorerDoc)));
        return (CMenuExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerView message handlers
