// ReadPDFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadPDF.h"
#include "ReadPDFDlg.h"

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
// CReadPDFDlg dialog

CReadPDFDlg::CReadPDFDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CReadPDFDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CReadPDFDlg)
    m_strFilename = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadPDFDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReadPDFDlg)
    DDX_Text(pDX, IDC_EDIT_Filename, m_strFilename);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReadPDFDlg, CDialog)
    //{{AFX_MSG_MAP(CReadPDFDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Browser, OnBUTTONBrowser)
    ON_BN_CLICKED(IDC_BUTTON_FirstPage, OnBUTTONFirstPage)
    ON_BN_CLICKED(IDC_BUTTON_PreviousPage, OnBUTTONPreviousPage)
    ON_BN_CLICKED(IDC_BUTTON_NextPage, OnBUTTONNextPage)
    ON_BN_CLICKED(IDC_BUTTON_LastPage, OnBUTTONLastPage)
    ON_BN_CLICKED(IDC_BUTTON_ZoomOut, OnBUTTONZoomOut)
    ON_BN_CLICKED(IDC_BUTTON_ZoomIn, OnBUTTONZoomIn)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadPDFDlg message handlers

BOOL CReadPDFDlg::OnInitDialog()
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
    m_fZoom = 100.0f;
    CRect rect;
    CWnd *pWnd = this->GetDlgItem(IDC_STATIC_PDFVIEW);
    pWnd->GetWindowRect(rect);
    ScreenToClient(rect);
    m_ctlPdf.Create(_T("STATIC"), "Hi", WS_CHILD | WS_VISIBLE, rect, this, 1234);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReadPDFDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReadPDFDlg::OnPaint()
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
HCURSOR CReadPDFDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CReadPDFDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CReadPDFDlg::OnCancel()
{
    //退出程序。
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        CDialog::OnCancel();
    }
}

void CReadPDFDlg::OnBUTTONBrowser()
{
    //打开PDF文件。
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "PDF Files (*.pdf)|*.pdf|All Files (*.*)|*.*||");

    if (dlg.DoModal() == IDOK) {
        UpdateData(TRUE);
        m_strFilename = dlg.GetPathName();
        UpdateData(FALSE);
        m_ctlPdf.LoadFile(m_strFilename);
        m_ctlPdf.setZoom(m_fZoom);
        m_ctlPdf.UpdateWindow();
    }
}

void CReadPDFDlg::OnBUTTONFirstPage()
{
    //跳转到第一页。
    m_ctlPdf.gotoFirstPage();
}

void CReadPDFDlg::OnBUTTONPreviousPage()
{
    //转到上一页。
    m_ctlPdf.gotoPreviousPage();
}

void CReadPDFDlg::OnBUTTONNextPage()
{
    //转到下一页。
    m_ctlPdf.gotoNextPage();
}

void CReadPDFDlg::OnBUTTONLastPage()
{
    //跳转到最后一页。
    m_ctlPdf.gotoLastPage();
}

void CReadPDFDlg::OnBUTTONZoomOut()
{
    //缩小。
    if (m_fZoom >= 10.0f) {
        m_fZoom /= 2.0f;
        m_ctlPdf.setZoom(m_fZoom);
    }
}

void CReadPDFDlg::OnBUTTONZoomIn()
{
    //放大。
    if (m_fZoom <= 500.0f) {
        m_fZoom *= 2.0f;
        m_ctlPdf.setZoom(m_fZoom);
    }
}
