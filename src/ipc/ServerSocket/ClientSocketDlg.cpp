// ClientSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>
#include "ServerSocket.h"
#include "ClientSocketDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

const int SOCK_TCP  = 0;
const int SOCK_UDP  = 1;

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg dialog


CClientSocketDlg::CClientSocketDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CClientSocketDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CClientSocketDlg)
    m_strPort = _T("2000");
    m_nSockType = SOCK_TCP; // default TCP
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CClientSocketDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CClientSocketDlg)
    DDX_Control(pDX, IDC_TXT_MESSAGE, m_ctlMessage);
    DDX_Control(pDX, IDC_SVR_PORTINC, m_ctlPortInc);
    DDX_Control(pDX, IDC_MESSAGE_LIST, m_ctlMsgList);
    DDX_Control(pDX, IDC_IPADDRESS, m_ctlIPAddress);
    DDX_Text(pDX, IDC_SVR_PORT, m_strPort);
    DDX_Radio(pDX, IDC_TCP, m_nSockType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientSocketDlg, CDialog)
    //{{AFX_MSG_MAP(CClientSocketDlg)
    ON_BN_CLICKED(IDC_BTN_START, OnBtnConnect)
    ON_BN_CLICKED(IDC_BTN_STOP, OnBtnDisconnect)
    ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg message handlers

BOOL CClientSocketDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        int nVirtKey = (int) pMsg->wParam;

        if (nVirtKey == VK_ESCAPE)
            return TRUE;

        if (nVirtKey == VK_RETURN && (GetFocus()->m_hWnd  == m_ctlMessage.m_hWnd)) {
            if (m_SocketManager.IsOpen())
                OnBtnSend();

            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CClientSocketDlg::OnInitDialog()
{
    ASSERT(GetDlgItem(IDC_BTN_SEND) != NULL);
    ASSERT(GetDlgItem(IDC_BTN_START) != NULL);
    ASSERT(GetDlgItem(IDC_BTN_STOP) != NULL);
    ASSERT(GetDlgItem(IDC_TCP) != NULL);
    ASSERT(GetDlgItem(IDC_UDP) != NULL);
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu* pSysMenu = GetSystemMenu(FALSE);

    if (pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);

        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    // TODO: Add extra initialization here
    m_ctlPortInc.SetRange32(2000, 4500);
    GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
    CString strLocal;
    m_SocketManager.GetLocalAddress(strLocal.GetBuffer(256), 256);
    strLocal.ReleaseBuffer();
    m_ctlIPAddress.SetWindowText(strLocal);
    // Initialize socket manager
    m_SocketManager.SetMessageWindow(&m_ctlMsgList);
    m_SocketManager.SetServerState(false);   // run as client
    m_SocketManager.SetSmartAddressing(false);   // always send to server
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CClientSocketDlg::OnBtnConnect()
{
    UpdateData();
    CString strServer;
    m_ctlIPAddress.GetWindowText(strServer);
    bool bSuccess;

    if (m_nSockType == SOCK_TCP) {
        bSuccess = m_SocketManager.ConnectTo(strServer, m_strPort, AF_INET, SOCK_STREAM);  // TCP
    } else {
        bSuccess = m_SocketManager.ConnectTo(strServer, m_strPort, AF_INET, SOCK_DGRAM);  // UDP
    }

    if (bSuccess && m_SocketManager.WatchComm()) {
        GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
        NextDlgCtrl();
        GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
        GetDlgItem(IDC_TCP)->EnableWindow(FALSE);
        GetDlgItem(IDC_UDP)->EnableWindow(FALSE);
        CString strMsg;

        if (m_nSockType == SOCK_TCP) {
            strMsg = _T("Connection established with server: ") + strServer;
            strMsg += _T(" on port ") + m_strPort + CString("\r\n");
            m_SocketManager.GetPeerName(m_SockPeer);
        } else {
            SockAddrIn addrin;
            m_SocketManager.GetSockName(addrin);   // just to get our current address
            LONG  uAddr = addrin.GetIPAddr();
            BYTE* sAddr = (BYTE*) &uAddr;
            int nPort = ntohs(addrin.GetPort());
            CString strAddr;
            // Address is stored in network format...
            strAddr.Format(_T("IP: %u.%u.%u.%u, Port: %d"),
                           (UINT)(sAddr[0]), (UINT)(sAddr[1]),
                           (UINT)(sAddr[2]), (UINT)(sAddr[3]), nPort);
            strMsg = _T("Socket created: ") + strAddr + CString("\r\n");
            m_SockPeer.CreateFrom(strServer, m_strPort, AF_INET);
        }

        m_ctlMsgList.SetWindowText(strMsg);
    }
}

void CClientSocketDlg::OnBtnDisconnect()
{
    // Disconnect socket
    m_SocketManager.StopComm();

    if (!m_SocketManager.IsOpen()) {
        GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
        PrevDlgCtrl();
        GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
        GetDlgItem(IDC_TCP)->EnableWindow(TRUE);
        GetDlgItem(IDC_UDP)->EnableWindow(TRUE);
    }
}

void CClientSocketDlg::OnBtnSend()
{
    BYTE byBuffer[256] = { 0 };
    CString strText;
    // Update so that we can retrieve new port number also... :-)
    UpdateData();
    m_ctlMessage.GetWindowText(strText);
    int nLen = strText.GetLength();

    if (nLen > 0) {
        strText += _T("\r\n");
        nLen = strText.GetLength() + 1;
        USES_CONVERSION;
        strcpy_s(reinterpret_cast<LPSTR>(byBuffer), 256, T2CA(strText));
        m_SocketManager.WriteComm(byBuffer, nLen, INFINITE);
    }
}


void CClientSocketDlg::OnDestroy()
{
    m_SocketManager.StopComm();
    CDialog::OnDestroy();
}

