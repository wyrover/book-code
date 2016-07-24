// ViewAdapterInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ViewAdapterInfo.h"
#include "ViewAdapterInfoDlg.h"
#include "Iphlpapi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
PIP_ADAPTER_INFO pinfo=NULL;
unsigned long len=0;

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
// CViewAdapterInfoDlg dialog

CViewAdapterInfoDlg::CViewAdapterInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewAdapterInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewAdapterInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewAdapterInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewAdapterInfoDlg)
	DDX_Control(pDX, IDC_BUTTONNext, m_buttonnext);
	DDX_Text(pDX, IDC_EDITMacAddress, m_macaddress);
	DDX_Text(pDX, IDC_EDITDescription, m_description);
	DDX_Text(pDX, IDC_EDITType, m_type);
	DDX_Text(pDX, IDC_EDITSubNet, m_subnet);
	DDX_Text(pDX, IDC_EDITIPAddress, m_IpAddress);
	DDX_Text(pDX, IDC_EDITGateway, m_gateway);
	DDX_Text(pDX, IDC_EDITPrimaryWinsServer, m_PrimaryWinsServer);
	DDX_Text(pDX, IDC_EDITDHCP, m_dhcp);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewAdapterInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CViewAdapterInfoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Refresh, OnRefresh)
	ON_BN_CLICKED(IDC_BUTTONNext, OnBUTTONNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAdapterInfoDlg message handlers

BOOL CViewAdapterInfoDlg::OnInitDialog()
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
	
	GetInfo();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CViewAdapterInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CViewAdapterInfoDlg::OnPaint() 
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
HCURSOR CViewAdapterInfoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CViewAdapterInfoDlg::GetInfo()
{
	if (pinfo!=NULL)
		delete (pinfo);
	unsigned  long nError;
	nError	=	GetAdaptersInfo(pinfo,&len);
	if (nError==0)
	{
		ParseData();			
	}
	if (nError==ERROR_NO_DATA)
	{
		AfxMessageBox("No adapter information exists for the local computer");
	}
	if (nError==ERROR_NOT_SUPPORTED)
	{
		AfxMessageBox("GetAdaptersInfo is not supported by the operating system running on the local computer");
	}
	if (nError==ERROR_BUFFER_OVERFLOW)
	{
		pinfo= (PIP_ADAPTER_INFO)malloc(len);
		nError	=	GetAdaptersInfo(pinfo,&len);
		if (nError==0)
		{
			ParseData();			
		}
	}
	
	return;
}
void CViewAdapterInfoDlg::ParseData()
{

	if (pinfo!=NULL)
	{
		m_macaddress.Format("%02X:%02X:%02X:%02X:%02X:%02X",pinfo->Address[0],pinfo->Address[1],pinfo->Address[2],pinfo->Address[3],pinfo->Address[4],pinfo->Address[5]);
		m_description	=	pinfo->Description;
		m_type.Format("%d",pinfo->Type);
		
		 PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);
		 m_IpAddress	="";
		  do 
		  {
			  m_IpAddress	+=	pAddressList->IpAddress.String;
			 pAddressList = pAddressList->Next;
			 if (pAddressList != NULL)
				m_IpAddress	+="\r\n";
		  } while (pAddressList != NULL);

		m_subnet.Format("%s",pinfo->IpAddressList.IpMask.String);
		m_gateway.Format("%s",pinfo->GatewayList.IpAddress.String);
		if (pinfo->HaveWins) 
			m_PrimaryWinsServer.Format("%s",pinfo->PrimaryWinsServer.IpAddress.String );
		else
			m_PrimaryWinsServer.Format("%s","N/A" );
		if (pinfo->DhcpEnabled )
			m_dhcp.Format("%s",pinfo->DhcpServer.IpAddress.String );
		else
			m_dhcp.Format("%s","N/A");
		pinfo = pinfo->Next;
	}
	else
	{
		m_buttonnext.EnableWindow(FALSE);
	}
	UpdateData(FALSE);

}
void CViewAdapterInfoDlg::OnRefresh() 
{
	
	GetInfo();
	m_buttonnext.EnableWindow();

}

void CViewAdapterInfoDlg::OnBUTTONNext() 
{
	ParseData();
}
CViewAdapterInfoDlg::~CViewAdapterInfoDlg()
{
	if (pinfo!=NULL)
		delete (pinfo);	
}