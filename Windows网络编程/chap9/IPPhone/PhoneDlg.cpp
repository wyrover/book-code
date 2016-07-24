// PhoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Phone.h"
#include "PhoneDlg.h"

#pragma comment(lib, "wsock32")

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
// CPhoneDlg dialog

CPhoneDlg::CPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CPhoneDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_TEST, OnTest)
	ON_WM_DESTROY()
	ON_COMMAND(ID_CONNECT, OnConnect)
	ON_COMMAND(ID_OPENSERVER, OnOpenserver)
	ON_COMMAND(ID_LOCAL_IP, OnLocalIp)
	ON_COMMAND(ID_DISCONNECT, OnDisconnect)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_READREADY,OnReadReady)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg message handlers

BOOL CPhoneDlg::OnInitDialog()
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
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	// we need to resize the dialog to make room for control bars.
	// first, figure out how big the control bars are.	crect rcclientstart;
	CRect rcClientNow,rcClientStart;
	
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,
				   0, reposQuery, rcClientNow);	
	// now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left,rcClientNow.top - rcClientStart.top);
	CRect  rcChild;					
	CWnd  *pWndChild = GetWindow(GW_CHILD);
	while (pWndChild)
	{           
		pWndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);		
		rcChild.OffsetRect(ptOffset);
		pWndChild->MoveWindow(rcChild, false);
		pWndChild = pWndChild->GetNextWindow();	
	}
	// adjust the dialog window dimensions	
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, false);		// and position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);
	
	if (WSAStartup(WINSOCK_VERSION,&wsaData))
	{
		MessageBeep(MB_ICONSTOP);
		MessageBox("Winsock could not be initialized!", AfxGetAppName(), MB_OK|MB_ICONSTOP);
	    WSACleanup();
	    return(FALSE);
	}
	m_bWaiting=TRUE;
	m_bPassive=TRUE;
	m_bClose=TRUE;
	m_RemoteIP=0;
	m_nBlockSize=BLOCK;
	nID=0;
	
	chSndOut=(unsigned char*)malloc(BLOCK);

	OnOpenserver();
	sndIn.SetBits(8);
	sndIn.SetBlockProp(m_nBlockSize,MAX);
	sndIn.SetSampleRate(11025);
	sndOut.SetBits(8);
	sndOut.SetBlockProp(m_nBlockSize,MAX);
	sndOut.SetSampleRate(11025);
	sndIn.SetTestProp(TRUE);
	sndIn.SetSoundOut(&sndOut);
	sndIn.SetCompressionObject(&cmpSend);

	cmpSend.SelectMethod(3);
	cmpReceive.SelectMethod(3);
	cmpReceive.SetWaveFormat(1,11025,8);
	cmpReceive.Initialize();
	cmpReceive.SetDstSamples(BLOCK/2,(unsigned char*)chRecv);
	cmpReceive.PrepareSpace(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPhoneDlg::OnPaint() 
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
HCURSOR CPhoneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPhoneDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
		
	return CDialog::Create(IDD, pParentWnd);
}

void CPhoneDlg::OnTest() 
{
	m_nBlockSize=BLOCK;
	
	sndIn.SetBits(8);
	sndIn.SetBlockProp(m_nBlockSize,MAX);
	sndIn.SetSampleRate(11025);
	
	sndOut.SetBits(8);
	sndOut.SetBlockProp(m_nBlockSize,MAX);
	sndOut.SetSampleRate(11025);
	sndIn.SetTestProp(TRUE);
	sndIn.SetSoundOut(&sndOut);
	sndIn.OpenMic();
	sndOut.OpenSpk();
}

void CPhoneDlg::OnOK() 
{
	sndIn.CloseMic();
	sndOut.CloseSpk();
    
	CDialog::OnOK();
}

