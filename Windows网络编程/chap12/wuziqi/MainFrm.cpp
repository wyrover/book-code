// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "wuziqi.h"

#include "MainFrm.h"

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
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LEFT,OnUpdateLeft)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_RIGHT,OnUpdateRight)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME,OnUpdateTime)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TEMP,OnUpdateTemp)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOP, OnUpdateViewTop)
	ON_COMMAND(ID_MENU_HELP, OnMenuHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_SEPARATOR,
//	ID_SEPARATOR,
//	ID_SEPARATOR,// status line indicator
//	ID_INDICATOR_LEFT,
//	ID_INDICATOR_RIGHT,
//	ID_INDICATOR_TIME,
//	ID_INDICATOR_TEMP,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	nTop=0;
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

	if (!m_wndStatusBar.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,ID_MY_STATUS_BAR)||
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
	
	SetIcon(AfxGetApp()->LoadIcon(IDR_MINE),TRUE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style = WS_OVERLAPPED | WS_BORDER   ;
	cs.style &= ~WS_MAXIMIZEBOX; 
	cs.style &=~WS_THICKFRAME;
	cs.cx=500;
	cs.cy=550;
	cs.lpszName="五子棋单机｜网络版";
	
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


void CMainFrame::OnUpdateLeft(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CMainFrame::OnUpdateRight(CCmdUI* pCmdUI)
{

	pCmdUI->Enable(1);

}
void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);

}
void CMainFrame::OnUpdateTemp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);

}

//最小化到状态栏
//wm_user+200 状态栏的消息
LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_USER + 200&&lParam == WM_LBUTTONDBLCLK)
	{//最大化
			NOTIFYICONDATA nid;

	nid.cbSize = sizeof(nid);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MINE);

	strcpy(nid.szTip,"五子棋");
	nid.uCallbackMessage = WM_USER + 200;
	nid.uID = 10;
	nid.hWnd =m_hWnd;
		ShowWindow(SW_SHOWNORMAL);
		BringWindowToTop();
		Shell_NotifyIcon(NIM_DELETE,&nid);
	}
	if(message==WM_USER + 200&&lParam == WM_RBUTTONDOWN)
	{//弹出菜单
		CMenu menu;
			CMenu* pSubMenu = menu.GetSubMenu(0);

		CPoint pos;
			GetCursorPos(&pos);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y, this, NULL);


	}


	if(message==WM_SYSCOMMAND&&wParam==SC_MINIMIZE)
	{//得到最小化消息，最小化到状态栏
		NOTIFYICONDATA nid;

	nid.cbSize = sizeof(nid);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MINE);

	strcpy(nid.szTip,"定时关机V1.0");
	nid.uCallbackMessage = WM_USER + 200;
	nid.uID = 10;
	nid.hWnd =m_hWnd;

	Shell_NotifyIcon(NIM_ADD, &nid);

	ShowWindow(SW_HIDE);
	message=NULL;
	}
	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}

void CMainFrame::OnClose() 
{//退出时删除状态栏图标
			NOTIFYICONDATA nid;

	nid.cbSize = sizeof(nid);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MINE);

	strcpy(nid.szTip,"定时关机V1.0");
	nid.uCallbackMessage = WM_USER + 200;
	nid.uID = 10;
	nid.hWnd =m_hWnd;
		
		Shell_NotifyIcon(NIM_DELETE,&nid);
	CFrameWnd::OnClose();
}


void CMainFrame::OnViewTop() 
{//set to always on top
	if(!nTop)
	{
		SetWindowPos( &wndTopMost,500,550,500,550,SWP_NOMOVE   );
		nTop=1;
	}
	else
	{
		SetWindowPos( &wndNoTopMost  ,500,550,500,550,SWP_NOMOVE   );
		nTop=0;
	}

}

void CMainFrame::OnUpdateViewTop(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(nTop);	
}

void CMainFrame::OnMenuHelp() 
{
HINSTANCE  n=ShellExecute(NULL, "open", "help.htm", NULL,NULL, SW_SHOWNORMAL);	//::WinExec("C:\Program Files\Internet Explorer\IEXPLORE.EXE c:\1.html",SW_SHOW);
		
}
