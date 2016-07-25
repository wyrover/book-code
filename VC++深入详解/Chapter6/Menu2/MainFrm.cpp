// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Menu2.h"

#include "MainFrm.h"
#include "Menu2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_HELLO,OnHello)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

/*	CMenu menu;
	menu.CreateMenu();
//	GetMenu()->AppendMenu(MF_POPUP,(UINT)menu.m_hMenu,"Test");
	GetMenu()->InsertMenu(2,MF_POPUP | MF_BYPOSITION,(UINT)menu.m_hMenu,"Test");
	menu.AppendMenu(MF_STRING,IDM_HELLO,"Hello");
	menu.AppendMenu(MF_STRING,112,"Bye");
	menu.AppendMenu(MF_STRING,113,"Mybole");

	GetMenu()->GetSubMenu(0)->AppendMenu(MF_STRING,114,"Welcome");
	GetMenu()->GetSubMenu(0)->InsertMenu(ID_FILE_OPEN,MF_BYCOMMAND | MF_STRING,115,"VC±à³Ì");
	menu.Detach();
	
	GetMenu()->DeleteMenu(1,MF_BYPOSITION);
	GetMenu()->GetSubMenu(0)->DeleteMenu(2,MF_BYPOSITION);
*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnHello()
{
	MessageBox("Hello");
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int MenuCmdID = LOWORD(wParam);
	CMenu2View *pView = (CMenu2View *)GetActiveView();
	if( MenuCmdID >= IDM_PHONE1 && MenuCmdID < IDM_PHONE1 + pView->m_strArray.GetSize() )
	{
//		MessageBox("Test");
		CClientDC dc(pView);
		dc.TextOut(0,0,pView->m_strArray.GetAt(MenuCmdID - IDM_PHONE1));
		return TRUE;
	}

	return CFrameWnd::OnCommand(wParam, lParam);
}

