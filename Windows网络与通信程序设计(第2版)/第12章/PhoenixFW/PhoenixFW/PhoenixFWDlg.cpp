// PhoenixFWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "PhoenixFWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


extern CPhoenixApp theApp;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(PM_QUERY_ACL_NOTIFY, OnQueryAcl)
	ON_MESSAGE(PM_SESSION_NOTIFY, OnSessionNotify)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_ANNUL, OnAnnul)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


	// 设置共享数据
	TCHAR sz[256];
	::GetModuleFileName(NULL, sz, 256);
	g_pIoControl->SetPhoenixInstance(m_hWnd, sz);

	// 向属性页中加入标签，标签名由各个子对话框的标题栏决定
	m_sheet.AddPage(&m_MonitorPage);
	m_sheet.AddPage(&m_RulePage);
	m_sheet.AddPage(&m_KerRulePage);
	m_sheet.AddPage(&m_SyssetPage);

	// 用Create来创建一个属性页
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);
		
	// 调整属性页的大小和位置
	RECT rect;
	GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	m_sheet.SetWindowPos(NULL, 
		0, 0, width - 4, height - 25, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	// 无效“应用”按钮
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


long CMainDlg::OnQueryAcl(WPARAM wParam, LPARAM lParam)		// 处理PM_QUERY_ACL_NOTIFY消息
{
	// 获取询问的应用程序名称
	LPCTSTR lpszApp = g_pIoControl->GetQueryApp(wParam);

	if(CRulePage::AddQueryRule(lpszApp))
		g_pIoControl->SetQueryApp(wParam, 1); // 放行
	else 
		g_pIoControl->SetQueryApp(wParam, 0); // 拒绝

	return 0; 
}

long CMainDlg::OnSessionNotify(WPARAM wParam, LPARAM lParam)	// 处理PM_SESSION_NOTIFY消息
{
	SESSION session;
	// 获取DLL发送来的Session
	g_pIoControl->GetSession(&session, wParam);
	// 将Session传递给CMonitorPage类
	CMonitorPage::HandleNotifySession(&session, lParam);
	return 0;
}

void CMainDlg::OnApply()		// 用户点击“应用”按钮
{
	if(!GetDlgItem(IDC_APPLY)->IsWindowEnabled())
		return;

	// 将用户的设置保存到文件
	if(!g_RuleFile.SaveRules())
	{
		MessageBox(L"保存规则出错。");
		return;
	}
	// 将文件中的数据应用到DLL模块和SYS模块
	if(!theApp.ApplyFileData())
		return;

	// 设置开机自动启动
	theApp.SetAutoStart(g_RuleFile.m_header.bAutoStart);

	// 无效本按钮
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void CMainDlg::OnAnnul()	// 用户点击“取消”按钮
{	
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox(L"要保存所做的修改吗？", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	// 最小化窗口
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CMainDlg::OnOK()		// 用户点击“确定”按钮
{
	// 应用用户设置
	OnApply(); 
	// 最小话窗口
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CMainDlg::OnCancel() 	// 用户关闭对话框
{
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox(L"要保存所做的修改吗？", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}

	CDialog::OnCancel();
}