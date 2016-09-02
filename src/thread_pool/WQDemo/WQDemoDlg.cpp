// WQDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WQDemo.h"
#include "WQDemoDlg.h"
#include "WorkQueue.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


class SpecificWorkItem : public WorkItemBase
{

public:
    SpecificWorkItem(CXListCtrl* pListCtrl, int nItem)
    {
        m_pListCtrl = pListCtrl;
        m_nItem = nItem;
    }
private:
    void   DoWork(void* pThreadContext);
    void   Abort();
    int    m_nItem;
    CXListCtrl* m_pListCtrl;

};

void SpecificWorkItem::DoWork(void* pThreadContext)
{
    m_pListCtrl->EnsureVisible(m_nItem, TRUE);
    m_pListCtrl->SetProgress(m_nItem, 0);

    for (int i = 0 ; i < 100 ; i++) {
        Sleep(100);
        m_pListCtrl->UpdateProgress(m_nItem, 0, i);
    }

    m_pListCtrl->DeleteProgress(m_nItem, 0);
    m_pListCtrl->SetItemText(m_nItem, 0, _T("Complete"));
    delete this;
}

void SpecificWorkItem::Abort()
{
    m_pListCtrl->SetItemText(m_nItem, 0, _T("Aborted"));
    delete this;
}

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
// CWQDemoDlg dialog

unsigned long __stdcall CWQDemoDlg::ThreadFuncDestroy(void*  pParam)
{
    CWQDemoDlg* pDlg = (CWQDemoDlg*)pParam;
    pDlg->m_WorkQueue.Destroy();
    return 1;
}


CWQDemoDlg::CWQDemoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CWQDemoDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CWQDemoDlg)
    m_nNumberOfThreads = 5;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWQDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWQDemoDlg)
    DDX_Control(pDX, IDC_ITEM_QUEUE_LIST, m_ItemsOueueList);
    DDX_Text(pDX, IDC_NUMBER_OF_THREADS_EDIT, m_nNumberOfThreads);
    DDV_MinMaxUInt(pDX, m_nNumberOfThreads, 0, 64);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWQDemoDlg, CDialog)
    //{{AFX_MSG_MAP(CWQDemoDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_CREATE, OnCreate)
    ON_BN_CLICKED(IDC_DESTROY, OnDestroy)
    ON_BN_CLICKED(IDC_ADD_ITEM_BTN, OnAddItemBtn)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWQDemoDlg message handlers

BOOL CWQDemoDlg::OnInitDialog()
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
    m_ItemsOueueList.InsertColumn(0, _T("Items Progress"), LVCFMT_LEFT, 195);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWQDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWQDemoDlg::OnPaint()
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
HCURSOR CWQDemoDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CWQDemoDlg::OnCreate()
{
    UpdateData();
    GetDlgItem(IDC_CREATE)->EnableWindow(FALSE);
    GetDlgItem(IDC_NUMBER_OF_THREADS_SPIN_SPIN)->EnableWindow(FALSE);
    GetDlgItem(IDC_DESTROY)->EnableWindow(TRUE);
    GetDlgItem(IDC_ADD_ITEM_BTN)->EnableWindow(TRUE);
    m_WorkQueue.Create(m_nNumberOfThreads);
}

void CWQDemoDlg::OnDestroy()
{
    GetDlgItem(IDC_DESTROY)->EnableWindow(FALSE);
    GetDlgItem(IDC_ADD_ITEM_BTN)->EnableWindow(FALSE);
    // in order not to freeze the GUI, I Create another thread to destroy the work queue
    // because the destroy waits for the already in process items to complete
    DWORD dwThreadId;
    HANDLE hThreade = CreateThread(NULL,
                                   0,
                                   CWQDemoDlg::ThreadFuncDestroy,
                                   this,
                                   0,
                                   &dwThreadId);

    while (TRUE) {
        // block-local variable
        DWORD result ;
        MSG msg ;

        // Read all of the messages in this next loop,
        // removing each message as we read it.
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // If it's a quit message, we're out of here.
            if (msg.message == WM_QUIT)
                return;

            // Otherwise, dispatch the message.
            DispatchMessage(&msg);
        } // End of PeekMessage while loop.

        // Wait for any message sent or posted to this queue
        // or for one of the passed handles be set to signaled.
        result = MsgWaitForMultipleObjects(1, &hThreade ,
                                           FALSE, INFINITE, QS_ALLINPUT);

        // The result tells us the type of event we have.
        if (result == WAIT_OBJECT_0) {
            // New messages have arrived.
            // Continue to the top of the always while loop to
            // dispatch them and resume waiting.
            break;
        } else {
            continue;
        } // End of else clause.
    } // End of the always while loop.

    GetDlgItem(IDC_CREATE)->EnableWindow(TRUE);
    GetDlgItem(IDC_NUMBER_OF_THREADS_SPIN_SPIN)->EnableWindow(TRUE);
}

void CWQDemoDlg::OnAddItemBtn()
{
    static int i = 0;
    i++;
    int nItem = m_ItemsOueueList.InsertItem(i, _T("Waiting..."));
    m_WorkQueue.InsertWorkItem(new SpecificWorkItem(&m_ItemsOueueList, nItem));
}
