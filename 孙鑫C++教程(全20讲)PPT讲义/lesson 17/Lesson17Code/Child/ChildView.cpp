// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Child.h"

#include "ChildDoc.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

IMPLEMENT_DYNCREATE(CChildView, CView)

BEGIN_MESSAGE_MAP(CChildView, CView)
	//{{AFX_MSG_MAP(CChildView)
	ON_COMMAND(IDM_PIPE_READ, OnPipeRead)
	ON_COMMAND(IDM_PIPE_WRITE, OnPipeWrite)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView construction/destruction

CChildView::CChildView()
{
	// TODO: add construction code here
	hRead=NULL;
	hWrite=NULL;
}

CChildView::~CChildView()
{
	if(hRead)
		CloseHandle(hRead);
	if(hWrite)
		CloseHandle(hWrite);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildView drawing

void CChildView::OnDraw(CDC* pDC)
{
	CChildDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CChildView printing

BOOL CChildView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CChildView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CChildView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CChildView diagnostics

#ifdef _DEBUG
void CChildView::AssertValid() const
{
	CView::AssertValid();
}

void CChildView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChildDoc* CChildView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChildDoc)));
	return (CChildDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

void CChildView::OnPipeRead() 
{
	// TODO: Add your command handler code here
	char buf[100];
	DWORD dwRead;
	if(!ReadFile(hRead,buf,100,&dwRead,NULL))
	{
		MessageBox("读取数据失败！");
		return;
	}
	MessageBox(buf);
}

void CChildView::OnPipeWrite() 
{
	// TODO: Add your command handler code here
	char buf[]="匿名管道测试程序";
	DWORD dwWrite;
	if(!WriteFile(hWrite,buf,strlen(buf)+1,&dwWrite,NULL))
	{
		MessageBox("写入数据失败！");
		return;
	}
}

void CChildView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	hRead=GetStdHandle(STD_INPUT_HANDLE);
	hWrite=GetStdHandle(STD_OUTPUT_HANDLE);
}
