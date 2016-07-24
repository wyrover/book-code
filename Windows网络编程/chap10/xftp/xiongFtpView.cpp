// xiongFtpView.cpp : implementation of the CXiongFtpView class
//

#include "stdafx.h"
#include "xiongFtp.h"

#include "xiongFtpDoc.h"
#include "xiongFtpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpView

IMPLEMENT_DYNCREATE(CXiongFtpView, CListView)

BEGIN_MESSAGE_MAP(CXiongFtpView, CListView)
	//{{AFX_MSG_MAP(CXiongFtpView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpView construction/destruction

CXiongFtpView::CXiongFtpView()
{
	// TODO: add construction code here

}

CXiongFtpView::~CXiongFtpView()
{
}

BOOL CXiongFtpView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpView drawing

void CXiongFtpView::OnDraw(CDC* pDC)
{
	CXiongFtpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CXiongFtpView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpView printing

BOOL CXiongFtpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CXiongFtpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CXiongFtpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpView diagnostics

#ifdef _DEBUG
void CXiongFtpView::AssertValid() const
{
	CListView::AssertValid();
}

void CXiongFtpView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CXiongFtpDoc* CXiongFtpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiongFtpDoc)));
	return (CXiongFtpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpView message handlers
