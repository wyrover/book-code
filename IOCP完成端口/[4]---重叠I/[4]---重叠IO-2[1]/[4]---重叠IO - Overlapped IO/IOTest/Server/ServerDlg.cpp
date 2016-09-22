
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

DWORD m_dwTotalEvent = 0;
WSAEVENT m_ArrEvent[WSA_MAXIMUM_WAIT_EVENTS] = {0};
CArray <CSocketOverlap *, CSocketOverlap *> m_ArrSocketOlp;

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

SOCKET CServerDlg::FindSocketFromEvent(WSAEVENT hEvent)
{
	for (INT_PTR idx = 0; idx < m_ArrSocketOlp.GetCount(); idx++) {
		if (hEvent == m_ArrSocketOlp.GetAt(idx)->m_Olap.hEvent) {
			return m_ArrSocketOlp.GetAt(idx)->m_Socket;
		}
	}
	return INVALID_SOCKET;
}

CSocketOverlap *CServerDlg::GetOverlapFromEvent(WSAEVENT hEvent)
{
	CSocketOverlap *pItem = NULL;
	for (INT_PTR idx = 0; idx < m_ArrSocketOlp.GetCount(); idx++) {
		pItem = m_ArrSocketOlp.GetAt(idx);
		if (hEvent == pItem->m_Olap.hEvent) {
			return pItem;
		}
	}
	return NULL;
}

BOOL CServerDlg::AddSocketOlpToArray(CSocketOverlap *hSocketOlp)
{
	if (m_ArrSocketOlp.GetCount() < MAXIMUM_WAIT_OBJECTS) {
		m_ArrSocketOlp.Add(hSocketOlp);
		return TRUE;
	}
	return FALSE;
}

void CServerDlg::DelSocketOlpFromArray(CSocketOverlap *hSocketOlp)
{
	CSocketOverlap *pItem = NULL;
	for (INT_PTR idx = 0; idx < m_ArrSocketOlp.GetCount(); idx++) {
		pItem = m_ArrSocketOlp.GetAt(idx);
		if (pItem->m_Olap.hEvent == hSocketOlp->m_Olap.hEvent) {
			m_ArrSocketOlp.RemoveAt(idx);
		}
	}
}

BOOL CServerDlg::PostAccept(SOCKET sListenSock)
{
	DWORD dwBytesRecv = 0;
	CSocketOverlap *m_pItem = new CSocketOverlap();
	m_pItem->m_IOType = TYPE_ACCEPT;
	m_pItem->m_Socket = sListenSock;
	m_pItem->m_AccSocket = WSASocket(AF_INET , SOCK_STREAM , IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	BOOL bRet = AddSocketOlpToArray(m_pItem);
	if (!bRet) {
		delete m_pItem;
		return FALSE;
	}
	m_ArrEvent[m_dwTotalEvent++] = m_pItem->m_Olap.hEvent;
	bRet = AcceptEx(m_pItem->m_Socket, m_pItem->m_AccSocket, m_pItem->m_szBuf, 0, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &dwBytesRecv, &m_pItem->m_Olap);
	if (!bRet) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CServerDlg::PostAccept(CSocketOverlap *sListenOs)
{
	DWORD dwBytesRecv = 0;
	sListenOs->m_IOType = TYPE_ACCEPT;
	sListenOs->m_AccSocket = WSASocket(AF_INET , SOCK_STREAM , IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	BOOL bRet = AcceptEx(sListenOs->m_Socket, sListenOs->m_AccSocket, sListenOs->m_szBuf, 0, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &dwBytesRecv, &sListenOs->m_Olap);
	if (!bRet) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			return FALSE;
		}
	}
	return TRUE;
}

CSocketOverlap *CServerDlg::AllocSocketOverlap(SOCKET mSocket)
{
	CSocketOverlap *pItem = new CSocketOverlap();
	pItem->m_Socket = mSocket;
	AddSocketOlpToArray(pItem);
	m_ArrEvent[m_dwTotalEvent++] = pItem->m_Olap.hEvent;
	return pItem;
}

void CServerDlg::FreeSocketOverlap(CSocketOverlap *pItem)
{
	if (pItem->m_Socket != INVALID_SOCKET)
		closesocket(pItem->m_Socket);
	if (pItem->m_AccSocket != INVALID_SOCKET)
		closesocket(pItem->m_AccSocket);

	DelSocketOlpFromArray(pItem);
	DelEventFromArray(pItem->m_Olap.hEvent);
	WSACloseEvent(pItem->m_Olap.hEvent);
	delete pItem;
}

void CServerDlg::DelEventFromArray(WSAEVENT hEvent)
{
	int index = -1, idx = 0;
	for (idx = 0; m_ArrEvent[idx] != 0; idx++) {
		if (m_ArrEvent[idx] == hEvent)
			index = idx;
	}
	for (idx = index; m_ArrEvent[idx] != 0; idx++) {
		m_ArrEvent[idx] = m_ArrEvent[idx+1];
	}
	--m_dwTotalEvent;
}

BOOL CServerDlg::PostRecv(CSocketOverlap *pItem)
{
	WSABUF wsaBuf = {0};
	ZeroMemory(pItem->m_szBuf, MAX_BUF_SIZE);
	wsaBuf.buf = pItem->m_szBuf;
	wsaBuf.len = MAX_BUF_SIZE;
	pItem->m_IOType = TYPE_READ;
	DWORD dwBytesRecv = 0, dwFlags = 0;
	int iRet = WSARecv(pItem->m_Socket, &wsaBuf, 1, &dwBytesRecv, &dwFlags, &(pItem->m_Olap), NULL);
	if (iRet != NO_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			return FALSE;
		}
	}
	return TRUE;
}

