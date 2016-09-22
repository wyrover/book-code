
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CServerDlg dialog




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SockListen = INVALID_SOCKET;
	m_SockClient = INVALID_SOCKET;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CServerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SEND, &CServerDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CServerDlg::WinSockInit()
{
	WSADATA data = {0};
	if(WSAStartup(MAKEWORD(2, 2), &data))
		return FALSE;
	if ( LOBYTE(data.wVersion) !=2 || HIBYTE(data.wVersion) != 2 ){
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL CServerDlg::SOCKET_Select(SOCKET hSocket, int nTimeOut, BOOL bRead)
{
	fd_set fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec  = 0;
	tv.tv_usec = nTimeOut;

	int iRet = 0;
	if ( bRead ) {
		iRet = select(0, &fdset, NULL , NULL, &tv);
	}else{
		iRet = select(0, NULL , &fdset, NULL, &tv);
	}

	if(iRet <= 0) {
		return FALSE;
	} else if (FD_ISSET(hSocket, &fdset)){
		return TRUE;
	}
	return FALSE;
}

#define MAX_BUF_SIZE 4096
UINT CServerDlg::ThreadProc(LPVOID pParam)
{
	ASSERT(pParam);
	sockaddr_in clientAddr = {0};
	CServerDlg *pThis = (CServerDlg *)pParam;
	pThis->WinSockInit();
	pThis->m_SockListen = INVALID_SOCKET;
	pThis->m_SockListen = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if ( pThis->m_SockListen == INVALID_SOCKET ) {
		AfxMessageBox(_T("新建Socket失败！"));
		goto __Error_End;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(9527);

	if ( bind(pThis->m_SockListen, (sockaddr*)&service, sizeof(sockaddr_in)) == SOCKET_ERROR ) {
		AfxMessageBox(_T("绑定端口失败！"));
		goto __Error_End;
	}

	if( listen(pThis->m_SockListen, SOMAXCONN) == SOCKET_ERROR ) {
		AfxMessageBox(_T("监听失败！"));
		goto __Error_End;
	}

	int iLen = sizeof(sockaddr_in);
	pThis->m_SockClient = accept(pThis->m_SockListen, (struct sockaddr *)&clientAddr , &iLen);
	if (pThis->m_SockClient == INVALID_SOCKET) {
		goto __Error_End;
	}

	while(TRUE) {
		if ( pThis->SOCKET_Select(pThis->m_SockClient) ) {
			TCHAR szBuf[MAX_BUF_SIZE] = {0};
			int iRet = recv(pThis->m_SockClient, (char *)szBuf, MAX_BUF_SIZE, 0);
			if ( iRet > 0 ) {
				//right;
				pThis->ShowMsg(szBuf);
			}else{
				//close socket;
				pThis->ShowMsg(_T("客户端已下线，请重新开启服务器等待客户端连接！"));
				break;
			}
		}
		Sleep(500);
	}

__Error_End:
	if (pThis->m_SockListen != INVALID_SOCKET) {
		closesocket(pThis->m_SockListen);
	}
	if (pThis->m_SockClient != INVALID_SOCKET) {
		closesocket(pThis->m_SockClient);
	}
	WSACleanup();
	return TRUE;
}

void CServerDlg::ShowMsg(CString strMsg)
{
	CEdit *pEditShow = (CEdit *)GetDlgItem(IDC_EDIT_SHOW);
	ASSERT(pEditShow);
	pEditShow->SetSel(-1, -1);
	pEditShow->ReplaceSel(_T("Client : ") + strMsg+_T("\r\n"));
}

void CServerDlg::OnBnClickedBtnStart()
{
	AfxBeginThread(ThreadProc, this);
}

void CServerDlg::OnBnClickedBtnSend()
{
	CString strText;
	ASSERT(m_SockClient != INVALID_SOCKET);
	GetDlgItemText(IDC_EDIT_WRITE, strText);
	if (!strText.IsEmpty() && SOCKET_Select(m_SockClient, 100, FALSE)) {
		send(m_SockClient, (char *)strText.GetBuffer(), strText.GetLength()*sizeof(TCHAR), 0);
		SetDlgItemText(IDC_EDIT_WRITE, _T(""));
	}
}
