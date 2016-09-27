// VCActiveXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCActiveX.h"
#include "VCActiveXDlg.h"

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
// CVCActiveXDlg dialog

CVCActiveXDlg::CVCActiveXDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVCActiveXDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVCActiveXDlg)
    m_day = 0;
    m_month = 0;
    m_year = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCActiveXDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVCActiveXDlg)
    DDX_Control(pDX, IDC_CALENDARCTRL, m_calendar);
    DDX_Text(pDX, IDC_DAY, m_day);
    DDX_Text(pDX, IDC_MONTH, m_month);
    DDX_Text(pDX, IDC_YEAR, m_year);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVCActiveXDlg, CDialog)
    //{{AFX_MSG_MAP(CVCActiveXDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SELECTDATE, OnSelectdate)
    ON_BN_CLICKED(IDC_NEXTWEEK, OnNextweek)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCActiveXDlg message handlers

BOOL CVCActiveXDlg::OnInitDialog()
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
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCActiveXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVCActiveXDlg::OnPaint()
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
HCURSOR CVCActiveXDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CVCActiveXDlg::OnSelectdate()
{
    UpdateData(TRUE);
    m_calendar.SetDay(m_day);
    m_calendar.SetMonth(m_month);
    m_calendar.SetYear(m_year);
}

void CVCActiveXDlg::OnNextweek()
{
    m_calendar.NextWeek();
}

BEGIN_EVENTSINK_MAP(CVCActiveXDlg, CDialog)
//{{AFX_EVENTSINK_MAP(CVCActiveXDlg)
ON_EVENT(CVCActiveXDlg, IDC_CALENDARCTRL, 1 /* AfterUpdate */, OnAfterUpdateCalendarctrl, VTS_NONE)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void CVCActiveXDlg::OnAfterUpdateCalendarctrl()
{
    COleDateTime date;
    date = m_calendar.GetValue();
    m_day = date.GetDay();
    m_month = date.GetMonth();
    m_year = date.GetYear();
    UpdateData(false);
}

