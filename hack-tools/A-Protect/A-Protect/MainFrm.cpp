
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "A-Protect.h"
#include "AboutDlg.h"
#include "A-ProtectView.h"
#include "C3600Splash.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	//ON_COMMAND(ID_Exit, &CMainFrame::OnExit)
	//ON_COMMAND(ID_About, &CMainFrame::OnAbout)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_WindowsOverhead, &CMainFrame::OnWindowsoverhead)
	ON_COMMAND(ID_CancelTheOverhead, &CMainFrame::OnCanceltheoverhead)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
    C3600Splash wndSplash;                 //创建启动窗口类的实例  
	wndSplash.Create(IDB_SPLASH);  
	wndSplash.CenterWindow();  
	wndSplash.UpdateWindow();          //send WM_PAINT  
	Sleep(2000);  
	wndSplash.DestroyWindow();//销毁初始画面窗口  

	//RECT rect1;
	//GetClientRect(&rect1);
	//MoveWindow(400,200,rect1.right-rect1.left,rect1.bottom-rect1.top);
	HICON m_hIcon;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE);
	SetWindowTextW(_T("A盾电脑防护 LE 2012-0.4.3"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style   &=   ~WS_MAXIMIZEBOX;
	cs.style&=~FWS_ADDTOTITLE;
	cs.style &= ~WS_THICKFRAME;//使窗口不能用鼠标改变大小

	//cs.x=500;
	//cs.y=200;
	cs.cx = 1900;
	cs.cy = 1700;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	return 0;
}

void CMainFrame::OnClose()
{
	if (MessageBoxW(L"退出之后，A盾电脑防护 将无法保护内核安全，同时无法防御木马对系统的攻击行为，确定要退出吗？",L"A盾电脑防护", MB_ICONINFORMATION|MB_YESNO) == IDYES)
	{
		DWORD dwReadByte;
		ReadFile((HANDLE)EXIT_PROCESS,0,0,&dwReadByte,0);
		Sleep(2000);
		exit(0);
		//ExitProcess(0);
	}
}
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nID==SC_MINIMIZE)
	{
		//托盘代码
		NOTIFYICONDATA nid;
		nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
		nid.hWnd=this->m_hWnd;
		nid.uID=IDR_MAINFRAME;
		nid.uFlags=NIF_INFO|NIF_ICON|NIF_MESSAGE|NIF_TIP;
		nid.dwInfoFlags=NIIF_USER;
		nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称
		nid.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
		lstrcpy(nid.szTip,_T("A盾电脑防护 正在保护您的系统..."));
		lstrcpy(nid.szInfoTitle,_T("A盾电脑防护 "));
		lstrcpy(nid.szInfo,_T("A盾电脑防护 正在保护您的系统..."));
		Shell_NotifyIcon(NIM_ADD,&nid);
		ShowWindow(SW_HIDE);
	}
	else
		CFrameWndEx::OnSysCommand(nID, lParam);
}

LRESULT CMainFrame::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if((lParam == WM_RBUTTONUP) || (lParam == WM_LBUTTONUP))  
    {  
        ModifyStyleEx(0,WS_EX_TOPMOST);
		NOTIFYICONDATA nid;
		nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
		nid.hWnd=this->m_hWnd;
		nid.uID=IDR_MAINFRAME;
		nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
		nid.dwInfoFlags=NIIF_USER;
		nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称
		nid.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
		lstrcpy(nid.szTip,TEXT("A盾电脑防护 "));
		Shell_NotifyIcon(NIM_DELETE,&nid);
        ShowWindow(SW_SHOWDEFAULT);
    } 
	return 0;
}

void CMainFrame::OnWindowsoverhead()
{
	// TODO: 在此添加命令处理程序代码
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	AfxMessageBox(L"设置窗口顶置成功!");
}


void CMainFrame::OnCanceltheoverhead()
{
	// TODO: 在此添加命令处理程序代码
	::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	AfxMessageBox(L"取消窗口顶置成功!");
}
