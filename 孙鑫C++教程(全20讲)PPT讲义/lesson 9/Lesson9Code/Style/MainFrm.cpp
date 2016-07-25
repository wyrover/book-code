// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Style.h"

#include "MainFrm.h"
#include "Splash.h"

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
	ON_WM_TIMER()
	ON_COMMAND(IDM_TEST, OnTest)
	ON_COMMAND(IDM_VIEW_NEWTOOL, OnViewNewtool)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_NEWTOOL, OnUpdateViewNewtool)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_PROGRESS,OnProgress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_TIMER,
	IDS_PROGRESS,
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

extern CStyleApp theApp;
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

	//SetWindowLong(m_hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
	//SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX);
//	SetClassLong(m_hWnd,GCL_HICON,(LONG)LoadIcon(NULL,IDI_ERROR));
	m_hIcons[0]=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1));
	m_hIcons[1]=LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDI_ICON2));
	m_hIcons[2]=LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON3));

	SetClassLong(m_hWnd,GCL_HICON,(LONG)m_hIcons[0]);

	SetTimer(1,1000,NULL);

	if (!m_newToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_newToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_newToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_newToolBar);

	CTime t=CTime::GetCurrentTime();
	CString str=t.Format("%H:%M:%S");
	CClientDC dc(this);
	CSize sz=dc.GetTextExtent(str);
	int index=0;
	index=m_wndStatusBar.CommandToIndex(IDS_TIMER);
	m_wndStatusBar.SetPaneInfo(index,IDS_TIMER,SBPS_NORMAL,sz.cx);
	m_wndStatusBar.SetPaneText(index,str);

/*	CRect rect;
	m_wndStatusBar.GetItemRect(2,&rect);
	//m_progress.Create(WS_CHILD | WS_VISIBLE | PBS_VERTICAL,
	//	CRect(100,100,120,200),this,123);
	m_progress.Create(WS_CHILD | WS_VISIBLE,// | PBS_VERTICAL,
		rect,&m_wndStatusBar,123);
	m_progress.SetPos(50);*/
//	SendMessage(UM_PROGRESS);
//	PostMessage(UM_PROGRESS);
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.cx=300;
//	cs.cy=200;
	//cs.style&=~FWS_ADDTOTITLE;
	//cs.style=WS_OVERLAPPEDWINDOW;
	//cs.lpszName="http://www.sunxin.org";
	/*WNDCLASS wndcls;
	wndcls.cbClsExtra=0;
	wndcls.cbWndExtra=0;
	wndcls.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wndcls.hCursor=LoadCursor(NULL,IDC_HELP);
	wndcls.hIcon=LoadIcon(NULL,IDI_ERROR);
	wndcls.hInstance=AfxGetInstanceHandle();
	wndcls.lpfnWndProc=::DefWindowProc;
	wndcls.lpszClassName="sunxin.org";
	wndcls.lpszMenuName=NULL;
	wndcls.style=CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndcls);

	cs.lpszClass="sunxin.org";*/
	//cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,0,0,
	//	LoadIcon(NULL,IDI_WARNING));
	//cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
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


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int index=1;
	SetClassLong(m_hWnd,GCL_HICON,(LONG)m_hIcons[index]);
	index=++index%3;

	CTime t=CTime::GetCurrentTime();
	CString str=t.Format("%H:%M:%S");
	CClientDC dc(this);
	CSize sz=dc.GetTextExtent(str);
	m_wndStatusBar.SetPaneInfo(1,IDS_TIMER,SBPS_NORMAL,sz.cx);
	m_wndStatusBar.SetPaneText(1,str);

	m_progress.StepIt();

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnTest() 
{
	// TODO: Add your command handler code here
	MessageBox("test");
}

void CMainFrame::OnViewNewtool() 
{
	// TODO: Add your command handler code here
	/*if(m_newToolBar.IsWindowVisible())
	{
		m_newToolBar.ShowWindow(SW_HIDE);
	}
	else
	{
		m_newToolBar.ShowWindow(SW_SHOW);
	}
	RecalcLayout();
	DockControlBar(&m_newToolBar);*/

	ShowControlBar(&m_newToolBar,!m_newToolBar.IsWindowVisible(),FALSE);
}

void CMainFrame::OnUpdateViewNewtool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_newToolBar.IsWindowVisible());
}

void CMainFrame::OnProgress()
{
	CRect rect;
	m_wndStatusBar.GetItemRect(2,&rect);
	m_progress.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		rect,&m_wndStatusBar,123);
	m_progress.SetPos(50);
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	m_wndStatusBar.GetItemRect(2,&rect);
	if(!m_progress.m_hWnd)
		m_progress.Create(WS_CHILD | WS_VISIBLE ,//| PBS_SMOOTH,
			rect,&m_wndStatusBar,123);
	else
		m_progress.MoveWindow(rect);
	m_progress.SetPos(50);
	// Do not call CFrameWnd::OnPaint() for painting messages
}
