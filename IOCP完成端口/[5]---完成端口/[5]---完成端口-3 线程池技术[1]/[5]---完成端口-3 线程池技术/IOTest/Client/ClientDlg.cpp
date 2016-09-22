
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

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


// CClientDlg dialog




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Socket = INVALID_SOCKET;
	m_bExit = FALSE;
	m_hThread = NULL;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CClientDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEND, &CClientDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
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

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CClientDlg::WinSockInit()
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

BOOL CClientDlg::SOCKET_Select(SOCKET hSocket, int nTimeOut, BOOL bRead)
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
UINT CClientDlg::ThreadProc(LPVOID pParam)
{
	ASSERT(pParam);
	CClientDlg *pThis = (CClientDlg *)pParam;
	pThis->WinSockInit();
	pThis->m_Socket = INVALID_SOCKET;
	pThis->m_Socket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if ( pThis->m_Socket == INVALID_SOCKET ) {
		AfxMessageBox(_T("新建Socket失败！"));
		goto __Error_End;
	}
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(9527);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( connect(pThis->m_Socket, (struct sockaddr *)&server,  sizeof(struct sockaddr)) == SOCKET_ERROR ) {
		AfxMessageBox(_T("连接失败，请重试！"));
		goto __Error_End;
	}
	while(TRUE) {
		if ( !pThis->m_bExit && pThis->SOCKET_Select(pThis->m_Socket) ) {
			TCHAR szBuf[MAX_BUF_SIZE] = {0};
			int iRet = recv(pThis->m_Socket, (char *)szBuf, MAX_BUF_SIZE, 0);
			if ( iRet > 0 ) {
				//right;
				pThis->ShowMsg(szBuf);
			}else{
				//close socket;
				pThis->ShowMsg(_T("聊天室服务器已停止，请重新进行连接！"));
				break;
			}
		}
		Sleep(500);
	}

__Error_End:
	if (pThis->m_Socket != INVALID_SOCKET) {
		closesocket(pThis->m_Socket);
	}
	WSACleanup();
	return TRUE;
}

void CClientDlg::ShowMsg(CString strMsg)
{
	CEdit *pEditShow = (CEdit *)GetDlgItem(IDC_EDIT_SHOW);
	ASSERT(pEditShow);
	pEditShow->SetSel(-1, -1);
	pEditShow->ReplaceSel(_T("Server : ") + strMsg+_T("\r\n"));
}

void CClientDlg::OnBnClickedBtnConnect()
{
	CWinThread *pThread = AfxBeginThread(ThreadProc, this);
	m_hThread = pThread->m_hThread;
}

void CClientDlg::OnBnClickedBtnSend()
{
	CString strText;
	ASSERT(m_Socket != INVALID_SOCKET);
	GetDlgItemText(IDC_EDIT_WRITE, strText);
	if (!strText.IsEmpty() && SOCKET_Select(m_Socket, 100, FALSE)) {
		send(m_Socket, (char *)strText.GetBuffer(), strText.GetLength()*sizeof(TCHAR), 0);
		SetDlgItemText(IDC_EDIT_WRITE, _T(""));
	}
}

void CClientDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_bExit = TRUE;
	if (m_hThread) {
		WaitForSingleObject(m_hThread, 1000);
	}
	if (m_Socket != INVALID_SOCKET) {
		closesocket(m_Socket);
	}

	CDialog::OnCancel();
}
