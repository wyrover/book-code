// NamedPipeCltView.cpp : implementation of the CNamedPipeCltView class
//

#include "stdafx.h"
#include "NamedPipeClt.h"

#include "NamedPipeCltDoc.h"
#include "NamedPipeCltView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltView

IMPLEMENT_DYNCREATE(CNamedPipeCltView, CView)

BEGIN_MESSAGE_MAP(CNamedPipeCltView, CView)
	//{{AFX_MSG_MAP(CNamedPipeCltView)
	ON_COMMAND(IDM_PIPE_CONNECT, OnPipeConnect)
	ON_COMMAND(IDM_PIPE_READ, OnPipeRead)
	ON_COMMAND(IDM_PIPE_WRITE, OnPipeWrite)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltView construction/destruction

CNamedPipeCltView::CNamedPipeCltView()
{
	// TODO: add construction code here
	hPipe=NULL;
}

CNamedPipeCltView::~CNamedPipeCltView()
{
	if(hPipe)
		CloseHandle(hPipe);
}

BOOL CNamedPipeCltView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltView drawing

void CNamedPipeCltView::OnDraw(CDC* pDC)
{
	CNamedPipeCltDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltView printing

BOOL CNamedPipeCltView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNamedPipeCltView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNamedPipeCltView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltView diagnostics

#ifdef _DEBUG
void CNamedPipeCltView::AssertValid() const
{
	CView::AssertValid();
}

void CNamedPipeCltView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNamedPipeCltDoc* CNamedPipeCltView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNamedPipeCltDoc)));
	return (CNamedPipeCltDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltView message handlers

void CNamedPipeCltView::OnPipeConnect() 
{
	// TODO: Add your command handler code here
	if(!WaitNamedPipe("\\\\.\\pipe\\MyPipe",NMPWAIT_WAIT_FOREVER))
	{
		MessageBox("当前没有可利用的命名管道实例！");
		return;
	}
	hPipe=CreateFile("\\\\.\\pipe\\MyPipe",GENERIC_READ | GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE==hPipe)
	{
		MessageBox("打开命名管道失败！");
		hPipe=NULL;
		return;
	}
}

void CNamedPipeCltView::OnPipeRead() 
{
	// TODO: Add your command handler code here
	char buf[100];
	DWORD dwRead;
	if(!ReadFile(hPipe,buf,100,&dwRead,NULL))
	{
		MessageBox("读取数据失败！");
		return;
	}
	MessageBox(buf);
}

void CNamedPipeCltView::OnPipeWrite() 
{
	// TODO: Add your command handler code here
	char buf[]="命名管道测试程序";
	DWORD dwWrite;
	if(!WriteFile(hPipe,buf,strlen(buf)+1,&dwWrite,NULL))
	{
		MessageBox("写入数据失败！");
		return;
	}
}
