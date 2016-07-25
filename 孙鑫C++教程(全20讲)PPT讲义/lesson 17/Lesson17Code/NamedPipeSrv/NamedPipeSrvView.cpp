// NamedPipeSrvView.cpp : implementation of the CNamedPipeSrvView class
//

#include "stdafx.h"
#include "NamedPipeSrv.h"

#include "NamedPipeSrvDoc.h"
#include "NamedPipeSrvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvView

IMPLEMENT_DYNCREATE(CNamedPipeSrvView, CView)

BEGIN_MESSAGE_MAP(CNamedPipeSrvView, CView)
	//{{AFX_MSG_MAP(CNamedPipeSrvView)
	ON_COMMAND(IDM_PIPE_CREATE, OnPipeCreate)
	ON_COMMAND(IDM_PIPE_READ, OnPipeRead)
	ON_COMMAND(IDM_PIPE_WRITE, OnPipeWrite)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvView construction/destruction

CNamedPipeSrvView::CNamedPipeSrvView()
{
	// TODO: add construction code here
	hPipe=NULL;
}

CNamedPipeSrvView::~CNamedPipeSrvView()
{
	if(hPipe)
		CloseHandle(hPipe);
}

BOOL CNamedPipeSrvView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvView drawing

void CNamedPipeSrvView::OnDraw(CDC* pDC)
{
	CNamedPipeSrvDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvView printing

BOOL CNamedPipeSrvView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNamedPipeSrvView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNamedPipeSrvView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvView diagnostics

#ifdef _DEBUG
void CNamedPipeSrvView::AssertValid() const
{
	CView::AssertValid();
}

void CNamedPipeSrvView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNamedPipeSrvDoc* CNamedPipeSrvView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNamedPipeSrvDoc)));
	return (CNamedPipeSrvDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvView message handlers

void CNamedPipeSrvView::OnPipeCreate() 
{
	// TODO: Add your command handler code here
	hPipe=CreateNamedPipe("\\\\.\\pipe\\MyPipe",
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		0,1,1024,1024,0,NULL);
	if(INVALID_HANDLE_VALUE==hPipe)
	{
		MessageBox("创建命名管道失败！");
		hPipe=NULL;
		return;
	}
	HANDLE hEvent;
	hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	if(!hEvent)
	{
		MessageBox("创建事件对象失败！");
		CloseHandle(hPipe);
		hPipe=NULL;
		return;
	}
	OVERLAPPED ovlap;
	ZeroMemory(&ovlap,sizeof(OVERLAPPED));
	ovlap.hEvent=hEvent;
	if(!ConnectNamedPipe(hPipe,&ovlap))
	{
		if(ERROR_IO_PENDING!=GetLastError())
		{
			MessageBox("等待客户端连接失败！");
			CloseHandle(hPipe);
			CloseHandle(hEvent);
			hPipe=NULL;
			return;
		}
	}
	if(WAIT_FAILED==WaitForSingleObject(hEvent,INFINITE))
	{
		MessageBox("等待对象失败！");
		CloseHandle(hPipe);
		CloseHandle(hEvent);
		hPipe=NULL;
		return;
	}
	CloseHandle(hEvent);
}

void CNamedPipeSrvView::OnPipeRead() 
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

void CNamedPipeSrvView::OnPipeWrite() 
{
	// TODO: Add your command handler code here
	char buf[]="http://www.sunxin.org";
	DWORD dwWrite;
	if(!WriteFile(hPipe,buf,strlen(buf)+1,&dwWrite,NULL))
	{
		MessageBox("写入数据失败！");
		return;
	}
}
