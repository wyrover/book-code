// VcExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VcExcel.h"
#include "VcExcelDlg.h"

#include "excel.h"

#define LIST_EXCEL_INDEX    0
#define LIST_EXCEL_NAME     1
#define LIST_EXCEL_SEX      2
#define LIST_EXCEL_AGE      3

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
// CVcExcelDlg dialog

CVcExcelDlg::CVcExcelDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVcExcelDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVcExcelDlg)
    m_strFilename = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVcExcelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVcExcelDlg)
    DDX_Control(pDX, IDC_LIST_Excel, m_ctlListExcel);
    DDX_Text(pDX, IDC_EDIT_Filename, m_strFilename);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVcExcelDlg, CDialog)
    //{{AFX_MSG_MAP(CVcExcelDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONBrowser, OnBUTTONBrowser)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVcExcelDlg message handlers

BOOL CVcExcelDlg::OnInitDialog()
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
    m_ctlListExcel.InsertColumn(LIST_EXCEL_INDEX, "序号", LVCFMT_LEFT, 80);
    m_ctlListExcel.InsertColumn(LIST_EXCEL_NAME, "姓名", LVCFMT_LEFT, 80);
    m_ctlListExcel.InsertColumn(LIST_EXCEL_SEX, "性别", LVCFMT_LEFT, 80);
    m_ctlListExcel.InsertColumn(LIST_EXCEL_AGE, "年龄", LVCFMT_LEFT, 80);
    m_ctlListExcel.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    if (!AfxOleInit()) {
        AfxMessageBox("COM组件初始化失败。");
        return FALSE;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVcExcelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVcExcelDlg::OnPaint()
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
HCURSOR CVcExcelDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CVcExcelDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CVcExcelDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CDialog::OnCancel();
    }
}

void CVcExcelDlg::OnBUTTONBrowser()
{
    // TODO: Add your control notification handler code here
    //打开Excel文件。
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    "Excel Files (*.xls,*.xlsx)|*.xls;*.xlsx|All Files (*.*)|*.*||");

    if (dlg.DoModal() == IDCANCEL)
        return;

    UpdateData(TRUE);
    //0 在这些Excel文件中，可以打开具体的Excel文件了。
    m_strFilename = dlg.GetPathName();
    //1 创建一个与“Excel服务器”的连接。
    _Application excelApp;//Excel的组件服务器

    if (!excelApp.CreateDispatch("Excel.Application")) {
        MessageBox("无法启动Excel服务器，可能是服务器忙，或系统没有安装Excel软件。");
        return;
    }

    //使Excel不可见，否则系统会打开Excel文件本身，并允许其它用户控制Excel。
    excelApp.SetVisible(FALSE);
    excelApp.SetUserControl(FALSE);
    //2 找到其Excel文件，打开一个文件。
    //则首先看到的是Excel的集合，
    //当打开多个Excel文件时，系统会自动打开多个Excel来打开这些文件。
    Workbooks excelFiles;
    LPDISPATCH lpBooks = excelApp.GetWorkbooks();
    excelFiles.AttachDispatch(lpBooks);//与应用建立关联
    //3 打开所选的文件。
    COleVariant avar((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    LPDISPATCH lpFiles = excelFiles.Open(m_strFilename,
                                         avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar);
    //4 在每一个Excel文件中，看到的是工作单的集合。
    _Workbook excelFile;
    excelFile.AttachDispatch(lpFiles);
    //5 当前工作单集合中。
    Worksheets excelSheets;
    excelSheets.AttachDispatch(excelFile.GetWorksheets());
    //6 关联“当前工作单”。
    _Worksheet excelSheet;
    LPDISPATCH lpSheet = excelFile.GetActiveSheet();
    excelSheet.AttachDispatch(lpSheet);
    //7 默认的工作单，工作单中，默认的表格数中有部分是使用的表格
    Range usedRange;
    usedRange.AttachDispatch(excelSheet.GetUsedRange());
    //8 具体的表格。
    Range range;
    range.AttachDispatch(usedRange.GetRows());
    long iNumRow = range.GetCount(); //已经使用的行数
    range.AttachDispatch(usedRange.GetColumns());
    long iNumCol = range.GetCount(); //已经使用的列数
    //9 开始读取其中的内容。
    CString strItem, strLine, strAll;
    m_ctlListExcel.DeleteAllItems();

    for (long i = 2; i <= iNumRow; i++) {
        strLine = "";
        strItem.Format("%d", i);
        m_ctlListExcel.InsertItem(i - 2, strItem);

        for (int j = 1; j <= iNumCol; j++) {
            range.AttachDispatch(excelSheet.GetCells());
            VARIANT va = range.GetItem(COleVariant(long(i)), COleVariant(long(j)));
            range.AttachDispatch(va.pdispVal);
            COleVariant vValue = range.GetValue2();

            switch (vValue.vt) {
            case VT_R8:
                strItem.Format("%d", (int)vValue.dblVal);
                break;

            case VT_BSTR:
                strItem = vValue.bstrVal;
                break;

            case VT_I4:
                strItem.Format("%ld", (int)vValue.lVal);
                break;

            default:
                strItem.Format("非常规格式（%d）", vValue.vt);
                break;
            }

            strLine += strItem;
            m_ctlListExcel.SetItemText(i - 2, j - 1, strItem);
        }

        strAll += strLine + "\r\n";
    }

    //MessageBox(strAll);
    //10 关闭表格
    range.ReleaseDispatch();
    usedRange.ReleaseDispatch();
    //11 关闭表单。
    excelSheet.ReleaseDispatch();
    excelSheets.ReleaseDispatch();
    //12 关闭工作的文件
    excelFile.ReleaseDispatch();
    //excelFile.Save();
    excelFile.Close(avar, COleVariant(m_strFilename), avar);
    excelFiles.ReleaseDispatch();
    excelFiles.Close();
    //13 关闭整个Excel服务器。
    excelApp.ReleaseDispatch();
    excelApp.Quit();
    HWND hwnd =::FindWindow("XLMAIN", NULL);
    ::SendMessage(hwnd, WM_CLOSE, 0, 0);
}
