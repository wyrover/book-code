// Ado4ExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ado4Excel.h"
#include "Ado4ExcelDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

#define LIST_MEMBER_INDEX   0
#define LIST_MEMBER_NAME    1
#define LIST_MEMBER_SEX     2
#define LIST_MEMBER_AGE     3

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
// CAdo4ExcelDlg dialog

CAdo4ExcelDlg::CAdo4ExcelDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAdo4ExcelDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAdo4ExcelDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdo4ExcelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAdo4ExcelDlg)
    DDX_Control(pDX, IDC_LIST_Member, m_ctlListMember);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdo4ExcelDlg, CDialog)
    //{{AFX_MSG_MAP(CAdo4ExcelDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONReadExcel, OnBUTTONReadExcel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdo4ExcelDlg message handlers

BOOL CAdo4ExcelDlg::OnInitDialog()
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
    m_ctlListMember.InsertColumn(LIST_MEMBER_INDEX, "序号", LVCFMT_LEFT, 80);
    m_ctlListMember.InsertColumn(LIST_MEMBER_NAME, "姓名", LVCFMT_LEFT, 80);
    m_ctlListMember.InsertColumn(LIST_MEMBER_SEX, "性别", LVCFMT_LEFT, 80);
    m_ctlListMember.InsertColumn(LIST_MEMBER_AGE, "年龄", LVCFMT_LEFT, 80);
    m_ctlListMember.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAdo4ExcelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAdo4ExcelDlg::OnPaint()
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
HCURSOR CAdo4ExcelDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CAdo4ExcelDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CAdo4ExcelDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CDialog::OnCancel();
    }
}

CString CAdo4ExcelDlg::VarToStr(_variant_t var)
{
    CString strRet = "";

    if (var.vt != VT_NULL) {
        var.ChangeType(VT_BSTR);
        strRet = var.bstrVal;
    }

    return strRet;
}

int CAdo4ExcelDlg::VarToInt(_variant_t var)
{
    int iRet = -1;
    CString strTemp = VarToStr(var);

    if (strTemp != "") {
        iRet = atoi(strTemp);
    }

    return iRet;
}

UINT CAdo4ExcelDlg::VarToUInt(_variant_t var)
{
    int iRet = -1;
    CString strTemp = VarToStr(var);

    if (strTemp != "") {
        iRet = atou(strTemp);
    }

    return iRet;
}

UINT CAdo4ExcelDlg::atou(CString str)
{
    UINT uRet = 0;
    int num = str.GetLength();
    UCHAR ch;

    for (int i = 0; i < num; i++) {
        ch = str.GetAt(i);

        if (ch == '.')
            break;

        if (ch < '0' || ch > '9') {
            uRet = 0;
            i = 100;
            break;
        }

        uRet = uRet * 10 + ch - '0';
    }

    return uRet;
}


void CAdo4ExcelDlg::ReadExcel(CString strFilename)
{
    m_ctlListMember.DeleteAllItems();

    try {
        //创建Connection对象
        HRESULT hr = m_pConnection.CreateInstance("ADODB.Connection");

        if (SUCCEEDED(hr)) {
            m_pConnection->Mode = adModeShareExclusive;
            CString strConnection;
            strConnection.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=\"%s\";Extended Properties=Excel 8.0;", strFilename);
            //MessageBox(strConnection);
            hr = m_pConnection->Open(strConnection.GetBuffer(0), "", "", adConnectUnspecified);
            //"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DemoInject.mdb","","",adConnectUnspecified);///连接数据库
            //上面一句中连接字串中的Provider是针对ACCESS2000环境的，对于ACCESS97，需要改为：Provider=Microsoft.Jet.OLEDB.3.51;
            /*用于 Extended Properties 值的有效 Excel 版本。
            对于 Microsoft Excel 8.0 (97)、9.0 (2000) 和 10.0 (2002) 工作簿，请使用 Excel 8.0。
            对于 Microsoft Excel 5.0 和 7.0 (95) 工作簿，请使用 Excel 5.0。
            对于 Microsoft Excel 4.0 工作簿，请使用 Excel 4.0。
            对于 Microsoft Excel 3.0 工作簿，请使用 Excel 3.0。 */
        }
    } catch (_com_error e) { //捕捉异常
        CString errormessage;
        errormessage.Format("连接数据库失败，请确认数据正常后再运行！\r\n错误信息:%s", e.ErrorMessage());
        MessageBox(errormessage, "数据库出错提示", MB_OK | MB_ICONERROR); //显示错误信息
    }

    BOOL bOpenSuccess = FALSE;

    try {
        int iCount = 0;
        CString strSql, strTemp;
        strSql.Format("select * from [班级成员$]");
        m_pRecordset = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
        bOpenSuccess = TRUE;

        while (!m_pRecordset->adoEOF) {
            strTemp.Format("%d", iCount + 1);
            m_ctlListMember.InsertItem(iCount, strTemp);
            m_ctlListMember.SetItemText(iCount, LIST_MEMBER_INDEX, VarToStr(m_pRecordset->GetCollect("序号")));
            m_ctlListMember.SetItemText(iCount, LIST_MEMBER_NAME, VarToStr(m_pRecordset->GetCollect("姓名")));
            m_ctlListMember.SetItemText(iCount, LIST_MEMBER_SEX, VarToStr(m_pRecordset->GetCollect("性别")));
            m_ctlListMember.SetItemText(iCount, LIST_MEMBER_AGE, VarToStr(m_pRecordset->GetCollect("年龄")));
            m_pRecordset->MoveNext();
            iCount++;
        }

        m_pRecordset->Close();
    } catch (_com_error e) { //捕捉异常
        CString errormessage;
        errormessage.Format("读取报警失败，请确认数据库正常后再运行！\r\n错误信息:%s", e.ErrorMessage());
        //if (bOpenSuccess)
        MessageBox(errormessage, "数据库出错提示", MB_OK | MB_ICONERROR); //显示错误信息
        //else
        //  MessageBox(errormessage,"数据库出错提示",MB_OK|MB_ICONERROR);//显示错误信息
    }

    m_pConnection->Close();
    return;
}

void CAdo4ExcelDlg::OnBUTTONReadExcel()
{
    // TODO: Add your control notification handler code here
    char buff[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, (char *)buff); //在系统启动的时候读当前路径
    CString strFilename;
    strFilename.Format("%s\\Ado4Excel.xls", buff); //记为绝对路径
    ReadExcel(strFilename);
}