void CPhoneDlg::OnDestroy() 
{
	int iErrorCode;
	char chMsgBuffer[100];

	if(m_sSend)
		closesocket(m_sSend);
	if(m_sRecv)
		closesocket(m_sRecv);

	if ((iErrorCode = WSACleanup()))
	{
		wsprintf(chMsgBuffer, "Winsock error %d.", iErrorCode);
		MessageBeep(MB_ICONSTOP);
		MessageBox(chMsgBuffer,  AfxGetAppName(), MB_OK|MB_ICONSTOP);   
    }
	
	free(chSndOut);
	if(nID!=0)
		KillTimer(nID);
	CDialog::OnDestroy();
}

void CPhoneDlg::OnConnect() 
{
	m_bPassive=FALSE;
	Connect(TRUE);
}

void CPhoneDlg::OnOpenserver() 
{
	m_sSend=socket(AF_INET,SOCK_DGRAM,0); 
	m_sRecv=socket(AF_INET,SOCK_DGRAM,0); 
	m_sAccept=socket(AF_INET,SOCK_STREAM,0);
	char chName[64];

	gethostname(chName,sizeof(chName));

	addrSend.sin_family=AF_INET;
	addrSend.sin_addr.S_un.S_addr=INADDR_ANY;//inet_addr(m_chLocalIp);
	addrSend.sin_port=htons(SENDPORT);
	if (bind(m_sSend,(LPSOCKADDR)&addrSend,sizeof(addrSend))== SOCKET_ERROR)
	{
		ReportWinsockErr("Could not bind server socket.");
	}
	
	addrRecv.sin_family=AF_INET;
	addrRecv.sin_addr.S_un.S_addr=INADDR_ANY;//inet_addr(m_chLocalIp);
	addrRecv.sin_port=htons(RECEIVEPORT);
	if (bind(m_sRecv,(LPSOCKADDR)&addrRecv,sizeof(addrRecv))==SOCKET_ERROR)
	{
		ReportWinsockErr("Could not bind server socket.");
	}

	int iError=WSAAsyncSelect(m_sRecv,m_hWnd,WM_READREADY,FD_READ|FD_CLOSE);
	if(iError==SOCKET_ERROR)
	{
		ReportWinsockErr("Could not Select server socket.");
	}
}

LRESULT CPhoneDlg::OnReadReady(WPARAM wParam, LPARAM lParam)
{
	int iRecievedNum;
	char* ip=(char*)malloc(64);
	
	if(m_bWaiting){
		iRecievedNum=recv(m_sRecv,chRecv,sizeof(chRecv),0);
		if(iRecievedNum==SOCKET_ERROR)
		{
			ReportWinsockErr("Could not Receive the Data.");
			return 0L;
		}
		if(chRecv[0]=='P' && chRecv[1]=='H' ){
			m_bWaiting=FALSE;
			sscanf(chRecv,"PHONE %s",ip);
			m_RemoteIP=inet_addr(ip);
			m_RemoteAddr.sin_addr.S_un.S_addr=m_RemoteIP;
			m_RemoteAddr.sin_family=AF_INET; 
			m_RemoteAddr.sin_port=htons(RECEIVEPORT);
			if(m_bPassive){
				char *call=(char*)malloc(128);
				wsprintf(call,"A Call From %s\n,Are want to response?",ip); 
				if(1==AfxMessageBox(call,MB_OKCANCEL))
					Connect(FALSE);
				else
					return 0L;
				free(call);
			}
			m_bPassive=TRUE;
			sndIn.SetTestProp(FALSE);
			sndIn.SetSocketParam(m_sSend,m_RemoteAddr);
			sndIn.OpenMic();
			sndOut.OpenSpk();
		}
		else{
		//	MessageBox("Error On the Buffer");
			return 0L;
		}
	}
	else{
		int tmp=sizeof(m_RemoteAddr);
		iRecievedNum=recvfrom(m_sRecv,chRecv,BLOCK,0,(LPSOCKADDR)&m_RemoteAddr,&tmp);
		if((chRecv[0]=='P' && chRecv[1]=='H'))
			return 0L;
		if(chRecv[0]=='C' && chRecv[1]=='O'){
			if(!m_bClose)
				OnDisconnect();
			return 0L;
		}
		if(iRecievedNum==SOCKET_ERROR)
		{
			ReportWinsockErr("Could not Receive the Data.");
			return 0L;
		}
		cmpReceive.SetDstSamples(iRecievedNum,(unsigned char*)chRecv);
		long temp=cmpReceive.UnConvert(TRUE,NULL,0);
		if(temp<BLOCK){
			cmpReceive.UnConvert(FALSE,chSndOut,temp);
			sndOut.WriteData((char *)chSndOut,temp);
		}
		else{
			cmpReceive.UnConvert(FALSE,chSndOut,BLOCK);
			sndOut.WriteData((char *)chSndOut,BLOCK);
		}
	}
	free(ip);
	return 0L;
}

