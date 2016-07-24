// TcpSpyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TcpSpy.h"
#include "TcpSpyDlg.h"
#include <Winsock2.h>

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
// CTcpSpyDlg dialog

CTcpSpyDlg::CTcpSpyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpSpyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcpSpyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_BIRD);
}

void CTcpSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcpSpyDlg)
	DDX_Control(pDX, IDC_EDIT_TIMER, m_txtTimer);
	DDX_Control(pDX, IDC_CHKAUTO, m_chkAuto);
	DDX_Control(pDX, IDC_LIST_TCP, m_tcp);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTcpSpyDlg, CDialog)
	//{{AFX_MSG_MAP(CTcpSpyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnExit)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnDisconnect)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TCP, OnDblclkListTcp)
	ON_BN_CLICKED(IDC_CHKAUTO, OnAuto)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcpSpyDlg message handlers

BOOL CTcpSpyDlg::OnInitDialog()
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
	pGetTcp=NULL;
	pGetUdp=NULL;
	pEtyTcp=NULL;
	hInst=NULL;

	HINSTANCE hInst = LoadLibrary("iphlpapi.dll"); 
	if(hInst==NULL) return FALSE;
	//typedef int(*PFUNC)(void);  PFUNC砆﹚竡ㄧ计皐,⊿Τ把计intㄧ计
	//typedef DWORD (WINAPI *MYFUNC)(PMIB_TCPTABLE,PDWORD,BOOL); 

	pGetTcp = (GetTcpTableFun)GetProcAddress(hInst, "GetTcpTable");
	if(pGetTcp==NULL) 
	{
		AfxMessageBox("GET FUNCTION GetTcpTable Failed"	);
		return FALSE;
	} 
	pGetUdp=(GetUdpTableFun)GetProcAddress(hInst,"GetUdpTable");
	if(pGetUdp==NULL)
	{
		AfxMessageBox("GET FUNCTION GetUdpTable Failed");
		return FALSE;
	}

	pEtyTcp=(SetTcpEntryFun)GetProcAddress(hInst,"SetTcpEntry");
	if(pEtyTcp==NULL)
	{
		AfxMessageBox("GET FUNCTION SetTcpEntry Failed"	);
		return FALSE;
	}

	m_tcp.SetExtendedStyle (LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_tcp.InsertColumn (0,"Protocol",LVCFMT_CENTER,80);
	m_tcp.InsertColumn (1,"ID",LVCFMT_LEFT,30);
	m_tcp.InsertColumn (2,"Status",LVCFMT_LEFT,100);
	m_tcp.InsertColumn (3,"Local IP",LVCFMT_LEFT,100);
	m_tcp.InsertColumn (4,"Local Port",LVCFMT_CENTER,100);
	m_tcp.InsertColumn (5,"Remote IP",LVCFMT_LEFT,100);
	m_tcp.InsertColumn (6,"Remote Port",LVCFMT_CENTER,100);
	m_txtTimer.SetLimitText (4);
	m_txtTimer.SetWindowText("1000");
	iDisCon=0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTcpSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTcpSpyDlg::OnPaint() 
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
HCURSOR CTcpSpyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTcpSpyDlg::OnRefresh() 
{
	// TODO: Add your control notification handler code here

	if(pGetTcp==NULL ||pGetUdp==NULL)return;
	
	CString strStatus,strTmp;//char szTemp[100];
	//BYTE	pTcpBuf[100*20+4];
	BYTE pUdpBuf[100*8+4];
	DWORD   dwTableSize;
	DWORD  lret;
	int i,k=0;
	in_addr	addrLoc,addrRem;
	char szLocAddr[100],szRemAddr[100];
	DWORD dwLocIP,dwRemIP;
	unsigned short nLocalPort,nRemotePort;

	m_tcp.DeleteAllItems ();
	dwTableSize=100*20+4;
	lret=pGetTcp((PMIB_TCPTABLE)pTcpBuf,&dwTableSize,FALSE);
	if (lret != NO_ERROR) return;
	mibtcp=(PMIB_TCPTABLE)pTcpBuf;
	k=(int)mibtcp->dwNumEntries-1;
	
	int m=0;
	for(i=0;i<k;i++){
		dwRemIP=htonl(mibtcp->table[i].dwRemoteAddr);
		addrRem.S_un.S_addr = ntohl(dwRemIP);
		strcpy(szRemAddr, inet_ntoa(addrRem));
		
		for(int j=0;j<iDisCon;j++){
			if(strcmp(szRemAddr,DisConIPAddr[j])==0) break;
		}
		if(j<iDisCon){
				pEtyTcp(&mibtcp->table [i]);
				continue;
		}
		
		switch (mibtcp->table[i].dwState)
		{
		case MIB_TCP_STATE_CLOSED     : 
			strStatus="CLOSED";
			break;
		case MIB_TCP_STATE_LISTEN     : 
			strStatus="LISTEN";
			break;
		case MIB_TCP_STATE_SYN_SENT   : 
			strStatus="SYN_SENT";
			break;
		case MIB_TCP_STATE_SYN_RCVD   : 
			strStatus="SYN_RCVD";
			break;
		case MIB_TCP_STATE_ESTAB      : 
			strStatus="ESTAB";
			break;
		case MIB_TCP_STATE_FIN_WAIT1  :
			strStatus="FIN_WAIT1";
			break;
		case MIB_TCP_STATE_FIN_WAIT2  :
			strStatus="FIN_WAIT2";
			break;
		case MIB_TCP_STATE_CLOSE_WAIT :
			strStatus="CLOSE WAIT";
			break;
		case MIB_TCP_STATE_CLOSING    :
			strStatus="CLOSING";
			break;
		case MIB_TCP_STATE_LAST_ACK   :
			strStatus="LAST ACK";
			break;
		case MIB_TCP_STATE_TIME_WAIT  :
			strStatus="TIME WAIT";
			break;
		case MIB_TCP_STATE_DELETE_TCB :
			strStatus="DELETE TCB";
			break;
		default:
			strStatus="<ERR>";
		}
		
		m_tcp.InsertItem (m,"TCP");
		strTmp.Format ("%3d",m);
		m_tcp.SetItemText (m,1,strTmp);
		m_tcp.SetItemText(m,2,strStatus);
		
		//Local IP
		dwLocIP=htonl(mibtcp->table[i].dwLocalAddr);
		addrLoc.S_un.S_addr = ntohl(dwLocIP);
		strcpy(szLocAddr, inet_ntoa(addrLoc));		
		m_tcp.SetItemText (m,3,szLocAddr);
		
		nLocalPort = htons(mibtcp->table[i].dwLocalPort);
		strTmp.Format ("%u",nLocalPort); //sprintf(szTemp, "%u", nLocalPort);
		m_tcp.SetItemText(m,4,strTmp);
		
		//Remote IP
		//dwRemIP=htonl(mibtcp->table[i].dwRemoteAddr);
		//addrRem.S_un.S_addr = ntohl(dwRemIP);
		//strcpy(szRemAddr, inet_ntoa(addrRem));
		m_tcp.SetItemText (m,5,szRemAddr);
		
		nRemotePort = htons(mibtcp->table[i].dwRemotePort );
		//if (nRemotePort==80) {
		//	mibtcp->table [i].dwState =MIB_TCP_STATE_DELETE_TCB;
		//	pEtyTcp(&mibtcp->table [i]);
		//	strTmp="DELETE";
		//}else
		strTmp.Format ("%u",nRemotePort);
		m_tcp.SetItemText(m,6,strTmp);
		m++;
	}
	
	k=m;
	dwTableSize=100*8+4;
	lret=pGetUdp((PMIB_UDPTABLE)pUdpBuf,&dwTableSize,FALSE);
	if(lret!=NO_ERROR) return;
	mibUdp=(PMIB_UDPTABLE)pUdpBuf;
	for(i=0;i<(int)mibUdp->dwNumEntries-1;i++)
	{
		strTmp.Format ("%3d",i);
		m_tcp.InsertItem (k+i,"UDP");
		m_tcp.SetItemText (k+i,1,strTmp);
		
		dwLocIP=htonl(mibUdp->table[i].dwLocalAddr);
		addrLoc.S_un.S_addr = ntohl(dwLocIP);
		strcpy(szLocAddr, inet_ntoa(addrLoc));
		nLocalPort = htons(mibUdp->table[i].dwLocalPort);
		strTmp.Format ("%u",nLocalPort); //sprintf(szTemp, "%u", nLocalPort);
		m_tcp.SetItemText(k+i,3,szLocAddr);
		m_tcp.SetItemText(k+i,4,strTmp);
	}
}


void CTcpSpyDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (hInst!=NULL) FreeLibrary(hInst);
	EndDialog(0);
}

