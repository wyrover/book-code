// VcPPTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VcPPT.h"
#include "VcPPTDlg.h"
#include "DlgProxy.h"

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
// CVcPPTDlg dialog

IMPLEMENT_DYNAMIC(CVcPPTDlg, CDialog);

CVcPPTDlg::CVcPPTDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVcPPTDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVcPPTDlg)
    m_strComment = _T("");
    m_strFilename = _T("");
    m_strContent = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pAutoProxy = NULL;
}

CVcPPTDlg::~CVcPPTDlg()
{
    // If there is an automation proxy for this dialog, set
    //  its back pointer to this dialog to NULL, so it knows
    //  the dialog has been deleted.
    if (m_pAutoProxy != NULL)
        m_pAutoProxy->m_pDialog = NULL;
}

void CVcPPTDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVcPPTDlg)
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    DDX_Text(pDX, IDC_EDIT_Filename, m_strFilename);
    DDX_Text(pDX, IDC_EDIT_Txt, m_strContent);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVcPPTDlg, CDialog)
    //{{AFX_MSG_MAP(CVcPPTDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Start, OnBUTTONStart)
    ON_BN_CLICKED(IDC_BUTTON_Run, OnBUTTONRun)
    ON_BN_CLICKED(IDC_BUTTON_Close, OnBUTTONClose)
    ON_BN_CLICKED(IDC_BUTTON_FirstSlide, OnBUTTONFirstSlide)
    ON_BN_CLICKED(IDC_BUTTON_PreviousSlide, OnBUTTONPreviousSlide)
    ON_BN_CLICKED(IDC_BUTTON_NextSlide, OnBUTTONNextSlide)
    ON_BN_CLICKED(IDC_BUTTON_LastSlide, OnBUTTONLastSlide)
    ON_BN_CLICKED(IDC_BUTTON_Play, OnBUTTONPlay)
    ON_BN_CLICKED(IDC_BUTTON_Minimize, OnBUTTONMinimize)
    ON_BN_CLICKED(IDC_BUTTON_Browser, OnBUTTONBrowser)
    ON_BN_CLICKED(IDC_BUTTONGetText, OnBUTTONGetText)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVcPPTDlg message handlers

BOOL CVcPPTDlg::OnInitDialog()
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

void CVcPPTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVcPPTDlg::OnPaint()
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
HCURSOR CVcPPTDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CVcPPTDlg::OnOK()
{
    //CDialog::OnOK();
}

void CVcPPTDlg::OnCancel()
{
    //退出程序。
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        CDialog::OnCancel();
    }
}

void CVcPPTDlg::OnBUTTONStart()
{
    //打开程序。
    UpdateData(TRUE);

    if (!m_pptApp.CreateDispatch("Powerpoint.Application")) {
        m_strComment = "PowerPoint启动失败，请确认PowerPoint程序已安装。";
    } else {
        m_strComment = "PowerPoint启动成功，可以打开PPT文件。";
        m_pptApp.SetVisible((long)TRUE);
        //m_pptApp.SetWindowState((long)2);   //最小化，ppWindowMinimized=2
        this->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
        this->GetDlgItem(IDC_BUTTON_Run)->EnableWindow(TRUE);
        this->GetDlgItem(IDC_BUTTON_Close)->EnableWindow(FALSE);
    }

    UpdateData(TRUE);
}

void CVcPPTDlg::OnBUTTONRun()
{
    //运行程序
    //static char BASED_CODE szFilter[] = "PowerPoint 文件 (*.ppt)|*.ppt||";
    //CFileDialog FileDlg(TRUE,"PPT",NULL,OFN_FILEMUSTEXIST|OFN_NONETWORKBUTTON
    //  |OFN_PATHMUSTEXIST,szFilter);
    //if (FileDlg.DoModal()!=IDOK)
    //  return;
    // To get the selected file's path and name
    //CString strFileName;
    //strFileName = FileDlg.GetPathName();
    UpdateData(TRUE);

    if (m_strFilename == "") {
        MessageBox("先选择要打开的文件。");
        return;
    }

    m_strComment.Format("PPT文件%s已运行。", m_strFilename);
    this->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
    this->GetDlgItem(IDC_BUTTON_Run)->EnableWindow(FALSE);
    this->GetDlgItem(IDC_BUTTON_Close)->EnableWindow(TRUE);
    UpdateData(FALSE);
    //获得
    m_pptPresentations = m_pptApp.GetPresentations();
    m_pptPresentation = m_pptPresentations.Open(m_strFilename, 0, 0, 1);
    //File name,Read-only,Untitled,WithWindow
    m_pptPresentations = m_pptApp.GetActivePresentation();
    m_pptSlides = m_pptPresentation.GetSlides();
    // Show the first slide of the presentation
    m_pptSlide = m_pptSlides.Item(COleVariant((long)1));
    m_pptSlideShowSettings = m_pptPresentation.GetSlideShowSettings();
    m_pptSlideShowSettings.Run();//Run the show
}

void CVcPPTDlg::OnBUTTONClose()
{
    //关闭程序
    if (m_pAutoProxy != NULL) {
        m_pptApp.Quit();
    }

    this->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(TRUE);
    this->GetDlgItem(IDC_BUTTON_Run)->EnableWindow(FALSE);
    this->GetDlgItem(IDC_BUTTON_Close)->EnableWindow(FALSE);
}

