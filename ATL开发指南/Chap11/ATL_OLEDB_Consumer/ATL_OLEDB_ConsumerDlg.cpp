// ATL_OLEDB_ConsumerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATL_OLEDB_Consumer.h"
#include "ATL_OLEDB_ConsumerDlg.h"

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
// CATL_OLEDB_ConsumerDlg dialog

CATL_OLEDB_ConsumerDlg::CATL_OLEDB_ConsumerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CATL_OLEDB_ConsumerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CATL_OLEDB_ConsumerDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATL_OLEDB_ConsumerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CATL_OLEDB_ConsumerDlg)
    DDX_Control(pDX, IDC_TEMPERATURESLIST, m_temperaturesList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATL_OLEDB_ConsumerDlg, CDialog)
    //{{AFX_MSG_MAP(CATL_OLEDB_ConsumerDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATL_OLEDB_ConsumerDlg message handlers

BOOL CATL_OLEDB_ConsumerDlg::OnInitDialog()
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
    // Create the columns to display the temperatures in the list control
    m_temperaturesList.InsertColumn(0, "City", LVCFMT_LEFT, 150);
    m_temperaturesList.InsertColumn(1, "High", LVCFMT_RIGHT, 70);
    m_temperaturesList.InsertColumn(2, "Low", LVCFMT_RIGHT, 70);
    m_temperaturesList.InsertColumn(3, "Precipitation", LVCFMT_RIGHT, 70);
    // Add the data from the data source
    HRESULT hr = m_temperatures.Open();

    if (SUCCEEDED(hr)) {
        CString tmpString;
        int i = 0;

        while (m_temperatures.MoveNext() == S_OK) {
            // City
            int ndx = m_temperaturesList.InsertItem(i, m_temperatures.m_City);
            // High Temperature
            tmpString.Format("%d", m_temperatures.m_High);
            m_temperaturesList.SetItemText(ndx, 1, tmpString);
            // Low Temperature
            tmpString.Format("%d", m_temperatures.m_Low);
            m_temperaturesList.SetItemText(ndx, 2, tmpString);
            // Precipitation
            tmpString.Format("%0.2lf", m_temperatures.m_Precipitation);
            m_temperaturesList.SetItemText(ndx, 3, tmpString);
            i++;
        }
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CATL_OLEDB_ConsumerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CATL_OLEDB_ConsumerDlg::OnPaint()
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
HCURSOR CATL_OLEDB_ConsumerDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}
