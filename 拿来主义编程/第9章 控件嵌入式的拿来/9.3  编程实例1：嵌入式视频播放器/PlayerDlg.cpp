// PlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Player.h"
#include "PlayerDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//一些参数保存在INI文件中。
CString g_strIniFilename;//设定一个全局变量保存INI文件的位置。

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
// CPlayerDlg dialog

CPlayerDlg::CPlayerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CPlayerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPlayerDlg)
    m_strFilename = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPlayerDlg)
    DDX_Control(pDX, IDC_VLCPLUGIN1, m_ctlVLC);
    DDX_Text(pDX, IDC_EDIT_Filename, m_strFilename);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
    //{{AFX_MSG_MAP(CPlayerDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONPlay, OnBUTTONPlay)
    ON_BN_CLICKED(IDC_BUTTONStop, OnBUTTONStop)
    ON_BN_CLICKED(IDC_BUTTONPause, OnBUTTONPause)
    ON_BN_CLICKED(IDC_BUTTONBrowser, OnBUTTONBrowser)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers

BOOL CPlayerDlg::OnInitDialog()
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
    m_bPause = FALSE;
    CString strTemp;
    char buff[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, (char *)buff); //在系统启动的时候读当前路径
    g_strIniFilename.Format("%s\\Player.ini", buff); //记为绝对路径
    GetPrivateProfileString("Init", "Filename", "", buff, MAX_PATH, g_strIniFilename);
    m_strFilename.Format("%s", buff);
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlayerDlg::OnPaint()
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
HCURSOR CPlayerDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CPlayerDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CPlayerDlg::OnCancel()
{
    //退出系统
    if (MessageBox("确实要退出吗?", "退出", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CString strTemp;
        UpdateData(TRUE);
        WritePrivateProfileString("Init", "Filename", m_strFilename, g_strIniFilename);
        CDialog::OnCancel();
    }
}

void CPlayerDlg::OnBUTTONPlay()
{
    //播放按钮
    UpdateData(TRUE);
    CVLCPlaylist list = m_ctlVLC.GetPlaylist();
    m_ctlVLC.SetAutoPlay(TRUE);
    VARIANT va1, va2;
    VariantInit(&va1);
    VariantInit(&va2);
    //MessageBox(m_strFilename);
    //m_ctlVLC.addt
    list.add(m_strFilename, va1, va2);
    //list.play();
    list.next();
    Sleep(100);
}

void CPlayerDlg::OnBUTTONStop()
{
    //停止按钮
    CVLCPlaylist list = m_ctlVLC.GetPlaylist();
    list.stop();
}

void CPlayerDlg::OnBUTTONPause()
{
    //暂停按钮
    CVLCPlaylist list = m_ctlVLC.GetPlaylist();
    list.togglePause();

    if (m_bPause)
        GetDlgItem(IDC_BUTTONPause)->SetWindowText("暂停");
    else
        GetDlgItem(IDC_BUTTONPause)->SetWindowText("恢复");

    m_bPause = !m_bPause;
}

void CPlayerDlg::OnBUTTONBrowser()
{
    //浏览按钮
    char szFileFilter[] =
        "Mp3 File(*.mp3)|*.mp3|"
        "MPEG File(*.mpeg)|*.mpeg|"
        "Media File(*.asf)|*.asf|"
        "Video File(*.dat)|*.dat|"
        "MPGA File(*.mpga)|*.mpga|"
        "Wave File(*.wav)|*.wav|"
        "AVI File(*.avi)|*.avi|"
        "Movie File(*.mov)|*.mov|"
        "Mid File(*.mid;*,rmi)|*.mid;*.rmi|"
        "Wma File(*.wma)|*.wma|"
        "Wmv File(*.wmv)|*.wmv|"
        "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE, "*.*", "*.*", NULL, szFileFilter);

    if (dlg.DoModal() == IDOK) {
        UpdateData(TRUE);
        m_strFilename = dlg.GetPathName();
        UpdateData(FALSE);
    }
}
