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
	menu.CreatePopupMenu();
//	GetMenu()->AppendMenu(MF_POPUP,(UINT)menu.m_hMenu,"WinSun");
	GetMenu()->InsertMenu(2,MF_BYPOSITION | MF_POPUP,(UINT)menu.m_hMenu,"WinSun");
	menu.AppendMenu(MF_STRING,IDM_HELLO,"Hello");
	menu.AppendMenu(MF_STRING,112,"Weixin");
	menu.AppendMenu(MF_STRING,113,"Mybole");
	menu.Detach();
	GetMenu()->GetSubMenu(0)->AppendMenu(MF_STRING,114,"Welcome");
	GetMenu()->GetSubMenu(0)->InsertMenu(ID_FILE_OPEN,
			MF_BYCOMMAND | MF_STRING,115,"Î¬ÐÂ");
	GetMenu()->DeleteMenu(1,MF_BYPOSITION);
	GetMenu()->GetSubMenu(0)->DeleteMenu(2,MF_BYPOSITION);*/
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

void CMainFrame::OnHello()
{
	MessageBox("Hello!");
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


//DEL void CMainFrame::OnEditCut() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int MenuCmdId=LOWORD(wParam);
	CMenu2View *pView=(CMenu2View*)GetActiveView();
	if(MenuCmdId>=IDM_PHONE1 && MenuCmdId<IDM_PHONE1+pView->m_strArray.GetSize())
	{
		CClientDC dc(pView);
		dc.TextOut(0,0,pView->m_strArray.GetAt(MenuCmdId-IDM_PHONE1));
		//MessageBox("Test");
		return TRUE;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}