UINT CServerDlg::ThreadProc(LPVOID pParam)
{
	ASSERT(pParam);
	sockaddr_in clientAddr = {0};
	///////////////////////////////////////
	DWORD dwIndex = 0, dwTrans = 0, dwFlags = 0;
	/////////////////////////////////////////////////////
	CServerDlg *pThis = (CServerDlg *)pParam;
	pThis->WinSockInit();
	SOCKET m_SockListen = WSASocket(AF_INET , SOCK_STREAM , IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if ( m_SockListen == INVALID_SOCKET ) {
		AfxMessageBox(_T("ÐÂ½¨SocketÊ§°Ü£¡"));
		goto __Error_End;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(9527);

	if ( bind(m_SockListen, (sockaddr*)&service, sizeof(sockaddr_in)) == SOCKET_ERROR ) {
		AfxMessageBox(_T("°ó¶¨¶Ë¿ÚÊ§°Ü£¡"));
		goto __Error_End;
	}

	if( listen(m_SockListen, SOMAXCONN) == SOCKET_ERROR ) {
		AfxMessageBox(_T("¼àÌýÊ§°Ü£¡"));
		goto __Error_End;
	}

	pThis->PostAccept(m_SockListen);

	while (TRUE) {
		dwIndex = WSAWaitForMultipleEvents(m_dwTotalEvent, m_ArrEvent, FALSE, 100, FALSE);
		if (dwIndex == WSA_WAIT_TIMEOUT || dwIndex == WSA_WAIT_IO_COMPLETION) {
			continue;
		}
		dwIndex = dwIndex - WSA_WAIT_EVENT_0;
		CSocketOverlap *pCurItem = pThis->GetOverlapFromEvent(m_ArrEvent[dwIndex]);
		WSAResetEvent(m_ArrEvent[dwIndex]);
		BOOL bRet = WSAGetOverlappedResult(pThis->FindSocketFromEvent(m_ArrEvent[dwIndex]), &pCurItem->m_Olap, &dwTrans, TRUE, &dwFlags);
		if (!bRet) {
			continue;
		}
		switch(pCurItem->m_IOType)
		{
		case TYPE_ACCEPT:
			{
				SOCKET accSock = pCurItem->m_AccSocket;
				pThis->PostRecv(pThis->AllocSocketOverlap(accSock));
				pThis->PostAccept(pCurItem);
			}
			break;
		case TYPE_READ:
			{
				if (dwTrans > 0) {
					USES_CONVERSION;
					CString strText = (LPTSTR)(pCurItem->m_szBuf);
					pThis->ShowMsg(strText);
					pThis->PostRecv(pCurItem);
				}else{
					// client close;
					pThis->FreeSocketOverlap(pCurItem);
				}
			}
			break;
		case TYPE_WRITE:
			pCurItem = pCurItem;
			break;
		default:break;
		}
	}

__Error_End:
	if (m_SockListen != INVALID_SOCKET) {
		closesocket(m_SockListen);
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
	//CString strText;
	//ASSERT(m_SockClient != INVALID_SOCKET);
	//GetDlgItemText(IDC_EDIT_WRITE, strText);
	//if (!strText.IsEmpty()) {
	//	send(m_SockClient, (char *)strText.GetBuffer(), strText.GetLength()*sizeof(TCHAR), 0);
	//	SetDlgItemText(IDC_EDIT_WRITE, _T(""));
	//}
}
