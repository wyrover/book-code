// ReadMemoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadMemory.h"
#include "ReadMemoryDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//线程结构
typedef struct _threadParamStruct {
    BOOL bRunning;
    CReadMemoryDlg *pDlg;
} THREADPARAM;

THREADPARAM g_threadParam;
UINT DebugThread(LPVOID pParam);

#define LIST_DEBUG_TYPE     0
#define LIST_DEBUG_PROCESS  1
#define LIST_DEBUG_THREAD   2
#define LIST_DEBUG_CONTENT  3

typedef HANDLE(WINAPI *OPENTHREAD)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
OPENTHREAD OpenThread;

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
// CReadMemoryDlg dialog

CReadMemoryDlg::CReadMemoryDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CReadMemoryDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CReadMemoryDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReadMemoryDlg)
    DDX_Control(pDX, IDC_LIST_Content, m_ctlListContent);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReadMemoryDlg, CDialog)
    //{{AFX_MSG_MAP(CReadMemoryDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONOpen, OnBUTTONOpen)
    ON_BN_CLICKED(IDC_BUTTON_DebugStart, OnBUTTONDebugStart)
    ON_BN_CLICKED(IDC_BUTTON_DebugStop, OnBUTTONDebugStop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadMemoryDlg message handlers

BOOL CReadMemoryDlg::OnInitDialog()
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
    m_ctlListContent.InsertColumn(LIST_DEBUG_TYPE, "类型", LVCFMT_LEFT, 160);
    m_ctlListContent.InsertColumn(LIST_DEBUG_PROCESS, "进程ID", LVCFMT_LEFT, 60);
    m_ctlListContent.InsertColumn(LIST_DEBUG_THREAD, "线程ID", LVCFMT_LEFT, 60);
    m_ctlListContent.InsertColumn(LIST_DEBUG_CONTENT, "内容", LVCFMT_LEFT, 180);
    m_ctlListContent.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    HMODULE hInst = LoadLibrary("kernel32.dll");

    if (hInst == NULL) {
        MessageBox("系统议论kernel32.dll库文件不存在。");
        return;
    }

    //定位三个要调用的函数位置。
    OpenThread = (OPENTHREAD)GetProcAddress(hInst, "OpenThread");

    if (OpenThread == NULL) {
        MessageBox("当前操作系统与该程序不兼容。");
        return;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReadMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReadMemoryDlg::OnPaint()
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
HCURSOR CReadMemoryDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CReadMemoryDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CReadMemoryDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("确实要退出吗?", "退出",
                   MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CDialog::OnCancel();
    }
}


void CReadMemoryDlg::OnBUTTONOpen()
{
    //打开程序。
    ShellExecute(NULL, "open", ".\\WindowsControl.exe", NULL, NULL, SW_SHOW);
}

void CReadMemoryDlg::OnBUTTONDebugStart()
{
    //启动调试线程。
    g_threadParam.bRunning = TRUE;
    g_threadParam.pDlg = this;
    AfxBeginThread(DebugThread, &g_threadParam, THREAD_PRIORITY_NORMAL);
    Sleep(100);
}

UINT DebugThread(LPVOID pParam)
{
    //调试线程。
    THREADPARAM* threadparam = (THREADPARAM*)pParam;
    CReadMemoryDlg *ptr = threadparam->pDlg;
    CListCtrl *pList = &ptr->m_ctlListContent;
    CString strTemp;
    //找到要调试程序。
    CString szCaption = "Windows标准控件示意程序"; //窗体标题
    CWnd *pWnd = ptr->FindWindow(NULL, szCaption);

    if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL) {
        AfxMessageBox("链接窗口失败!");
        return 1;
    }

    //获得进程ID，进而用DebugActiveProcess打开该线程。
    DWORD dwID;//进程ID
    HWND hHWND = pWnd->GetSafeHwnd();
    GetWindowThreadProcessId(hHWND, &dwID);
    DebugActiveProcess(dwID);
    //由于调试程序中，用于句柄，所以由ID转为句柄。
    HANDLE hProcess =::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);

    if (hProcess == NULL) {
        strTemp.Format("OpenProcess失败，GetLastError=%d\n", GetLastError());
        AfxMessageBox(strTemp);
        return 2;
    }

    //进入调试状态。
    pList->DeleteAllItems();
    int iListIndex = 0;
    BOOL bRet = TRUE;
    DEBUG_EVENT DebugEv;

    while (g_threadParam.bRunning) {
        //接收事件，无事件则空转。
        bRet = WaitForDebugEvent(&DebugEv, 200);

        if (!bRet) {
            Sleep(200);
            continue;
        }

        //有事件，则开始事件识别。
        pList->InsertItem(iListIndex, "");
        strTemp.Format("%d", DebugEv.dwProcessId);
        pList->SetItemText(iListIndex, LIST_DEBUG_PROCESS, strTemp);
        strTemp.Format("%d", DebugEv.dwThreadId);
        pList->SetItemText(iListIndex, LIST_DEBUG_THREAD, strTemp);

        //根据不同事件，分别处理各种异常。
        switch (DebugEv.dwDebugEventCode) {
        case EXCEPTION_DEBUG_EVENT: { //如果是异常事件。
            PEXCEPTION_RECORD ptr = &DebugEv.u.Exception.ExceptionRecord;
            strTemp.Format("EXCEPTION_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("异常码：%d", (UINT)ptr->ExceptionCode);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case CREATE_THREAD_DEBUG_EVENT: { //线程创建的事件。
            //调用GetThreadContext函数和SetThreadContext函数进行上下文操作，
            //或调用SuspendThread函数和ResumeThread函数挂起和恢复线程运行。
            LPCREATE_THREAD_DEBUG_INFO ptr = &DebugEv.u.CreateThread;
            strTemp.Format("CREATE_THREAD_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("开始地址：%d，基地址：%d，线程ID:%d", ptr->lpStartAddress, ptr->lpThreadLocalBase, ptr->hThread);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case CREATE_PROCESS_DEBUG_EVENT: //进程创建的事件。
            //除了像CREATE_THREAD_DEBUG_EVENT事件中调用那4个函数之外，
            //还可调用ReadProcessMemory函数和WriteProcessMemory函数进行
            //进程内存的读取和写入。
        {
            LPCREATE_PROCESS_DEBUG_INFO ptr = &DebugEv.u.CreateProcessInfo;
            strTemp.Format("CREATE_PROCESS_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("地址：%d", ptr->lpStartAddress);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
            //CREATE_PROCESS_DEBUG_INFO pi;
            /*char buff[1024]={0,1,2,3,4,5,6,7,8,9,10};
            ULONG len=0;
            //CREATE_PROCESS_DEBUG_INFO pi;
            bRet=ReadProcessMemory(hProcess,(char *)DebugEv.u.CreateProcessInfo.lpImageName,(char *)buff,4,&len);
            if (!bRet || len<=0)
            {
                strTemp.Format("%d %d %d",bRet,len,GetLastError());
                //AfxMessageBox(strTemp);
            }
            else
            {
                CString strAll;
                //strTemp.Format("%d",len);
                //MessageBox(strTemp);

                CFile file;
                file.Open("a.txt",CFile::modeCreate|CFile::modeReadWrite,NULL);
                for (ULONG i=0;i<len;i++)
                {
                    strTemp.Format("%02X ",(UCHAR)buff[i]);
                    strAll+=strTemp;
                }
                //file.Write((char *)buff,len);
                file.Write(strAll,strAll.GetLength());
                file.Close();
                AfxMessageBox("ok");
            }*/
        }
        break;

        case EXIT_THREAD_DEBUG_EVENT: { //线程退出的事件。
            //显示线程的退出码。
            LPEXIT_THREAD_DEBUG_INFO ptr = &DebugEv.u.ExitThread;
            strTemp.Format("EXIT_THREAD_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("退出码：%d", ptr->dwExitCode);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case EXIT_PROCESS_DEBUG_EVENT: { //进程退出的事件。
            //显示进程的退出码。
            LPEXIT_PROCESS_DEBUG_INFO ptr = &DebugEv.u.ExitProcess;
            strTemp.Format("EXIT_PROCESS_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("退出码：%d", ptr->dwExitCode);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case LOAD_DLL_DEBUG_EVENT: { //动态链接库截入的事件。
            //读取DLL文件的载入信息。
            LPLOAD_DLL_DEBUG_INFO ptr = &DebugEv.u.LoadDll;
            strTemp.Format("LOAD_DLL_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("地址:%d,名称：%s", ptr->lpBaseOfDll, ptr->lpImageName);
            /*HANDLE hThread=::OpenThread(THREAD_ALL_ACCESS, FALSE, DebugEv.dwThreadId);
            SuspendThread(hThread);

            char pImageNameBase[MAX_PATH];
            char LoadDllName[MAX_PATH];
            ReadProcessMemory(hProcess,ptr->lpImageName,pImageNameBase,4,NULL);
            ReadProcessMemory(hProcess,(PVOID)pImageNameBase,LoadDllName,MAX_PATH,NULL);
            strTemp.Format("%s %s",pImageNameBase,LoadDllName);*/
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case UNLOAD_DLL_DEBUG_EVENT: { //动态链接库卸载的事件。
            //显示卸载DLL文件的信息。
            LPUNLOAD_DLL_DEBUG_INFO ptr = &DebugEv.u.UnloadDll;
            strTemp.Format("UNLOAD_DLL_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("地址:%d", (UINT)ptr->lpBaseOfDll);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case OUTPUT_DEBUG_STRING_EVENT: { //显示调试信息的事件。
            //显示输出的调试信息串。
            LPOUTPUT_DEBUG_STRING_INFO ptr = &DebugEv.u.DebugString;
            strTemp.Format("OUTPUT_DEBUG_STRING_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("%s", ptr->lpDebugStringData);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        default: { //未定义消息的处理。
            strTemp.Format("未定义");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("未定义消息的处理");
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;
        }

        //从调试中恢复。
        ContinueDebugEvent(DebugEv.dwProcessId, DebugEv.dwThreadId, DBG_CONTINUE);
        iListIndex++;
    }//while循环结束

    CloseHandle(hProcess);
    strTemp.Format("线程退出。");
    AfxMessageBox(strTemp);
    return 0;
}

void CReadMemoryDlg::OnBUTTONDebugStop()
{
    //关闭线程
    g_threadParam.bRunning = FALSE;
}
