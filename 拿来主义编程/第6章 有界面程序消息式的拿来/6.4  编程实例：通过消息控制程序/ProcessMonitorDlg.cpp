// ProcessMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProcessMonitor.h"
#include "ProcessMonitorDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

#include "Psapi.h"
#pragma comment(lib,"psapi.lib")

#define LIST_MONITOR_TYPE       0
#define LIST_MONITOR_PATHNAME   1
#define LIST_MONITOR_PROGRAM    2

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
// CProcessMonitorDlg dialog

CProcessMonitorDlg::CProcessMonitorDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CProcessMonitorDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CProcessMonitorDlg)
    m_iFrequence = 0;
    m_strProgram = _T("");
    m_bRunOrNot = FALSE;
    m_strPathname = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProcessMonitorDlg)
    DDX_Control(pDX, IDC_LIST_Monitor, m_ctlListMonitor);
    DDX_Control(pDX, IDC_TREE_Process, m_ctlTreeProcess);
    DDX_Text(pDX, IDC_EDIT_Frequence, m_iFrequence);
    DDV_MinMaxUInt(pDX, m_iFrequence, 5, 600);
    DDX_Text(pDX, IDC_EDIT_Program, m_strProgram);
    DDX_Check(pDX, IDC_CHECK_RunOrNot, m_bRunOrNot);
    DDX_Text(pDX, IDC_EDIT_Pathname, m_strPathname);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcessMonitorDlg, CDialog)
    //{{AFX_MSG_MAP(CProcessMonitorDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Enum, OnBUTTONEnum)
    ON_BN_CLICKED(IDC_BUTTON_TerminateProcess, OnBUTTONTerminateProcess)
    ON_BN_CLICKED(IDC_BUTTON_EnumWindow, OnBUTTONEnumWindow)
    ON_BN_CLICKED(IDC_BUTTON_Browser, OnBUTTONBrowser)
    ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
    ON_BN_CLICKED(IDC_BUTTON_Delete, OnBUTTONDelete)
    ON_BN_CLICKED(IDC_BUTTON_Monitor, OnBUTTONMonitor)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessMonitorDlg message handlers

BOOL CProcessMonitorDlg::OnInitDialog()
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
    m_ctlListMonitor.InsertColumn(LIST_MONITOR_TYPE, "类型", LVCFMT_LEFT, 60);
    m_ctlListMonitor.InsertColumn(LIST_MONITOR_PATHNAME, "程序", LVCFMT_LEFT, 140);
    m_ctlListMonitor.InsertColumn(LIST_MONITOR_PROGRAM, "程序", LVCFMT_LEFT, 0);
    m_ctlListMonitor.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    char buff[MAX_PATH * 4];
    //发现另存为另外目录时，再读INI文件出现问题。
    GetCurrentDirectory(MAX_PATH * 4, (char *)buff);
    m_strProgramDir.Format("%s\\ProcessMonitor.ini", buff);
    CString strTemp;
    int iCount = GetPrivateProfileInt("Monitor", "Count", 0, m_strProgramDir);

    for (int i = 0; i < iCount; i++) {
        strTemp.Format("Type%d", i + 1);
        GetPrivateProfileString("Monitor", strTemp, "", buff, MAX_PATH, m_strProgramDir);
        m_ctlListMonitor.InsertItem(i, buff); //LIST_MONITOR_TYPE
        strTemp.Format("Path%d", i + 1);
        GetPrivateProfileString("Monitor", strTemp, "", buff, MAX_PATH, m_strProgramDir);
        m_ctlListMonitor.SetItemText(i, LIST_MONITOR_PATHNAME, buff);
        strTemp.Format("List%d", i + 1);
        GetPrivateProfileString("Monitor", strTemp, "", buff, MAX_PATH, m_strProgramDir);
        m_ctlListMonitor.SetItemText(i, LIST_MONITOR_PROGRAM, buff);
    }

    m_iFrequence = GetPrivateProfileInt("Monitor", "Freq", 10, m_strProgramDir);
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProcessMonitorDlg::OnPaint()
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
HCURSOR CProcessMonitorDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CProcessMonitorDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CProcessMonitorDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
        CString strTemp1, strTemp2;
        int iCount = m_ctlListMonitor.GetItemCount();
        strTemp1.Format("%d", iCount);
        WritePrivateProfileString("Monitor", "Count", strTemp1, m_strProgramDir);

        for (int i = 0; i < iCount; i++) {
            strTemp1 = m_ctlListMonitor.GetItemText(i, LIST_MONITOR_TYPE);
            strTemp2.Format("Type%d", i + 1);
            WritePrivateProfileString("Monitor", strTemp2, strTemp1, m_strProgramDir);
            strTemp1 = m_ctlListMonitor.GetItemText(i, LIST_MONITOR_PATHNAME);
            strTemp2.Format("Path%d", i + 1);
            WritePrivateProfileString("Monitor", strTemp2, strTemp1, m_strProgramDir);
            strTemp1 = m_ctlListMonitor.GetItemText(i, LIST_MONITOR_PROGRAM);
            strTemp2.Format("List%d", i + 1);
            WritePrivateProfileString("Monitor", strTemp2, strTemp1, m_strProgramDir);
        }

        strTemp1.Format("%d", m_iFrequence);
        WritePrivateProfileString("Monitor", "Freq", strTemp1, m_strProgramDir);
        CDialog::OnCancel();
    }
}

