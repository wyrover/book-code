// StructStoreView.cpp : implementation of the CStructStoreView class
//

#include "stdafx.h"
#include "StructStore.h"

#include "StructStoreDoc.h"
#include "StructStoreView.h"
#include "Thread.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructStoreView

IMPLEMENT_DYNCREATE(CStructStoreView, CView)

BEGIN_MESSAGE_MAP(CStructStoreView, CView)
    //{{AFX_MSG_MAP(CStructStoreView)
    ON_COMMAND(ID_STORAGEREAD, OnStorageread)
    ON_COMMAND(ID_STORAGEWRITE, OnStoragewrite)
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructStoreView construction/destruction

CStructStoreView::CStructStoreView()
{
    // TODO: add construction code here
}

CStructStoreView::~CStructStoreView()
{
}

BOOL CStructStoreView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CStructStoreView drawing

void CStructStoreView::OnDraw(CDC* pDC)
{
    CStructStoreDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CStructStoreView printing

BOOL CStructStoreView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CStructStoreView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CStructStoreView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CStructStoreView diagnostics

#ifdef _DEBUG
void CStructStoreView::AssertValid() const
{
    CView::AssertValid();
}

void CStructStoreView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CStructStoreDoc* CStructStoreView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStructStoreDoc)));
    return (CStructStoreDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructStoreView message handlers

void CStructStoreView::OnStorageread()
{
    CWinThread* pThread = AfxBeginThread(ReadThreadProc, GetSafeHwnd());
}

void CStructStoreView::OnStoragewrite()
{
    CWinThread* pThread = AfxBeginThread(WriteThreadProc, GetSafeHwnd());
}
