

#include "stdafx.h"
#include "AntiSpy.h"
#include "AntiSpyDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE  g_hDevice = NULL;
#define DRIVER_NAME L"AntiSpy"
#define DRIVER_PATH L"AntiSpy.sys"	
#define LINK_NAME   L"\\\\.\\AntiSpyDeviceLinkName"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}



void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CAntiSpyDlg::CAntiSpyDlg(CWnd* pParent)
	: CDialogEx(CAntiSpyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	WCHAR wzFilePath[MAX_PATH];
	WCHAR* wzPoint = NULL;
	GetModuleFileName(NULL, wzFilePath, MAX_PATH);
	wzPoint = wcsrchr(wzFilePath,'\\');

	if(wzPoint!=NULL)
	{
		*(wzPoint + 1) = '\0';
		wcscat(wzFilePath,L"AntiSpy.sys");
	}

	if(FreeSource(MAKEINTRESOURCE(IDR_SYS),L"SYS",wzFilePath)==TRUE)
	{
		if (LoadNTDriver(DRIVER_NAME,DRIVER_PATH)==TRUE)
		{
			DeleteFile(wzFilePath);
		}
		
	}

	g_hDevice = OpenDevice(LINK_NAME);

	if (g_hDevice==INVALID_HANDLE_VALUE)
	{
		MessageBox(L"OpenDeviceFailed");
		return;
	}
}

CAntiSpyDlg::~CAntiSpyDlg()
{
	UnloadNTDriver(DRIVER_NAME);
	
}

void CAntiSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CAntiSpyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CAntiSpyDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


HANDLE CAntiSpyDlg::OpenDevice(LPCTSTR wzLinkPath)
{
	HANDLE hDevice = CreateFile(wzLinkPath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	return hDevice;
}


VOID CAntiSpyDlg::InitControl()
{
	m_Tab.InsertItem(ProcessManager,L"进程管理");
	m_Tab.InsertItem(DriverModule,L"驱动模块");

	m_ProcessManagerDlg.Create(IDD_DLG_PROCESS_MANAGER,&m_Tab);
	m_DriverModuleDlg.Create(IDD_DLG_DRIVER_MODULE,&m_Tab);

	m_Dlg[0] = &m_ProcessManagerDlg;
	m_Dlg[1] = &m_DriverModuleDlg;

	CRect rc;
	m_Tab.GetClientRect(rc);
	rc.top +=20;
	rc.bottom -= 1;
	rc.left += 2;
	rc.right += 10;
	m_ProcessManagerDlg.MoveWindow(rc);
	m_DriverModuleDlg.MoveWindow(rc);
	m_ProcessManagerDlg.ShowWindow(SW_SHOW);
}

BOOL CAntiSpyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	


	

	InitControl();
	return TRUE;  
}



void CAntiSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



void CAntiSpyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CAntiSpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAntiSpyDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	for(int i=0;i<2;i++)
	{
		if (m_Dlg[i]!=NULL)
		{
			m_Dlg[i]->ShowWindow(SW_HIDE);
		}

	}
	m_CurSelTab = m_Tab.GetCurSel();


	if (m_Dlg[m_CurSelTab]!=NULL)
	{
		m_Dlg[m_CurSelTab]->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


//装载NT驱动程序

BOOL CAntiSpyDlg::LoadNTDriver(WCHAR* lpszDriverName,WCHAR* lpszDriverPath)
{
	WCHAR szDriverImagePath[256];
	//得到完整的驱动路径
	GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄

	//打开服务控制管理器
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if(hServiceMgr==NULL)  
	{
		bRet = FALSE;
		goto BeforeLeave;
	}

	//创建驱动所对应的服务

	hServiceDDK = CreateServiceW( hServiceMgr,
		lpszDriverName,         //驱动程序在注册表中的名字  
		lpszDriverName,        // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS,    // 加载驱动程序的访问权限
		SERVICE_KERNEL_DRIVER, // 表示加载的服务是驱动程序
		SERVICE_DEMAND_START,  // 注册表驱动程序的Start 值  
		SERVICE_ERROR_IGNORE,  // 注册表括驱动程序的 ErrorControl 值  
		szDriverImagePath,     // 注册表驱动程序的ImagePath值  
		NULL,  
		NULL,  
		NULL,  
		NULL,  
		NULL);   
	DWORD dwRtn;

	//判断服务是否失败

	if( hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();

		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			bRet = FALSE;
			goto BeforeLeave;
		}  

		hServiceDDK = OpenService( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS );  

		if( hServiceDDK == NULL )  
		{
			bRet = FALSE;
			goto BeforeLeave;
		}  
	}  
	
	//开启此项服务
	bRet= StartService( hServiceDDK, NULL, NULL );  

	if( !bRet )  
	{  
		DWORD dwRtn = GetLastError();  
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{   
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				//设备被挂住
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  
			{  
				//服务已经开启
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}
	bRet = TRUE;
BeforeLeave:

	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}

	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;
}


//卸载驱动程序  
BOOL CAntiSpyDlg::UnloadNTDriver(WCHAR* szSvrName)  
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;  //SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;  //NT驱动程序的服务句柄
	SERVICE_STATUS SvrSta;

	//打开SCM管理器
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( hServiceMgr == NULL )  
	{
		//打开SCM管理器失败
		bRet = FALSE;
		goto BeforeLeave;
	}  
	
	//打开驱动所对应的

	hServiceDDK = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS );  

	if( hServiceDDK == NULL )  
	{
		bRet = FALSE;
		goto BeforeLeave;
	}  

	DeleteService( hServiceDDK ); 

	//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载 
	if(ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta )==TRUE)
	{
		//动态卸载驱动程序
		MessageBox(L"Sucess");
		 
	}

	bRet = TRUE;
BeforeLeave:
	
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;	
}



BOOL CAntiSpyDlg::FreeSource(LPCTSTR pwzResName,LPCTSTR pwzResType,LPCTSTR wzFileName) 
{ 
	BOOL bRet = TRUE; 
	HINSTANCE hInst = GetModuleHandle(NULL); 

	//判断指定的资源是否存在 
	HRSRC hResInfo = FindResource(hInst,pwzResName,pwzResType);

	if(hResInfo == NULL) 
		return FALSE; 

	HANDLE hFile = NULL; 
	DWORD dwWritten = 0; 

	//调入指定的资源到内存 
	HGLOBAL hResData  = LoadResource(hInst,hResInfo); 
	LPVOID  pvResData = LockResource(hResData); 
	DWORD  dwResSize  = SizeofResource(hInst,hResInfo); 

	//创建目标文件并保存数据至文件 
	hFile = CreateFile( 
		wzFileName, 
		GENERIC_WRITE|GENERIC_READ, 
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_HIDDEN ,NULL 
		); 

	if(WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL))
	{
		CloseHandle(hFile); 
		FreeResource(hResData); 
		return TRUE;
	}

	return FALSE; 

} 