#define MAXPROCESSNUM   10000

typedef struct _IO_COUNTERS {
    ULONGLONG  ReadOperationCount;
    ULONGLONG  WriteOperationCount;
    ULONGLONG  OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
} IO_COUNTERS, *PIO_COUNTERS;
typedef BOOL(WINAPI * GETPROCESSIOCOUNTERS)(HANDLE hProcess, PIO_COUNTERS lpIoCounters);
GETPROCESSIOCOUNTERS GetProcessIoCounters = NULL;

void CProcessMonitorDlg::OnBUTTONEnum()
{
    // TODO: Add your control notification handler code here
    m_strFullProcessName = ";";
    m_ctlTreeProcess.DeleteAllItems();
    DWORD dwPID[MAXPROCESSNUM * sizeof(DWORD)];
    int iPIDLen = MAXPROCESSNUM * sizeof(DWORD);
    int iPIDNeedLen = 0;

    if (!EnumProcesses((DWORD *)dwPID, iPIDLen, (DWORD *)&iPIDNeedLen))
        return;

    HINSTANCE h = GetModuleHandle("Kernel32.dll");
    GetProcessIoCounters = (GETPROCESSIOCOUNTERS)GetProcAddress(h, "GetProcessIoCounters");
    CString strTemp;
    int iCountP = (int)iPIDNeedLen / sizeof(DWORD);
    char strNameBuff[MAX_PATH];

    for (int i = 0; i < iCountP; i++) {
        strTemp.Format("进程ID：(%d)", dwPID[i]);
        HTREEITEM treePID = m_ctlTreeProcess.InsertItem(strTemp, TVI_ROOT);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwPID[i]);

        if (hProcess == NULL)
            continue;

        DWORD iNameLen = GetModuleBaseName(hProcess, NULL, strNameBuff, MAX_PATH);

        if (iNameLen > 0) {
            strTemp.Format("%s[%d];", strNameBuff, dwPID[i]);
            m_strFullProcessName += strTemp;
            strTemp.Format("进程名称：%s(%d)", strNameBuff, dwPID[i]);
            m_ctlTreeProcess.SetItemText(treePID, strTemp);
            strTemp.Format("进程ID：%d", dwPID[i]);
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
        }

        //进程的全路径名。
        iNameLen = GetModuleFileNameEx(hProcess, NULL, strNameBuff, MAX_PATH);

        if (iNameLen > 0) {
            strTemp.Format("进程名称：%s", strNameBuff);
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
        }

        //进程的版本。----------------------------------------------
        DWORD dwVersion = GetProcessVersion(dwPID[i]);

        if (dwVersion != 0) {
            union DWORD2WORD {
                WORD w[2];
                DWORD dw;
            } dw2w;
            dw2w.dw = dwVersion;
            strTemp.Format("版本号：%d.%d", dw2w.w[1], dw2w.w[0]);
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
        }

        //进程地址。----------------------------------------------
        PROCESS_MEMORY_COUNTERS pmc;

        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            strTemp = "进程内存地址";
            HTREEITEM treePID2 = m_ctlTreeProcess.InsertItem(strTemp, treePID);
            strTemp.Format("PageFaultCount: 0x%08X", pmc.PageFaultCount);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("PeakWorkingSetSize: 0x%08X", pmc.PeakWorkingSetSize);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("WorkingSetSize: 0x%08X", pmc.WorkingSetSize);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("QuotaPeakPagedPoolUsage: 0x%08X", pmc.QuotaPeakPagedPoolUsage);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("QuotaPagedPoolUsage: 0x%08X", pmc.QuotaPagedPoolUsage);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("QuotaPeakNonPagedPoolUsage: 0x%08X", pmc.QuotaPeakNonPagedPoolUsage);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("QuotaNonPagedPoolUsage: 0x%08X", pmc.QuotaNonPagedPoolUsage);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("PagefileUsage: 0x%08X", pmc.PagefileUsage);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
            strTemp.Format("PeakPagefileUsage: 0x%08X", pmc.PeakPagefileUsage);
            m_ctlTreeProcess.InsertItem(strTemp, treePID2);
        }

        //进程的时间。----------------------------------------------
        FILETIME ct, et, kt, ut;

        if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut)) {
            SYSTEMTIME sct, set, skt, sut;
            FileTimeToSystemTime(&ct, &sct);
            FileTimeToSystemTime(&et, &set);
            FileTimeToSystemTime(&kt, &skt);
            FileTimeToSystemTime(&ut, &sut);
            strTemp.Format("进程创建时间：%d-%d-%d %d:%d:%d",
                           sct.wYear, sct.wMonth, sct.wDay, sct.wHour, sct.wMinute, sct.wSecond);
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
            CTime tm1 = CTime::GetCurrentTime();
            CTime tm2(sct);
            CTimeSpan ts(0, 8, 0, 0);
            tm2 += ts;
            ts = tm1 - tm2;
            strTemp.Format("进程执行时间：%d:%d:%d",
                           ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
            /*treePID=m_ctlTreeProcess.InsertItem("进程时间",treePID);
            strTemp.Format("创建时间：%d-%d-%d %d:%d:%d",
                sct.wYear,sct.wMonth,sct.wDay,sct.wHour,sct.wMinute,sct.wSecond);
            m_ctlTreeProcess.InsertItem(strTemp,treePID);
            strTemp.Format("退出时间：%d-%d-%d %d:%d:%d",
                set.wYear,set.wMonth,set.wDay,set.wHour,set.wMinute,set.wSecond);
            m_ctlTreeProcess.InsertItem(strTemp,treePID);
            strTemp.Format("核心态时间：%d-%d-%d %d:%d:%d",
                skt.wYear,skt.wMonth,skt.wDay,skt.wHour,skt.wMinute,skt.wSecond);
            m_ctlTreeProcess.InsertItem(strTemp,treePID);
            strTemp.Format("用户态时间：%d-%d-%d %d:%d:%d",
                sut.wYear,sut.wMonth,sut.wDay,sut.wHour,sut.wMinute,sut.wSecond);
            m_ctlTreeProcess.InsertItem(strTemp,treePID);*/
        }

        //读写操作的计数。----------------------------------------------
        if (GetProcessIoCounters == NULL) {
            IO_COUNTERS io;

            if (GetProcessIoCounters(hProcess, &io)) {
                strTemp.Format("读、写、其它操作：%d，%d，%d",
                               io.ReadOperationCount, io.WriteOperationCount, io.OtherOperationCount);
                m_ctlTreeProcess.InsertItem(strTemp, treePID);
                strTemp.Format("通信中，读、写、其它操作：%d，%d，%d",
                               io.ReadTransferCount, io.WriteTransferCount, io.OtherTransferCount);
                m_ctlTreeProcess.InsertItem(strTemp, treePID);
            }
        }

        //读窗口标题。----------------------------------------------
        if (::GetWindowText((HWND)hProcess, strNameBuff, MAX_PATH) > 0) {
            strTemp.Format("名称：%s", strNameBuff);
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
        }

        //读窗口标题。----------------------------------------------
        if (::GetClassName((HWND)hProcess, strNameBuff, MAX_PATH) > 0) {
            strTemp.Format("类名：%s", strNameBuff);
            m_ctlTreeProcess.InsertItem(strTemp, treePID);
        }

        //所支持的模块。----------------------------------------------
        treePID = m_ctlTreeProcess.InsertItem("支持的模块DLL", treePID);
        HMODULE hModule[MAXPROCESSNUM * sizeof(HANDLE)];
        DWORD dwModuleLen = MAXPROCESSNUM * sizeof(HANDLE);
        DWORD dwModuleNeedLen = 0;

        if (!EnumProcessModules(hProcess, (HMODULE *)hModule, dwModuleLen,
                                (DWORD *)&dwModuleNeedLen))
            continue;

        int iCountM = dwModuleNeedLen / sizeof(HANDLE);

        for (int j = 0; j < iCountM; j++) {
            iNameLen = GetModuleFileName(hModule[j], strNameBuff, MAX_PATH);

            if (iNameLen > 0)
                strTemp.Format("支持模块名%s(%d)", strNameBuff, hModule[j]);
            else
                strTemp.Format("支持模块ID(%d)", hModule[j]);

            m_ctlTreeProcess.InsertItem(strTemp, treePID);
            //模块的地址。----------------------------------------------
            MODULEINFO ModuleInfo;

            if (GetModuleInformation(hProcess, hModule[j], &ModuleInfo, sizeof(ModuleInfo))) {
                strTemp.Format("BaseOfDll：%d", ModuleInfo.lpBaseOfDll);
                m_ctlTreeProcess.InsertItem(strTemp, treePID);
                strTemp.Format("SizeOfImage：%d", ModuleInfo.SizeOfImage);
                m_ctlTreeProcess.InsertItem(strTemp, treePID);
                strTemp.Format("EntryPoint：%d", ModuleInfo.EntryPoint);
                m_ctlTreeProcess.InsertItem(strTemp, treePID);
            }
        }

        CloseHandle(hProcess);
    }

    m_strFullProcessName.MakeLower();
    //MessageBox(m_strFullProcessName);
}

