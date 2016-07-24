// QueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxinet.h"
#include "WebQuery.h"
#include "QueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CQueryDlg dialog

CQueryDlg::CQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQueryDlg)
	m_strHost = _T("");
	m_nType = -1;
	m_strName = _T("");
	m_strPwd = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueryDlg)
	DDX_Control(pDX, IDC_EDIT_PWD, m_EditPwd);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_STATIC_PWD, m_StaPwd);
	DDX_Control(pDX, IDC_STATIC_NAME, m_StaName);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFile);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_nType);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CQueryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnQuery)
	ON_BN_CLICKED(IDC_RADIO_FTP, OnRadioType)
	ON_BN_CLICKED(IDC_RADIO_TYPE, OnRadioType)
	ON_BN_CLICKED(IDC_RADIO_GOPHER, OnRadioType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueryDlg message handlers

BOOL CQueryDlg::OnInitDialog()
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
	//缺省的Internet查询类型
	m_nType=0;
	//Internet域名
	m_strHost=_T("");
	//FTP用户名
	m_strName=_T("");
	//FTP用户口令
	m_strPwd=_T("");
	//更新界面显示
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQueryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQueryDlg::OnPaint() 
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
HCURSOR CQueryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQueryDlg::OnQuery() 
{
	//获得当前输入
	UpdateData(TRUE);

	//清除列表框原来的显示内容
	while(m_ListFile.GetCount()!=0)
		m_ListFile.DeleteString(0);

	//根据用户选择执行不同的查询函数
	switch (m_nType)
	{
		//HTTP查询
	case 0:
		TryHttpSite();
		break;
		//FTP查询
	case 1:
		TryFtpSite();
		break;
		//Gopher查询
	case 2:
		TryGopherSite();
		break;
	default:
		AfxMessageBox("无效的选择！",MB_OK|MB_ICONSTOP);
		break;
	}
}

void CQueryDlg::TryHttpSite()
{
	CInternetSession* pSession;
	CString head="http://";
	CInternetFile* pFile=NULL;

	//创建CInternetSession类的对象
	pSession=new CInternetSession;
	//显示信息
	m_ListFile.AddString("开始HTTP查询...");
	m_ListFile.AddString("试图连接HTTP站点: "+m_strHost);
	
	try
	{
		//打开HTTP站点
		pFile=(CInternetFile*)pSession->OpenURL(head+m_strHost);
	}
	catch (CInternetException* e)
	{
		//错误处理
		pFile=NULL;
		e->Delete();
	}

	if (pFile)
	{
		//获取文件成功
		m_ListFile.AddString("与HTTP站点的连接已经建立。");
		m_ListFile.AddString("");
		m_ListFile.AddString("文件内容：");

		CString strLine;
		//显示文件代码
		for (int i=0;pFile->ReadString(strLine);i++)
			m_ListFile.AddString(strLine);
		//关闭文件
		pFile->Close();
		//删除文件对象
		delete pFile;
	}
	else
	{
		//获取文件错误
		m_ListFile.AddString("无法连接HTTP服务器。");
	}

	m_ListFile.AddString("----结束----");
}

void CQueryDlg::TryFtpSite()
{
	CInternetSession* pSession;
	CFtpConnection* pConnection=NULL;

	//创建CInternetSession类的对象
	pSession=new CInternetSession;
	//显示信息
	m_ListFile.AddString("开始FTP查询...");
	m_ListFile.AddString("试图连接FTP站点: "+m_strHost);
	
	try
	{
		//建立与FTP服务器的连接
		pConnection=pSession->GetFtpConnection(m_strHost,m_strName,m_strPwd);
	}
	catch (CInternetException* e)
	{
		//错误处理
		pConnection=NULL;
		e->Delete();
	}

	if (pConnection)
	{
		//成功建立FTP连接
		m_ListFile.AddString("与FTP站点的连接已经建立。");
		m_ListFile.AddString("");
		CString strLine;
		//获得FTP服务器缺省目录名
		pConnection->GetCurrentDirectory(strLine);
		m_ListFile.AddString("FTP服务器缺省目录: "+strLine);
		//关闭与FTP服务器的连接
		pConnection->Close();
		//删除连接类对象
		delete pConnection;
	}
	else
	{
		//建立FTP连接失败
		m_ListFile.AddString("无法连接FTP服务器！");
	}

	m_ListFile.AddString("----结束----");
}

void CQueryDlg::TryGopherSite()
{
	CInternetSession* pSession;
	CGopherConnection* pConnection=NULL;

	//创建CInternetSession类的对象
	pSession=new CInternetSession;
	//显示信息
	m_ListFile.AddString("开始Gopher查询...");
	m_ListFile.AddString("试图连接Gopher站点: "+m_strHost);

	try
	{
		//建立与Gopher服务器的连接
		pConnection=pSession->GetGopherConnection(m_strHost,m_strName,m_strPwd);
	}
	catch (CInternetException* e)
	{
		//错误处理
		pConnection=NULL;
		e->Delete();
	}

	if (pConnection)
	{
		//成功建立Gopher连接
		m_ListFile.AddString("与Gopher站点的连接已经建立。");
		m_ListFile.AddString("");

		CString strLine;
		CString strTemp;
		//获得Gopher服务器特定目录var的Gopher地址
		CGopherLocator locator=pConnection->CreateLocator(m_strName,strTemp,GOPHER_TYPE_DIRECTORY);
		strLine=locator;
		m_ListFile.AddString("目录的Gopher地址: "+strLine);
		//关闭与FTP服务器的连接
		pConnection->Close();
		//删除连接类对象
		delete pConnection;
	}
	else
	{
		//建立Gopher连接失败
		m_ListFile.AddString("无法连接Gopher服务器！");
	}

	m_ListFile.AddString("----结束----");
}

void CQueryDlg::OnRadioType() 
{
	//获得当前输入
	UpdateData(TRUE);
	
	//HTTP查询和Gopher查询采用匿名登录方式
	//客户名和口令输入控件设为禁用状态
	if (m_nType==0)
	{
		m_strName=_T("");
		m_strPwd=_T("");
		m_StaName.EnableWindow(FALSE);
		m_StaPwd.EnableWindow(FALSE);
		m_EditName.EnableWindow(FALSE);
		m_EditPwd.EnableWindow(FALSE);
		//更改文本内容
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("用户名：");
	}
	//FTP查询，支持匿名和非匿名的登录方式
	//客户名和口令输入控件设为激活状态
	else if (m_nType==1)
	{
		m_strName="anonymous";
		m_strPwd=_T("");
		m_StaName.EnableWindow(TRUE);
		m_StaPwd.EnableWindow(TRUE);
		m_EditName.EnableWindow(TRUE);
		m_EditPwd.EnableWindow(TRUE);
		//更改文本内容
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("用户名：");
	}
	else if (m_nType==2)
	{
		m_strName=_T("");
		m_strPwd=_T("");
		m_StaName.EnableWindow(TRUE);
		m_StaPwd.EnableWindow(FALSE);
		m_EditName.EnableWindow(TRUE);
		m_EditPwd.EnableWindow(FALSE);
		//更改文本内容
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("目录名：");
	}

	//更新界面显示
	UpdateData(FALSE);
}
