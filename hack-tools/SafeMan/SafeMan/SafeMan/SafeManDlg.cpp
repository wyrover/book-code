// SafeManDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SafeMan.h"
#include "SafeManDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSafeManDlg 对话框

CSafeManDlg::CSafeManDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(CSafeManDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pProcessManager = new CProcessManager();
	m_pSSDTManager = new CSSDTManager();
	m_pMonitorManager = new CMonitorManager();
}

CSafeManDlg::~CSafeManDlg()
{
	if(m_pProcessManager != NULL)
		delete m_pProcessManager;

	if(m_pSSDTManager != NULL)
		delete m_pSSDTManager;

	if(m_pMonitorManager != NULL)
		delete m_pMonitorManager;
}
void CSafeManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PROCESSMANAGER, m_ProcessManager);
	DDX_Control(pDX, IDC_BUTTON_SSDTMANAGER, m_SSDTManager);
	DDX_Control(pDX, IDC_BUTTON_MONITORMANAGER, m_MonitorManager);
	DDX_Control(pDX, IDC_MIN, m_Min);
	DDX_Control(pDX, IDC_EXIT, m_Exit);
	DDX_Control(pDX, IDC_TRAY, m_Tray);
}

BEGIN_MESSAGE_MAP(CSafeManDlg, CDialogSK)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PROCESSMANAGER, &CSafeManDlg::OnBnClickedButtonProcessmanager)
	ON_BN_CLICKED(IDC_BUTTON_SSDTMANAGER, &CSafeManDlg::OnBnClickedButtonSsdtmanager)
	ON_BN_CLICKED(IDC_BUTTON_MONITORMANAGER, &CSafeManDlg::OnBnClickedButtonProcessmonitormanager)
	ON_BN_CLICKED(IDC_MIN, &CSafeManDlg::OnBnClickedMin)
	ON_BN_CLICKED(IDC_EXIT, &CSafeManDlg::OnBnClickedExit)
	ON_COMMAND(ID_MENU_ITEM_SAFEMAN, &CSafeManDlg::OnMenuItemSafeman)
	ON_COMMAND(ID_MENU_ITEM_QUIT, &CSafeManDlg::OnMenuItemQuit)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_BN_CLICKED(IDC_TRAY, &CSafeManDlg::OnBnClickedTray)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CSafeManDlg 消息处理程序
BOOL CSafeManDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 创建圆角对话框
	CRect dialogRect; 
	GetClientRect(&dialogRect); 
	CRgn rgn; 
	rgn.CreateRoundRectRgn(0,0,dialogRect.Width(),dialogRect.Height(),20,20); 
	SetWindowRgn(rgn,TRUE); 

	SetBitmap(IDB_BACKGROUND);

	m_ProcessManager.LoadBitmap(IDB_PROCESSMANAGER);  // 悬停按钮载入位图
	m_SSDTManager.LoadBitmap(IDB_SSDTMANAGER);  // 悬停按钮载入位图
	m_MonitorManager.LoadBitmap(IDB_MONITORMANAGER);  // 悬停按钮载入位图
	m_Min.LoadBitmap(IDB_MIN);
	m_Exit.LoadBitmap(IDB_EXIT);
	m_Tray.LoadBitmap(IDB_TRAY);

	m_ProcessManager.SetState(TRUE);
	m_SSDTManager.SetState(FALSE);
	m_MonitorManager.SetState(FALSE);

	m_pProcessManager->Create(IDD_PROCESSMANAGER, this);
	m_pSSDTManager->Create(IDD_SSDTMANAGER, this);
	m_pMonitorManager->Create(IDD_MONITORMANAGER, this);

	m_pProcessManager->ShowWindow(SW_SHOW);  
	m_pSSDTManager->ShowWindow(SW_HIDE);  
	m_pMonitorManager->ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSafeManDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogSK::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CSafeManDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSafeManDlg::OnBnClickedButtonProcessmanager()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ProcessManager.SetState(TRUE);
	m_SSDTManager.SetState(FALSE);
	m_MonitorManager.SetState(FALSE);
	
	m_pProcessManager->ShowWindow(SW_SHOW); 
	m_pSSDTManager->ShowWindow(SW_HIDE);  
	m_pMonitorManager->ShowWindow(SW_HIDE);
}

void CSafeManDlg::OnBnClickedButtonSsdtmanager()
{
	// TODO: 在此添加控件通知处理程序代码

	m_SSDTManager.SetState(TRUE);
	m_ProcessManager.SetState(FALSE);
	m_MonitorManager.SetState(FALSE);

	m_pSSDTManager->ShowWindow(SW_SHOW); 
	m_pProcessManager->ShowWindow(SW_HIDE);  
	m_pMonitorManager->ShowWindow(SW_HIDE);
}

void CSafeManDlg::OnBnClickedButtonProcessmonitormanager()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MonitorManager.SetState(TRUE);
	m_ProcessManager.SetState(FALSE);
	m_SSDTManager.SetState(FALSE);

	m_pMonitorManager->ShowWindow(SW_SHOW); 
	m_pProcessManager->ShowWindow(SW_HIDE);  
	m_pSSDTManager->ShowWindow(SW_HIDE);
}


void CSafeManDlg::OnBnClickedMin()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}

void CSafeManDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pProcessManager != NULL)
		delete m_pProcessManager;
	if(m_pSSDTManager != NULL)
		delete m_pSSDTManager;
	if(m_pMonitorManager != NULL)
		delete m_pMonitorManager;
	exit(0);
}

void CSafeManDlg::OnMenuItemSafeman()
{
	// TODO: 在此添加命令处理程序代码

	m_TrayIcon.RemoveIcon();
	ShowWindow(SW_SHOW);
}

void CSafeManDlg::OnMenuItemQuit()
{
	// TODO: 在此添加命令处理程序代码

	m_TrayIcon.RemoveIcon();
	OnCancel();
}


// 托盘消息
//
LRESULT CSafeManDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam,lParam);
}


void CSafeManDlg::OnBnClickedTray()
{
	// TODO: 在此添加控件通知处理程序代码

	m_TrayIcon.Create(this,WM_ICON_NOTIFY,_T("SafeMan正在运行中"),m_hIcon,IDR_MENU_TRAY);  // 构造
	ShowWindow(SW_HIDE);  // 隐藏窗口
}


// 设置鼠标样式
//
BOOL CSafeManDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strClassName;
	GetClassName(pWnd->GetSafeHwnd(), strClassName.GetBuffer(80), 80);

	if (strClassName == "Button")  // 按钮类型
	{
		SetCursor( LoadCursor(NULL, IDC_HAND) );  // 鼠标样式为手型
		return TRUE;
	}
	return CDialogSK::OnSetCursor(pWnd, nHitTest, message);
}