void CProcessMonitorDlg::OnBUTTONTerminateProcess()
{
    // TODO: Add your control notification handler code here
    HTREEITEM treeHandle = m_ctlTreeProcess.GetSelectedItem();
    CString strItem = m_ctlTreeProcess.GetItemText(treeHandle);
    int iPosStart = strItem.Find("(");
    int iPosEnd = strItem.Find(")");

    if (iPosStart < 0 || iPosEnd < 0) {
        MessageBox("只有根结点才能删除。");
        return;
    }

    CString strID = strItem.Mid(iPosStart + 1, iPosEnd - iPosStart - 1);
    TerminateProcessByID(strID);
}


void CProcessMonitorDlg::TerminateProcessByID(CString strID)
{
    DWORD dwPID = (DWORD)atoi(strID);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwPID);

    if (hProcess == NULL)
        return;

    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
}


CTreeCtrl *g_pList = NULL;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
void CProcessMonitorDlg::OnBUTTONEnumWindow()
{
    // TODO: Add your control notification handler code here
    g_pList = (CTreeCtrl *)&m_ctlTreeProcess;
    g_pList->DeleteAllItems();
    int iCount = 0x12345;
    EnumWindows(EnumWindowsProc, (LPARAM)&iCount);
}

typedef struct tagWINDOWINFO {
    DWORD cbSize;
    RECT  rcWindow;
    RECT  rcClient;
    DWORD dwStyle;
    DWORD dwExStyle;
    DWORD dwWindowStatus;
    UINT  cxWindowBorders;
    UINT  cyWindowBorders;
    ATOM  atomWindowType;
    WORD  wCreatorVersion;
} WINDOWINFO, *PWINDOWINFO, *LPWINDOWINFO;
typedef BOOL(WINAPI * GETWINDOWINFO)(HWND hwnd, PWINDOWINFO pwi);
GETWINDOWINFO GetWindowInfo;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    DWORD dwID = GetWindowThreadProcessId(hwnd, NULL);
    CString strTemp;
    strTemp.Format("线程ID：%d，窗口ID：%d", hwnd, dwID);
    HTREEITEM treeItem = g_pList->InsertItem(strTemp, TVI_ROOT);
    WINDOWINFO wi;
    HINSTANCE h = GetModuleHandle("user32.dll");
    GetWindowInfo = (GETWINDOWINFO)GetProcAddress(h, "GetWindowInfo");

    if (GetWindowInfo == NULL)
        return FALSE;

    if (GetWindowInfo(hwnd, &wi)) {
        char strNameBuff[MAX_PATH] = {0};
        GetWindowText((HWND)dwID, strNameBuff, MAX_PATH);
        strTemp.Format("窗口名称：%s", strNameBuff);
        g_pList->InsertItem(strTemp, treeItem);
        strTemp.Format("窗口坐标：(%d,%d)-(%d,%d)", wi.rcWindow.left,
                       wi.rcWindow.top, wi.rcWindow.right, wi.rcWindow.bottom);
        g_pList->InsertItem(strTemp, treeItem);
        strTemp.Format("客户区坐标：(%d,%d)-(%d,%d)", wi.rcClient.left,
                       wi.rcClient.top, wi.rcClient.right, wi.rcClient.bottom);
        g_pList->InsertItem(strTemp, treeItem);
        strTemp.Format("窗口状态 ：%d", wi.dwWindowStatus); //==WS_ACTIVECAPTION?"当前窗口":"");
        g_pList->InsertItem(strTemp, treeItem);
        strTemp.Format("窗口边界宽度%d，边界高度%d", wi.cxWindowBorders, wi.cyWindowBorders);
        g_pList->InsertItem(strTemp, treeItem);
        strTemp.Format("窗口版本%d", wi.wCreatorVersion);
        g_pList->InsertItem(strTemp, treeItem);
    }

    return TRUE;
}

