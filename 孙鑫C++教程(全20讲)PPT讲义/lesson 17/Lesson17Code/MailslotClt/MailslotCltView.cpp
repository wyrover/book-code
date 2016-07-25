// MailslotCltView.cpp : implementation of the CMailslotCltView class
//

#include "stdafx.h"
#include "MailslotClt.h"

#include "MailslotCltDoc.h"
#include "MailslotCltView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltView

IMPLEMENT_DYNCREATE(CMailslotCltView, CView)

BEGIN_MESSAGE_MAP(CMailslotCltView, CView)
	//{{AFX_MSG_MAP(CMailslotCltView)
	ON_COMMAND(IDM_MAILSLOT_SEND, OnMailslotSend)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltView construction/destruction

CMailslotCltView::CMailslotCltView()
{
	// TODO: add construction code here

}

CMailslotCltView::~CMailslotCltView()
{
}

BOOL CMailslotCltView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltView drawing

void CMailslotCltView::OnDraw(CDC* pDC)
{
	CMailslotCltDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltView printing

BOOL CMailslotCltView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMailslotCltView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMailslotCltView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltView diagnostics

#ifdef _DEBUG
void CMailslotCltView::AssertValid() const
{
	CView::AssertValid();
}

void CMailslotCltView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMailslotCltDoc* CMailslotCltView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailslotCltDoc)));
	return (CMailslotCltDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltView message handlers

void CMailslotCltView::OnMailslotSend() 
{
	// TODO: Add your command handler code here
	HANDLE hMailslot;
	hMailslot=CreateFile("\\\\.\\mailslot\\MyMailslot",GENERIC_WRITE,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE==hMailslot)
	{
		MessageBox("打开油槽失败！");
		return;
	}
	char buf[]="http://www.sunxin.org";
	DWORD dwWrite;
	if(!WriteFile(hMailslot,buf,strlen(buf)+1,&dwWrite,NULL))
	{
		MessageBox("写入数据失败！");
		CloseHandle(hMailslot);
		return;
	}
	CloseHandle(hMailslot);
}
