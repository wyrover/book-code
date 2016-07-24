// ProxyServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProxyServer.h"
#include "ProxyServerDlg.h"

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
// CProxyServerDlg dialog

CProxyServerDlg::CProxyServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProxyServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxyServerDlg)
	m_bSecondProxy = FALSE;
	m_nPort = 2000;
	m_nProxyPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProxyServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxyServerDlg)
	DDX_Control(pDX, IDC_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_PROXYADDR, m_editProxyAddr);
	DDX_Control(pDX, IDC_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_INFO, m_editInfo);
	DDX_Check(pDX, IDC_SECONDPROXY, m_bSecondProxy);
	DDX_Text(pDX, IDC_SERVERPORT, m_nPort);
	DDX_Text(pDX, IDC_PROXYPORT, m_nProxyPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProxyServerDlg, CDialog)
	//{{AFX_MSG_MAP(CProxyServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SECONDPROXY, OnSecondproxy)
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
	ON_MESSAGE(AGM_OUTPUT, OnOutput)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxyServerDlg message handlers

BOOL CProxyServerDlg::OnInitDialog()
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
	
	m_editPassword.EnableWindow(FALSE);
	m_editProxyAddr.EnableWindow(FALSE);
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PROXYPORT);
    pEdit->EnableWindow(FALSE);
    m_editUsername.EnableWindow(FALSE);
	
	WSADATA data;
	int iRet;

	iRet = WSAStartup(MAKEWORD(2,2), &data);
	m_proxyServer.m_hConnectionCount = CreateSemaphore(NULL, 1, 1, NULL);
	m_dwCount = 0;

	m_pFile = fopen("c:\\ProxyServer\\proxy.log", "ab");
    
	BYTE b1, b2, b3, b4;
	int nRet;
	FILE * pFile = fopen("c:\\ProxyServer\\refuse.txt", "r");
	while(!feof(pFile))
	{
		nRet = fscanf(pFile, "%d.%d.%d.%d", &b1, &b2, &b3, &b4);
		if(nRet !=4 )break;

		m_proxyServer.m_ulIpRefused[m_proxyServer.m_nRefused] = b4<<24 | b3<<16 | b2<<8 | b1;
		m_proxyServer.m_nRefused;
	}
	// HKEY_CURRENT_USER\Software\Max Studio
	HKEY hKey;
	DWORD dwType, dwSize;
	char pszPassword[16];

	dwSize = 16;
	dwType = REG_SZ;
	if(RegCreateKey(HKEY_CURRENT_USER, "Software\\Max Studio\\ProxyServer", &hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey, "pszPassword", 0, &dwType, (BYTE*)pszPassword, &dwSize)!=ERROR_SUCCESS)
		{
			m_strPasswd = "66";
		}
		{
			m_strPasswd = (CString)pszPassword;
		}
		RegCloseKey(hKey);
	}

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProxyServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProxyServerDlg::OnPaint() 
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
HCURSOR CProxyServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CProxyServerDlg::OnSecondproxy() 
{
   if (!m_bSecondProxy)
   {
	   m_editPassword.EnableWindow(TRUE);
	   m_editProxyAddr.EnableWindow(TRUE);
	   CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PROXYPORT);
	   pEdit->EnableWindow(TRUE);
	   m_editUsername.EnableWindow(TRUE);
	   m_bSecondProxy = TRUE;
   }
   else
   {
	   m_editPassword.EnableWindow(FALSE);
	   m_editProxyAddr.EnableWindow(FALSE);
	   CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PROXYPORT);
	   pEdit->EnableWindow(FALSE);
	   m_editUsername.EnableWindow(FALSE);
	   m_bSecondProxy = FALSE;
   }
  
}

void CProxyServerDlg::OnStart() 
{
	UpdateData();
	CString strTemp1,strTemp2,strTemp3;
	if (m_bSecondProxy)
	{
		
		m_editProxyAddr.GetWindowText(strTemp1);
		m_editUsername.GetWindowText(strTemp2);
		m_editPassword.GetWindowText(strTemp3);	
	}
	m_proxyServer.m_Info.strProxyAddr = strTemp1;
	m_proxyServer.m_Info.strUser = strTemp2;
	m_proxyServer.m_Info.strPasswd = strTemp3;
	m_proxyServer.m_Info.hWnd = GetSafeHwnd();
	m_proxyServer.m_Info.nPort = m_nPort;	// local
	m_proxyServer.m_Info.nPort2nd = m_nProxyPort;
	AfxBeginThread(CProxyServer::ListenThread, &m_proxyServer, THREAD_PRIORITY_BELOW_NORMAL);
	
}

void CProxyServerDlg::OnOutput(WPARAM wParam, LPARAM lParam)
{
    char* pszStr = (char*)lParam;
	CString pszTemp;
	m_editInfo.GetWindowText(pszTemp);
	CString str;
	str.Format("%s%s",pszTemp.GetBuffer(20),pszStr);
    pszTemp.ReleaseBuffer();
//	m_dwCount += dwCount;
	
	m_editInfo.ReplaceSel(str);
//	TRACE(pszStr);
	fputs(pszStr, m_pFile);
	fflush(m_pFile);

}