void CProcessMonitorDlg::OnBUTTONBrowser()
{
    // TODO: Add your control notification handler code here
    CFileDialog dlg(TRUE, "*.*", NULL, NULL, "实时监看文件 (*.exe)|*.exe|所有文件 (*.*)|*.*||");

    if (dlg.DoModal() == IDOK) {
        UpdateData(TRUE);
        m_strPathname = dlg.GetPathName();
        m_strProgram = dlg.GetFileName();
        UpdateData(FALSE);
    }
}

void CProcessMonitorDlg::OnBUTTONAdd()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if (m_strProgram == "") {
        MessageBox("没有选择要监控的进程名。");
        return;
    }

    int iCount = m_ctlListMonitor.GetItemCount();
    m_ctlListMonitor.InsertItem(iCount, m_bRunOrNot ? "必须运行" : "禁止运行"); //LIST_MONITOR_TYPE
    m_ctlListMonitor.SetItemText(iCount, LIST_MONITOR_PATHNAME, m_strPathname);
    m_ctlListMonitor.SetItemText(iCount, LIST_MONITOR_PROGRAM, m_strProgram);
    UpdateData(FALSE);
}

void CProcessMonitorDlg::OnBUTTONDelete()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_ctlListMonitor.GetFirstSelectedItemPosition();

    if (pos == NULL)
        return;

    int index = m_ctlListMonitor.GetNextSelectedItem(pos);

    if (MessageBox("确实要删除吗?", "操作提示", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
        m_ctlListMonitor.DeleteItem(index);
    }
}