void CVcPPTDlg::OnBUTTONFirstSlide()
{
    //跳转到第一页。
    m_pptPresentation = m_pptApp.GetActivePresentation();
    m_pptSlideShowWindow = m_pptPresentation.GetSlideShowWindow();
    m_pptSlideShowView = m_pptSlideShowWindow.GetView();
    m_pptSlideShowView.First();
}

void CVcPPTDlg::OnBUTTONPreviousSlide()
{
    //跳转到上一页。
    m_pptPresentation = m_pptApp.GetActivePresentation();
    m_pptSlideShowWindow = m_pptPresentation.GetSlideShowWindow();
    m_pptSlideShowView = m_pptSlideShowWindow.GetView();
    m_pptSlideShowView.Previous();
}

void CVcPPTDlg::OnBUTTONNextSlide()
{
    //跳转到下一页。
    m_pptPresentation = m_pptApp.GetActivePresentation();
    m_pptSlideShowWindow = m_pptPresentation.GetSlideShowWindow();
    m_pptSlideShowView = m_pptSlideShowWindow.GetView();
    m_pptSlideShowView.Next();
}

void CVcPPTDlg::OnBUTTONLastSlide()
{
    //跳转到尾页。
    m_pptPresentation = m_pptApp.GetActivePresentation();
    m_pptSlideShowWindow = m_pptPresentation.GetSlideShowWindow();
    m_pptSlideShowView = m_pptSlideShowWindow.GetView();
    m_pptSlideShowView.Last();
}

void CVcPPTDlg::OnBUTTONPlay()
{
    // TODO: Add your control notification handler code here
    m_pptPresentation = m_pptApp.GetActivePresentation();
    m_pptSlideShowWindow = m_pptPresentation.GetSlideShowWindow();
    m_pptSlideShowView = m_pptSlideShowWindow.GetView();
    long Curpos = m_pptSlideShowView.GetCurrentShowPosition(); //当前幻灯片序号
    m_pptSlides = m_pptPresentation.GetSlides();
    long endpos = m_pptSlides.GetCount(); //获得幻灯片总数
    CString strTemp;
    strTemp.Format("%d,%d", Curpos, endpos);
    MessageBox(strTemp);
}

void CVcPPTDlg::OnBUTTONMinimize()
{
    // TODO: Add your control notification handler code here
    //Make the application visible and minimized.
    m_pptApp.SetVisible((long)TRUE);
    m_pptApp.SetWindowState((long) 2);   //ppWindowMinimized=2
}

void CVcPPTDlg::ShowComment(CString str, int i, int j, int k)
{
    UpdateData(TRUE);

    if (str != "")
        m_strComment = str;
    else
        m_strComment.Format("%d,%d,%d", i, j, k);

    UpdateData(FALSE);
}


void CVcPPTDlg::OnBUTTONBrowser()
{
    // TODO: Add your control notification handler code here
    //打开Excel文件。
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    "Powerpoint Files (*.ppt,*.pptx)|*.ppt;*.pptx|All Files (*.*)|*.*||");

    if (dlg.DoModal() == IDCANCEL)
        return;

    UpdateData(TRUE);
    m_strFilename = dlg.GetPathName();
    UpdateData(FALSE);
}

void CVcPPTDlg::OnBUTTONGetText()
{
    // TODO: Add your control notification handler code here
    m_pptPresentation = m_pptApp.GetActivePresentation();
    m_pptSlideShowWindow = m_pptPresentation.GetSlideShowWindow();
    m_pptSlideShowView = m_pptSlideShowWindow.GetView();
    long Curpos = m_pptSlideShowView.GetCurrentShowPosition(); //当前幻灯片序号
    m_pptSlides = m_pptPresentation.GetSlides();
    long endpos = m_pptSlides.GetCount(); //获得幻灯片总数
    //要获得标题什么的要用到_slide的GetShape函数，返回shapes类型，
    //shapes里面自己选择，比如里面的GetTitle可以获得标题的shape，
    //再通过Shape的GetTextFrame函数， TextFrame的GetTextRange函数，
    //最后 TextRange的GetText就获得标题的文字信息了
    m_pptSlide = m_pptSlides.Item(COleVariant((long)Curpos));
    Shapes pptShapes = m_pptSlide.GetShapes();
    Shape pptShape;//=pptShapes.GetTitle();
    long lCount = pptShapes.GetCount();
    UpdateData(TRUE);
    m_strContent = "";

    for (long i = 1; i <= lCount; i++) {
        TRY {
            pptShape = pptShapes.Item(COleVariant((long)i));
            LPDISPATCH lpFrame = pptShape.GetTextFrame();

            TextFrame pptTextFrame;
            pptTextFrame.AttachDispatch(lpFrame);//与应用建立关联

            TextRange pptTextRange = pptTextFrame.GetTextRange();
            CString strText = pptTextRange.GetText();
            m_strContent += strText + "\r\n";
        }
        CATCH_ALL(e) {
            //未做任何处理。
        }
        END_CATCH_ALL
    }

    m_strComment = "内容读取成功。";
    UpdateData(FALSE);
}

