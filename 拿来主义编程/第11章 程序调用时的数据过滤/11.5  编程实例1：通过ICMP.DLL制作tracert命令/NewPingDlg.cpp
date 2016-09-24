// NewPingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewPing.h"
#include "NewPingDlg.h"

//配置文件
#define INIFILENAME ".\\NewPing.ini"

//所需要的头文件和静态库文件
#include "IPHlpApi.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "IPHlpApi.Lib")

#define LIST_ECHO_TTL   0
#define LIST_ECHO_IP    1
#define LIST_ECHO_MS    2

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
// CNewPingDlg dialog

CNewPingDlg::CNewPingDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CNewPingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNewPingDlg)
    m_strDestIP = _T("");
    m_strComment = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewPingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewPingDlg)
    DDX_Control(pDX, IDC_LIST_Echo, m_ctlListEcho);
    DDX_Text(pDX, IDC_EDIT_DestIP, m_strDestIP);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewPingDlg, CDialog)
    //{{AFX_MSG_MAP(CNewPingDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONIcmpDll, OnBUTTONIcmpDll)
    ON_BN_CLICKED(IDC_BUTTONClear, OnBUTTONClear)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPingDlg message handlers

BOOL CNewPingDlg::OnInitDialog()
{
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
    m_ctlListEcho.InsertColumn(LIST_ECHO_TTL, "TTL", LVCFMT_LEFT, 80);
    m_ctlListEcho.InsertColumn(LIST_ECHO_IP, "IP地址", LVCFMT_LEFT, 100);
    m_ctlListEcho.InsertColumn(LIST_ECHO_MS, "耗时", LVCFMT_LEFT, 100);
    m_ctlListEcho.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    char buff[MAX_PATH];
    GetPrivateProfileString("Init", "IP", "127.0.0.1", buff, MAX_PATH, INIFILENAME);
    m_strDestIP.Format("%s", buff);
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNewPingDlg::OnPaint()
{
    if (IsIconic()) {
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
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNewPingDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CNewPingDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CNewPingDlg::OnCancel()
{
    //函数退出
    if (MessageBox("确实要退出吗，是否继续？", "操作提示", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        UpdateData(TRUE);
        WritePrivateProfileString("Init", "IP", m_strDestIP, INIFILENAME);
        CDialog::OnCancel();
    }
}

//先声明ICMP.dll中要调用的三个函数。
typedef HANDLE(WINAPI *ICMPCREATEFILE)(VOID);
typedef BOOL(WINAPI *ICMPCLOSEHANDLE)(HANDLE IcmpHandle);
typedef DWORD(WINAPI *ICMPSENDECHO)(
    HANDLE IcmpHandle,             //句柄
    IPAddr DestinationAddress,     //目的IP地址
    LPVOID RequestData,            //所发送的数据
    WORD RequestSize,              //所发送数据的长度
    PIP_OPTION_INFORMATION RequestOptions,//查询时所用的选项。
    LPVOID ReplyBuff,              //接收缓冲区。
    DWORD ReplySize,               //接收缓冲区的长度。
    DWORD Timeout);                //超时值。
//所用的函数名。
ICMPCREATEFILE IcmpCreateFile;
ICMPCLOSEHANDLE IcmpCloseHandle;
ICMPSENDECHO IcmpSendEcho;

void CNewPingDlg::OnBUTTONIcmpDll()
{
    //实施PING操作
    UpdateData(TRUE);

    if (m_strDestIP.GetLength() < 7) {
        MessageBox("“目的主机IP地址”格式不正确");
        return;
    }

    //打开ICMP.dll文件。
    ULONG ulDestIP = (ULONG)inet_addr(m_strDestIP);
    HMODULE hInst = LoadLibrary(".\\ICMP.dll");

    if (hInst == NULL) {
        MessageBox("库文件不存在，或不是有效的DLL文件。");
        return;
    }

    //定位三个要调用的函数位置。
    IcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(hInst, "IcmpCreateFile");
    IcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(hInst, "IcmpCloseHandle");
    IcmpSendEcho = (ICMPSENDECHO)GetProcAddress(hInst, "IcmpSendEcho");

    if (IcmpCreateFile == NULL || IcmpCloseHandle == NULL ||
        IcmpSendEcho == NULL) {
        MessageBox("DLL文件中不存在所需要的三个函数，可能ICMP2.dll文件格式或版本有错。");
        return;
    }

    //打开ICMP操作。
    HANDLE hIcmp = IcmpCreateFile();

    if (hIcmp == INVALID_HANDLE_VALUE) {
        MessageBox("打开ICMP句柄出错。");
        FreeLibrary(hInst);
        return;
    }

    //初始化一些参数。
    //发送参数清零。
    IP_OPTION_INFORMATION IpOption;
    ZeroMemory(&IpOption, sizeof(IP_OPTION_INFORMATION));
    //假设发送的数据，Windows的ping程序默认发送的是“abcde..xyzabcdefghi”。
    char *SendData = "The quick brown fox jumps over a lazy dog.";
    //接收缓冲区，以及接收缓冲区的指针。
    char ReplyBuff[500] = {0};
    PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuff;
    //目的主机有可能开机，有可能没开机。
    //因此判断到目的主机和返回IP为同一个，则认为是成功；
    //判断到超时，则结束，视为没有成功。
    BOOL bLoop = TRUE; //由于事先不知道中间的路由数有多少，所以当遇到目的地时退出。
    CString strTemp, strEcho;

    while (bLoop) {
        //设置TTL值。
        IpOption.Ttl++;
        strTemp.Format("%d", IpOption.Ttl);
        m_ctlListEcho.InsertItem(0, strTemp); //LIST_ECHO_TTL

        if (IcmpSendEcho(hIcmp, ulDestIP, SendData, sizeof(SendData), &IpOption, ReplyBuff, sizeof(ReplyBuff), 3000) != 0) {
            //耗时。
            if (pEchoReply->RoundTripTime == 0) {
                strTemp = "<1 ms";
            } else {
                strTemp.Format("%d ms", pEchoReply->RoundTripTime);
            }

            m_ctlListEcho.SetItemText(0, LIST_ECHO_MS, strTemp);

            if (pEchoReply->Address == ulDestIP) {
                m_strComment.Format("Trace结束。达到了最终目地的");
                bLoop = FALSE;
            }
        } else {
            m_strComment.Format("Trace因超时而退出。未能达到最终目地的");
            m_ctlListEcho.SetItemText(0, LIST_ECHO_MS, "超时");
            bLoop = FALSE;
        }

        struct in_addr addr;

        addr.S_un.S_addr = pEchoReply->Address;

        strTemp.Format("%s", inet_ntoa(addr));

        m_ctlListEcho.SetItemText(0, LIST_ECHO_IP, strTemp);

        Sleep(100);
    }

    //写入日志，并关闭
    IcmpCloseHandle(hIcmp);
    FreeLibrary(hInst);
    UpdateData(FALSE);
}

void CNewPingDlg::OnBUTTONClear()
{
    // TODO: Add your control notification handler code here
    m_ctlListEcho.DeleteAllItems();
}
