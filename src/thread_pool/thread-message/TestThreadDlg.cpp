
// TestThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestThread.h"
#include "TestThreadDlg.h"
#include <map>
#include <vector>
using namespace std;

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


// CTestThreadDlg dialog




CTestThreadDlg::CTestThreadDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTestThreadDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestThreadDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_CAPTION, m_stCaption);
}

BEGIN_MESSAGE_MAP(CTestThreadDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_CREATETHREAD, &CTestThreadDlg::OnBnClickedButtonCreatethread)
    ON_BN_CLICKED(IDC_BUTTON_THREAD_HELLO, &CTestThreadDlg::OnBnClickedButtonThreadHello)
    ON_BN_CLICKED(IDC_BUTTON_QUITTHREAD, &CTestThreadDlg::OnBnClickedButtonQuitthread)
    ON_MESSAGE(WM_APP + 2, &CTestThreadDlg::OnThreadQuitFunction)
END_MESSAGE_MAP()


// CTestThreadDlg message handlers

BOOL CTestThreadDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu* pSysMenu = GetSystemMenu(FALSE);

    if (pSysMenu != NULL) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);

        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    m_stCaption.SetWindowText(L"No Thread Running");
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestThreadDlg::OnPaint()
{
    if (IsIconic()) {
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
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestThreadDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

DWORD threadProc(LPVOID lParam)
{
    MSG msg;

    while (GetMessage(&msg, 0, 0, 0)) {
        if (msg.message == WM_APP + 1) {
            MessageBoxA(NULL, "Hello", "From Thread", MB_OK);
        }

        DispatchMessage(&msg);
    }

    MessageBoxA(NULL, "Thread will closed On pressing OK", "From Thread", MB_OK);
    AfxGetApp()->m_pMainWnd->PostMessage(WM_APP + 2, 0, 0);
    return 0;
}




void CTestThreadDlg::OnBnClickedButtonCreatethread()
{
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProc, 0, 0, &m_idThread);
    CString strTemp;
    strTemp.Format(L"Thread Started with id#%d", m_idThread);
    m_stCaption.SetWindowText(strTemp);
}

void CTestThreadDlg::OnBnClickedButtonThreadHello()
{
    PostThreadMessage(m_idThread, WM_APP + 1, 0, 0);
}

void CTestThreadDlg::OnBnClickedButtonQuitthread()
{
    PostThreadMessage(m_idThread, WM_QUIT, 0, 0);
}

LRESULT CTestThreadDlg::OnThreadQuitFunction(WPARAM wParam, LPARAM lParam)
{
    CString strTemp;
    strTemp.Format(L"Thread Closed with id#%d", m_idThread);
    m_stCaption.SetWindowTextW(strTemp);
    return LRESULT(0);
}
