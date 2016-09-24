// VcWordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VcWord.h"
#include "VcWordDlg.h"

#include <atlbase.h>
#include "msword.h"

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
// CVcWordDlg dialog

CVcWordDlg::CVcWordDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVcWordDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVcWordDlg)
    m_strFilename = _T("");
    m_strContent = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVcWordDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVcWordDlg)
    DDX_Text(pDX, IDC_EDIT_Filename, m_strFilename);
    DDX_Text(pDX, IDC_EDIT_Content, m_strContent);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVcWordDlg, CDialog)
    //{{AFX_MSG_MAP(CVcWordDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Browser, OnBUTTONBrowser)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVcWordDlg message handlers

BOOL CVcWordDlg::OnInitDialog()
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
    if (!AfxOleInit()) {
        AfxMessageBox("COM组件初始化失败。");
        return FALSE;
    }

    //char buff[MAX_PATH];
    //GetCurrentDirectory(MAX_PATH,buff);
    //m_strCurrentDirectory.Format("%s",(char *)buff);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVcWordDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVcWordDlg::OnPaint()
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
HCURSOR CVcWordDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void CVcWordDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CVcWordDlg::OnCancel()
{
    //退出程序。
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        CDialog::OnCancel();
    }
}

void CVcWordDlg::OnBUTTONBrowser()
{
    //打开Word文件。
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    "Word Files (*.doc,*.docx)|*.doc;*.docx|All Files (*.*)|*.*||");

    if (dlg.DoModal() == IDCANCEL)
        return;

    UpdateData(TRUE);
    m_strFilename = dlg.GetPathName();
    //打开Word文件，并读取其中的内容。
    //WORD应用程序。
    _Application app;
    app.CreateDispatch("word.Application");
    Documents docs;
    docs.AttachDispatch(app.GetDocuments());
    //CString strFileName;
    //strFileName.Format(,m_strCurrentDirectory);
    CComVariant cFileName(m_strFilename.AllocSysString()), visible(TRUE), doctype(0), doctemplate(TRUE);
    _Document doc;
    doc.AttachDispatch(docs.Add(&cFileName, &visible, &doctype, &doctemplate));
    Paragraphs pgraphs;
    pgraphs.AttachDispatch(doc.GetParagraphs());
    LONG pgCount = pgraphs.GetCount();
    m_strContent = "";

    for (LONG i = 1; i <= pgCount; i++) {
        Paragraph pgraph;
        pgraph.AttachDispatch(pgraphs.Item(i));
        Range range;
        range.AttachDispatch(pgraph.GetRange());
        m_strContent += range.GetText() + "\r\n";
    }

    UpdateData(FALSE);
    CComVariant save(0), format(0), route(0);
    doc.Close(&save, &format, &route);
    doc.ReleaseDispatch();
    docs.ReleaseDispatch();
    app.Quit(&save, &format, &route);
    app.ReleaseDispatch();
    MessageBox("读取完成。");
}
