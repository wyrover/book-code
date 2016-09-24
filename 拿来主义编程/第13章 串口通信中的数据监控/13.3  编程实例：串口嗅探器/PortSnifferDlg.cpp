// PortSnifferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PortSniffer.h"
#include "PortSnifferDlg.h"

#include "VSPE_API.h"
#pragma comment(lib,"VSPE_API.lib")

#include "PComm.h"
#pragma comment(lib,"PComm.lib")

//一些参数保存在INI文件中。
#define INIFILENAME ".\\PortSniffer.ini"
#define MAXBUFFLEN  2048

#define LIST_COMMAND_DIRECTION  0
#define LIST_COMMAND_TIME       1
#define LIST_COMMAND_COMMAND    2

//线程结构
typedef struct _threadParamStruct {
    int iIndex;
    int iCom;
    CString strIP;
    BOOL g_bRunning;
    CPortSnifferDlg *pDlg;
} THREADPARAM;
THREADPARAM g_sThreadParam;
BOOL g_bRunning;
UINT RealCOMThread(LPVOID pParam);

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
// CPortSnifferDlg dialog

CPortSnifferDlg::CPortSnifferDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CPortSnifferDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPortSnifferDlg)
    m_strComment = _T("");
    m_iComReal = 0;
    m_iComVirtual = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPortSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPortSnifferDlg)
    DDX_Control(pDX, IDC_LIST_Command, m_ctlListCommand);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    DDX_Text(pDX, IDC_EDIT_ComReal, m_iComReal);
    DDX_Text(pDX, IDC_EDIT_ComVirtual, m_iComVirtual);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPortSnifferDlg, CDialog)
    //{{AFX_MSG_MAP(CPortSnifferDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONStart, OnBUTTONStart)
    ON_BN_CLICKED(IDC_BUTTONDStop, OnBUTTONDStop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortSnifferDlg message handlers

BOOL CPortSnifferDlg::OnInitDialog()
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
    //整个程序的初始化。
    CString strTemp;
    m_ctlListCommand.InsertColumn(LIST_COMMAND_DIRECTION, "方向", LVCFMT_LEFT, 40);
    m_ctlListCommand.InsertColumn(LIST_COMMAND_TIME, "时间", LVCFMT_LEFT, 60);
    m_ctlListCommand.InsertColumn(LIST_COMMAND_COMMAND, "命令", LVCFMT_LEFT, 300);
    m_ctlListCommand.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    Init();//整个VSPE的初始化工作。
    m_bReading = FALSE; //串口不连好，则不需要进行监控。
    //两个实串口。
    m_iComVirtual = GetPrivateProfileInt("COM", "ComVirtual", 5, INIFILENAME);
    m_iComReal = GetPrivateProfileInt("COM", "ComReal", 3, INIFILENAME);
    m_strComment.Format("请输入要创建的虚拟串口，以及要连接的实串口。");
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPortSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPortSnifferDlg::OnPaint()
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
HCURSOR CPortSnifferDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CPortSnifferDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CPortSnifferDlg::OnCancel()
{
    //退出程序
    if (MessageBox("确实要退出吗?", "退出", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CString strTemp, strTemp2;
        UpdateData(TRUE);
        OnBUTTONDStop();
        //保存当前COM参数是最终的参数。
        strTemp.Format("%d", m_iComVirtual);
        WritePrivateProfileString("COM", "ComVirtual", strTemp, INIFILENAME);
        strTemp.Format("%d", m_iComReal);
        WritePrivateProfileString("COM", "ComReal", strTemp, INIFILENAME);
        Uninit();
        CDialog::OnCancel();
    }
}

void CPortSnifferDlg::Init()
{
    //VSPE必须通过激活码激活。
    const char* activationKey = "SRBGMZSYPuIHWILsmLjF5CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPRJEEYxlJg+4gdYguyYuMXkIPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9JyUaC2ZWE1DZV2+wYWlRm7FFYrW3MDbZg8MkQsOQ8r1IPIEvcZBgPQg8gS9xkGA9LzrhjimHDiMlKqr6pSiw9CDl9n+0bAgFr2ho7nXjCoTMHYzt4tsbEkJGNktLGVG42SZ63UbmIUNcKmfhSzXldVCLhfvZv3StR9c/vkYG471Nh62eC1qIYuBUvm+a3BK8iR0POD8w5ovtuYr0T8aQP3eh4b8lUwnPHG9NRJxerttq/+/zX7c++9LDSQym3ThbWesK+A+X/vNw9qDgYt1dsJxDEEytsCRiT7bTiV5Djh1RlpIwETXWA089hiE9OYd7GpjKLq5dQOqSVcA3Fg1Wfdbqn/yn8q0/AIDOd0iZlbVeLY68zKh1Di4gGEoa1kR8EOBp2mxeaFrfwUm3DsJ5Pc04f7aEw9XljfBUwl/bAs3LVH5HRii8lXZvUVvnnfpcQ==1F250CF0960AE1C09E9450C816DE1232"; // <-----  PUT ACTIVATION KEY HERE
    CString strTemp;
    //激活VSPE API
    bool result = vspe_activate(activationKey);

    if (result == false) {
        strTemp.Format("激活失败，造成这一原则是驱动未安装或版本不对，当前版本是：%s",
                       vspe_getVersionInformation());//显示VSPE API的版本
        MessageBox(strTemp);
        return;
    }

    //初始化。
    result = vspe_initialize();

    if (result == false) {
        strTemp.Format("初始化失败。");
        MessageBox(strTemp);
        return;
    }

    //删除所有已创建的设备。
    vspe_destroyAllDevices();
    //停止所有正在运行的设备。
    result = vspe_stopEmulation();

    if (result == false) {
        strTemp.Format("出错:有的设备无法停止。请重试。");
        MessageBox(strTemp, "出错提示", MB_OK);
        return;
    }
}


void CPortSnifferDlg::Uninit()
{
    //释放VSPE。
    vspe_release();
}


UINT RealCOMThread(LPVOID pParam)
{
    //转发线程。
    THREADPARAM* threadparam = (THREADPARAM*)pParam;
    CPortSnifferDlg *ptr = threadparam->pDlg;
    CListCtrl *pListCommand = (CListCtrl *)&ptr->m_ctlListCommand;
    CString strTemp;

    while (g_bRunning) {
        //如果当前都没创建或没连接，则进入空转状态。
        if (!ptr->m_bReading) {
            Sleep(1000);
            continue;
        }

        //将两串口间数据进行转换。
        CString strTemp, strAll = "";
        UCHAR buff[MAXBUFFLEN] = {0};
        //读虚串口，转实串口
        int len = sio_read(ptr->m_iComVirtual, (char *)buff, MAXBUFFLEN);
        sio_flush(ptr->m_iComVirtual, 0);

        if (len > 0) {
            //写入到实串口。
            sio_write(ptr->m_iComReal, (char *)buff, len);
            //并将之显示到日志中。
            strAll = "";

            for (int c = 0; c < len; c++) {
                strTemp.Format("%02X ", buff[c]);
                strAll += strTemp;
            }

            ptr->WriteToList(TRUE, strAll);
        }

        //读实串口，转读串口
        len = sio_read(ptr->m_iComReal, (char *)buff, MAXBUFFLEN);
        sio_flush(ptr->m_iComReal, 0);

        if (len > 0) {
            //写入到虚串口。
            sio_write(ptr->m_iComVirtual, (char *)buff, len);
            //并将之显示到日志中。
            strAll = "";

            for (int c = 0; c < len; c++) {
                strTemp.Format("%02X ", buff[c]);
                strAll += strTemp;
            }

            ptr->WriteToList(FALSE, strAll);
        }

        Sleep(100);
    }

    return 0;
}

void CPortSnifferDlg::OnBUTTONStart()
{
    //开启，依次完成5个操作，1连实串口，2创建VSPE，
    //3运行服务，4连接虚串口，5运行转发线程。
    UpdateData(TRUE);
    CString strTemp;
    //1连实串口
    strTemp = ComConnect(m_iComReal);

    if (strTemp != "") {
        MessageBox(strTemp, "出错提示", MB_OK);
        return;
    }

    //2创建“连接器”。
    strTemp.Format("%d:0", m_iComVirtual);
    int deviceId = vspe_createDevice("Connector", strTemp);

    if (deviceId == -1) {
        strTemp.Format("错误：无法创建设备。");
        MessageBox(strTemp);
        vspe_release();
        return;
    }

    //3运行服务
    bool result = vspe_startEmulation();

    if (result == false) {
        strTemp.Format("启动失败，请重新运行。");
        MessageBox(strTemp);
        return;
    }

    //4连接虚串口。
    strTemp = ComConnect(m_iComVirtual);

    if (strTemp != "") {
        MessageBox(strTemp, "出错提示", MB_OK);
        return;
    }

    //5开始读取
    g_bRunning = TRUE;
    g_sThreadParam.pDlg = this;
    AfxBeginThread(RealCOMThread, &g_sThreadParam, THREAD_PRIORITY_NORMAL);
    Sleep(100);
    m_bReading = TRUE; //可以开始读取操作了。
    m_strComment.Format("启动成功，进入监听状态。");
    UpdateData(FALSE);
}

void CPortSnifferDlg::OnBUTTONDStop()
{
    //依次1停转换线程2关实串口3关虚串口
    //4停服务5删除设备
    UpdateData(TRUE);
    CString strTemp;
    //1停止转换线程。
    m_bReading = FALSE;
    g_bRunning = FALSE;
    Sleep(1000);
    //2，3关实、虚串口。
    ComDisconnect(m_iComReal);
    ComDisconnect(m_iComVirtual);
    //4停止当前的VSPE服务。
    bool result = vspe_stopEmulation();

    if (result == false) {
        strTemp.Format("错误:服务无法停止，请重新操作。");
        MessageBox(strTemp);
        return;
    }

    //5删除所有已创建的设备。
    vspe_destroyAllDevices();
    m_strComment.Format("停止成功，退出监听状态。");
    UpdateData(FALSE);
}

CString CPortSnifferDlg::ComConnect(int iCom)
{
    //连接串口，不管是实串口，还是虚串口。
    //返回值如果为空串，则表示操作正常，否则表示错误信息。
    UpdateData(TRUE);
    CString strTemp = "";

    if (sio_open(iCom) != SIO_OK) {
        strTemp.Format("端口COM%d打开失败。", iCom);
    } else {
        if (sio_ioctl(iCom, B38400, BIT_8 | STOP_1 | P_NONE) != SIO_OK) {
            ComDisconnect(iCom);
            strTemp.Format("端口COM%d打开成功，但参数设置出错。", iCom);
        } else {
            if (sio_flowctrl(iCom, 0) != SIO_OK) {
                ComDisconnect(iCom);
                strTemp.Format("端口COM%d打开成功，但流控设置出错。", iCom);
            }
        }
    }

    return strTemp;
}

CString CPortSnifferDlg::ComDisconnect(int iCom)
{
    //关闭串口，不管是实串口，还是虚串口。
    //返回值如果为空串，则表示操作正常，否则表示错误信息。
    UpdateData(FALSE);
    sio_close(m_iComReal);
    return "";
}

void CPortSnifferDlg::WriteToList(BOOL bDir, CString strMsg)
{
    //将转发的信息写入到日志中。
    int iCount = m_ctlListCommand.GetItemCount();

    if (bDir)
        m_ctlListCommand.InsertItem(iCount, "发出"); //LIST_COMMAND_DIRECTION
    else
        m_ctlListCommand.InsertItem(iCount, "接入"); //LIST_COMMAND_DIRECTION

    m_ctlListCommand.SetItemText(iCount, LIST_COMMAND_COMMAND, strMsg);
    CTime time = CTime::GetCurrentTime();
    m_ctlListCommand.SetItemText(iCount, LIST_COMMAND_TIME, time.Format("%H:%M:%S"));
    //m_ctlListCommand.SetItemData(iCount,(DWORD)len);
}