void CProcessMonitorDlg::OnBUTTONMonitor()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CString strTitle;
    this->GetDlgItem(IDC_BUTTON_Monitor)->GetWindowText(strTitle);

    if (strTitle == "开始监控") {
        if (m_iFrequence <= 3) {
            MessageBox("监控周期最少为5秒。");
            return;
        }

        KillTimer(1);
        SetTimer(1, m_iFrequence * 1000, NULL);
        GetDlgItem(IDC_BUTTON_Monitor)->SetWindowText("停止监控");
    } else {
        KillTimer(1);
        GetDlgItem(IDC_BUTTON_Monitor)->SetWindowText("开始监控");
    }
}

void CProcessMonitorDlg::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    switch (nIDEvent) {
    case 1: {
        OnBUTTONEnum();
        int iCount = m_ctlListMonitor.GetItemCount();

        for (int i = 0; i < iCount; i++) {
            CString strTemp;
            strTemp = m_ctlListMonitor.GetItemText(i, LIST_MONITOR_TYPE);

            if (strTemp == "必须运行") {
                //如果没找到则运行。
                strTemp = ";" + m_ctlListMonitor.GetItemText(i, LIST_MONITOR_PROGRAM) + "[";
                strTemp.MakeLower();

                if (m_strFullProcessName.Find(strTemp) < 0) {
                    strTemp = m_ctlListMonitor.GetItemText(i, LIST_MONITOR_PATHNAME);
                    ShellExecute(NULL, "open", strTemp, "", NULL, SW_SHOW);
                }
            }

            if (strTemp == "禁止运行") {
                //如果找到了则禁止运行。
                strTemp = ";" + m_ctlListMonitor.GetItemText(i, LIST_MONITOR_PROGRAM) + "[";
                strTemp.MakeLower();
                int iPos1 = m_strFullProcessName.Find(strTemp);

                if (iPos1 >= 0) {
                    iPos1 += strTemp.GetLength();
                    int iPos2 = m_strFullProcessName.Find("];", iPos1);
                    strTemp = m_strFullProcessName.Mid(iPos1, iPos2 - iPos1);
                    TerminateProcessByID(strTemp);
                }
            }
        }
    }
    break;

    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}
