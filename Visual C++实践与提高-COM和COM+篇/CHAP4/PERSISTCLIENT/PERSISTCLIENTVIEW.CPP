// PersistClientView.cpp : implementation of the CPersistClientView class
//

#include "stdafx.h"
#include "PersistClient.h"

#include "PersistClientDoc.h"
#include "PersistClientView.h"
#include "Thread.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersistClientView

IMPLEMENT_DYNCREATE(CPersistClientView, CView)

BEGIN_MESSAGE_MAP(CPersistClientView, CView)
    //{{AFX_MSG_MAP(CPersistClientView)
    ON_COMMAND(ID_STORAGEREAD, OnStorageread)
    ON_COMMAND(ID_STORAGEWRITE, OnStoragewrite)
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersistClientView construction/destruction

CPersistClientView::CPersistClientView()
{
    // TODO: add construction code here
    pInfo = NULL;
}

CPersistClientView::~CPersistClientView()
{
}

BOOL CPersistClientView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPersistClientView drawing

void CPersistClientView::OnDraw(CDC* pDC)
{
    CPersistClientDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: add draw code for native data here
    //if(pInfo!=NULL)
    CRect rc;
    GetClientRect(&rc);
    pDC->DrawText(info, &rc, DT_LEFT);
}

/////////////////////////////////////////////////////////////////////////////
// CPersistClientView printing

BOOL CPersistClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CPersistClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CPersistClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPersistClientView diagnostics

#ifdef _DEBUG
void CPersistClientView::AssertValid() const
{
    CView::AssertValid();
}

void CPersistClientView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CPersistClientDoc* CPersistClientView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPersistClientDoc)));
    return (CPersistClientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPersistClientView message handlers

void CPersistClientView::OnStorageread()
{
    pInfo = NULL;
    CString* pInfo = new CString; // sInfo;
    CWinThread* pThread = AfxBeginThread(ReadThreadProc, pInfo);
    delete pInfo;
    Invalidate();
}

void CPersistClientView::OnStoragewrite()
{
    CWinThread* pThread = AfxBeginThread(WriteThreadProc, GetSafeHwnd());
    Invalidate();
}