void CTcpSpyDlg::OnDisconnect() 
{
	// TODO: Add your control notification handler code here

	int nSel = m_tcp.GetNextItem (-1,LVNI_SELECTED); 
	if (nSel==-1)return;
	char szIdx[5];
	CString str;
	str=m_tcp.GetItemText(nSel,0);
	if(str!="TCP")return;
	m_tcp.GetItemText(nSel,1,szIdx,sizeof(szIdx));
	int i=atoi(szIdx);
	mibtcp->table[i].dwState =MIB_TCP_STATE_DELETE_TCB;
	DWORD dwRet=pEtyTcp(&mibtcp->table [i]);
	if (dwRet==NO_ERROR){
		if(iDisCon<10){
			char szip[30];
		    m_tcp.GetItemText(nSel,5,szip,30);
			DisConIPAddr[iDisCon]=new char [strlen(szip)+1];
			strcpy(DisConIPAddr[iDisCon],szip);
			iDisCon++;
		}
		m_tcp.DeleteItem (nSel);
	}
}

void CTcpSpyDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (hInst!=NULL) FreeLibrary(hInst);
}



void CTcpSpyDlg::OnDblclkListTcp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW *pitem=(NM_LISTVIEW *)pNMHDR;
	int ir=pitem->iItem ;
	char szIP[100];
	m_tcp.GetItemText(ir,5,szIP,100);
	
	if(strlen(szIP)>0 ){
		if(strcmp(szIP,"0.0.0.0")){
			HCURSOR hWaitCur=LoadCursor(NULL,IDC_WAIT);
			HCURSOR hOldCur=SetCursor(hWaitCur);
			HOSTENT *phost=NULL;
			int addr=inet_addr(szIP);
			phost=gethostbyaddr((char*)&addr,100,AF_INET);
			SetCursor(hOldCur);
			if (phost)	::MessageBox(NULL,phost->h_name,"Host Name",MB_OK);
			else  ::MessageBox(NULL,szIP,"IP Addr",MB_OK);			
		}
	}
	*pResult = 0;
}

void CTcpSpyDlg::OnAuto() 
{
	// TODO: Add your control notification handler code here
	int i=m_chkAuto.GetCheck ();
	char szTimer[5];
	m_txtTimer.GetWindowText(szTimer,5);
	int itimer=1000;
	if (strlen(szTimer)>0) itimer=atoi(szTimer);
	if(i==1) SetTimer(1,itimer,NULL);
	else KillTimer(1);
}

void CTcpSpyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnRefresh();
	CDialog::OnTimer(nIDEvent);
}
