// MailslotSrvView.cpp : implementation of the CMailslotSrvView class
//

#include "stdafx.h"
#include "MailslotSrv.h"

#include "MailslotSrvDoc.h"
#include "MailslotSrvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvView

IMPLEMENT_DYNCREATE(CMailslotSrvView, CView)

BEGIN_MESSAGE_MAP(CMailslotSrvView, CView)
	//{{AFX_MSG_MAP(CMailslotSrvView)
	ON_COMMAND(IDM_MAILSLOT_RECV, OnMailslotRecv)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvView construction/destruction

CMailslotSrvView::CMailslotSrvView()
{
	// TODO: add construction code here

}

CMailslotSrvView::~CMailslotSrvView()
{
}

BOOL CMailslotSrvView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvView drawing

void CMailslotSrvView::OnDraw(CDC* pDC)
{
	CMailslotSrvDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvView printing

BOOL CMailslotSrvView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMailslotSrvView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMailslotSrvView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvView diagnostics

#ifdef _DEBUG
void CMailslotSrvView::AssertValid() const
{
	CView::AssertValid();
}

void CMailslotSrvView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMailslotSrvDoc* CMailslotSrvView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailslotSrvDoc)));
	return (CMailslotSrvDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvView message handlers

void CMailslotSrvView::OnMailslotRecv() 
{
	// TODO: Add your command handler code here
	HANDLE hMailslot;
	hMailslot=CreateMailslot("\\\\.\\mailslot\\MyMailslot",0,
		MAILSLOT_WAIT_FOREVER,NULL);
	if(INVALID_HANDLE_VALUE==hMailslot)
	{
		MessageBox("创建油槽失败！");
		return;
	}
	char buf[100];
	DWORD dwRead;
	if(!ReadFile(hMailslot,buf,100,&dwRead,NULL))
	{
		MessageBox("读取数据失败！");
		CloseHandle(hMailslot);
		return;
	}
	MessageBox(buf);
	CloseHandle(hMailslot);
}