void CPhoneDlg::ReportWinsockErr(LPSTR lpszErrorMsg)
{
	char chMsgBuffer[100];

	wsprintf(chMsgBuffer, "\nWinsock error %d: %s\n\n", WSAGetLastError(), lpszErrorMsg);
	MessageBox(chMsgBuffer,  AfxGetAppName(), MB_OK|MB_ICONSTOP);
	return;   
}

int CPhoneDlg::GetLocalIP(unsigned long *ip, int len)
{
	char chName[128];

	gethostname(chName,sizeof(chName));
	struct hostent* pHost;
	pHost = gethostbyname(chName); 
	CString str;
	int i=0;
	for(i=0; pHost!=NULL && pHost->h_addr_list[i]!=NULL; i++ ){
		 int j;  
		 for(j = 0;j<pHost->h_length;j++ ){
			 CString addr;
			 if( j > 0 )
				 str += "."; 
 			addr.Format("%u", (unsigned int)((unsigned char*)pHost->h_addr_list[i])[j]);
			str += addr;
		 }
		 if(i>len)
			 return 0;
		ip[i]=inet_addr(str);
		str.Empty();
 	}
	return i;
}

void CPhoneDlg::OnLocalIp() 
{
	unsigned long ip[2];
	int i=GetLocalIP(ip,2);
	char* str=(char*)malloc(64);
	in_addr t;
	t.S_un.S_addr=ip[0];
	if(i!=0)
		str=inet_ntoa(t);
	MessageBox(str);
	free(str);
}

void CPhoneDlg::OnDisconnect() 
{
	char *chClose=(char*)malloc(12);
	wsprintf(chClose,"CLOSE");
	m_bClose=TRUE;
	sendto(m_sSend,chClose,sizeof(chClose),0,(LPSOCKADDR)&m_RemoteAddr,sizeof(m_RemoteAddr));
	sndIn.CloseMic();
	sndOut.CloseSpk();
	m_RemoteIP=0;
    m_bWaiting=TRUE;
	m_bPassive=TRUE;
	free(chClose);
}

BOOL CPhoneDlg::Connect(BOOL b)
{
	unsigned long ip;
	char* str=new char[64];
	char chQuery[64];
	SOCKADDR_IN to;
	
	m_bClose=FALSE;
	if(b){
		CIPAddress dlg;
		if(dlg.DoModal()==IDCANCEL)
			return FALSE;
		else{
			m_RemoteIP=inet_addr(dlg.ip);
			nID=SetTimer(1,10000,NULL);
		}
	}
	int i=GetLocalIP(&ip,1);
	in_addr t;
	t.S_un.S_addr=ip;
	if(i!=0)
		str=inet_ntoa(t);
	wsprintf(chQuery,"PHONE %s",str);

	to.sin_family=AF_INET;
	to.sin_port=htons(RECEIVEPORT);
	to.sin_addr.S_un.S_addr=m_RemoteIP;//inet_addr("210.32.151.247"); 

	int iError=sendto(m_sSend,chQuery,sizeof(chQuery),0,(LPSOCKADDR)&to,sizeof(to));
	if(iError==SOCKET_ERROR)
	{
		ReportWinsockErr("Could not establish connect.");
		return FALSE;
	}
	return TRUE;
}

void CPhoneDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent){
	case 1:
		if(m_bPassive==FALSE){
			AfxMessageBox("You have called the wrong IP,or he don't want to response!");
			m_bPassive=TRUE;
		}
	case 2:{
		   }
	}
		
	CDialog::OnTimer(nIDEvent);
}
