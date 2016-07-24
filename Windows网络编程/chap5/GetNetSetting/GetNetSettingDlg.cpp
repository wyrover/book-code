// GetNetSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetNetSetting.h"
#include "GetNetSettingDlg.h"


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
// CGetNetSettingDlg dialog

CGetNetSettingDlg::CGetNetSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetNetSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetNetSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetNetSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetNetSettingDlg)
	DDX_Control(pDX, IDC_NETCARDLIST, m_listNetcard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetNetSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CGetNetSettingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_QUERYINFO, OnQueryinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetNetSettingDlg message handlers

BOOL CGetNetSettingDlg::OnInitDialog()
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
	
	m_clsNetSetting.GetSetting();
	int nTotalcards = m_clsNetSetting.m_TotalNetCards;
	for ( int i = 0 ; i < nTotalcards;i++)
	{
		m_listNetcard.AddString(m_clsNetSetting.m_Cards[i].szDescription);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGetNetSettingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetNetSettingDlg::OnPaint() 
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
HCURSOR CGetNetSettingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGetNetSettingDlg::OnQueryinfo() 
{
   	int nIndex = m_listNetcard.GetCurSel();
	if (nIndex == LB_ERR)
	{
		AfxMessageBox("Please select a netcard!");
	}
	dlg.Create(IDD_CARDINFO);
	dlg.SetWindowText(m_clsNetSetting.m_Cards[nIndex].szDescription);
	dlg.m_staticHostName.SetWindowText(m_clsNetSetting.m_szHostName);
	dlg.m_staticDomain.SetWindowText(m_clsNetSetting.m_szDomain);
	dlg.m_staticIPAddress.SetWindowText(m_clsNetSetting.m_Cards[nIndex].szIpAddress);

	if (m_clsNetSetting.m_EnableDNS == 0)
		dlg.m_staticIsDns.SetWindowText("否");
	if (m_clsNetSetting.m_EnableDNS == 1)
		dlg.m_staticIsDns.SetWindowText("是");
	else 
        dlg.m_staticIsDns.SetWindowText("未知");
	
	if (m_clsNetSetting.m_IPEnableRouter)
		dlg.m_staticIsRoute.SetWindowText("是");
	else
		dlg.m_staticIsRoute.SetWindowText("否");

	
    
	TCHAR szAddr[128];
	wsprintf(szAddr,_T("%02x-%02x-%02x-%02x-%02x-%02x"),m_clsNetSetting.m_Cards[nIndex].szMacAddr[0],
		m_clsNetSetting.m_Cards[nIndex].szMacAddr[1],m_clsNetSetting.m_Cards[nIndex].szMacAddr[2],
		m_clsNetSetting.m_Cards[nIndex].szMacAddr[3],m_clsNetSetting.m_Cards[nIndex].szMacAddr[4],
		m_clsNetSetting.m_Cards[nIndex].szMacAddr[5]);
	_tcsupr(szAddr);
	dlg.m_addrMAC.SetWindowText(szAddr);
    
	dlg.m_listctrlIP.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    dlg.m_listctrlIP.InsertColumn(0,_T("IP地址"),LVCFMT_IMAGE|LVCFMT_LEFT);
	dlg.m_listctrlIP.InsertColumn(1,_T("子网掩码"));
	dlg.m_listctrlIP.SetColumnWidth(0,110);
	dlg.m_listctrlIP.SetColumnWidth(1,110);
    
	dlg.m_listctrlIP.DeleteAllItems();
	
	CStringArray strAIpAddr,strAMask,strADnsServer,strGateWay;
	SplitString(m_clsNetSetting.m_Cards[nIndex].szGateWay,strGateWay);
	dlg.m_listNetGate.ResetContent();
	for(int i = 0; i < strGateWay.GetSize(); i++)
		dlg.m_listNetGate.AddString(strGateWay[i]);

	SplitString(m_clsNetSetting.m_Cards[nIndex].szDNSNameServer,strADnsServer);
	dlg.m_listDNS.ResetContent();
	for(i = 0; i < strADnsServer.GetSize(); i++)
		dlg.m_listDNS.AddString(strADnsServer[i]);

	SplitString(m_clsNetSetting.m_Cards[nIndex].szIpAddress,strAIpAddr);
	SplitString(m_clsNetSetting.m_Cards[nIndex].szIpMask,strAMask);

	dlg.m_listctrlIP.DeleteAllItems();

	for(i = 0; i < strAIpAddr.GetSize(); i++)
	{
		dlg.m_listctrlIP.InsertItem(i,strAIpAddr[i],0);
		dlg.m_listctrlIP.SetItemText(i,1,strAMask[i]);
	}

	strAIpAddr.RemoveAll();
	strAMask.RemoveAll();
	strADnsServer.RemoveAll();
	strGateWay.RemoveAll();
	dlg.ShowWindow(TRUE);
	
}

void CGetNetSettingDlg::SplitString(LPCTSTR lpszString, CStringArray &strArray)
{
    CString strTmp;
	TCHAR szTmp[128];

	int length = _tcslen(lpszString);
	int i = 0;
	int k = 0;
	while(i < length+1)
	{
		if(lpszString[i] != _T(' ') && lpszString[i] != _T(',') && lpszString[i] != _T('\0'))
		{
			szTmp[k] = lpszString[i];
			k++;
		}
		else
		{
			szTmp[k] = _T('\0');
			strArray.Add(szTmp);
			k = 0;
		}
		i++;
	}
}
