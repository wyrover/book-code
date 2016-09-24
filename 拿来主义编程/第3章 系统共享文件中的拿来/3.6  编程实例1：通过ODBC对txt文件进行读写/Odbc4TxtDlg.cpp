// Odbc4TxtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Odbc4Txt.h"
#include "Odbc4TxtDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//使用ODBC API之前要用到的头文件和LIB库
//#include "sql.h"              // This is the the main include for ODBC Core functions.
#include "sqlext.h"         // This is the include for applications using the Microsoft SQL Extensions
//#include "sqltypes.h"    // This file defines the types used in ODBC
//#include "sqlucode.h"   // This is the the unicode include for ODBC Core functions
//#include "odbcss.h"      // This is the application include file for the SQL Server driver specific defines.
#pragma comment(lib, "odbc32.lib")

BOOL ReturnIsTRUE(SQLRETURN retcode);

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
// COdbc4TxtDlg dialog

COdbc4TxtDlg::COdbc4TxtDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COdbc4TxtDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(COdbc4TxtDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COdbc4TxtDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COdbc4TxtDlg)
    DDX_Control(pDX, IDC_LIST_Member, m_ctlListMember);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COdbc4TxtDlg, CDialog)
    //{{AFX_MSG_MAP(COdbc4TxtDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Read, OnBUTTONRead)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbc4TxtDlg message handlers

BOOL COdbc4TxtDlg::OnInitDialog()
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

void COdbc4TxtDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COdbc4TxtDlg::OnPaint()
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
HCURSOR COdbc4TxtDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void COdbc4TxtDlg::OnBUTTONRead()
{
    //通过ODBC连接数据库。
    CString strTemp;
    SQLHENV henv;  //环境句柄
    SQLHDBC hdbc;  //连接句柄
    SQLHSTMT hstmt;  //语句句柄
    SQLRETURN retcode; //返回值
    //分配环境句柄
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

    if (!ReturnIsTRUE(retcode)) {
        MessageBox("分配环境句柄空间错误。请重新分配。", "出错提示", MB_OK);
        return;
    }

    //设置ODBC环境属性
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    if (!ReturnIsTRUE(retcode)) {
        MessageBox("设置ODBC环境属性错误。请重新设置。", "出错提示", MB_OK);
        return;
    }

    //分配连接句柄
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

    if (!ReturnIsTRUE(retcode)) {
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        MessageBox("生成环境句柄错误。请重新生成。", "出错提示", MB_OK);
        return;
    }

    //连接到数据源
    retcode = SQLConnect(hdbc, (SQLCHAR*)"TxtDSN", SQL_NTS, (SQLCHAR*)"", SQL_NTS, (SQLCHAR*)"", SQL_NTS);

    if (!ReturnIsTRUE(retcode)) {
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        MessageBox("连接到数据源错误。请重新生成。", "出错提示", MB_OK);
        return;
    }

    //分配语句句柄
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if (ReturnIsTRUE(retcode)) {
        //处理数据
        SQLCHAR      szName[MAX_PATH], szSex[MAX_PATH];
        SQLINTEGER   cbName, cbSex;
        SQLINTEGER   sOrder, cbOrder, sAge, cbAge;
        SQLCHAR      szSQL[50] = "SELECT * FROM DatabaseTxt.txt";
        retcode = SQLExecDirect(hstmt, szSQL, SQL_NTS);

        if (ReturnIsTRUE(retcode)) {
            SQLBindCol(hstmt, 1, SQL_C_ULONG, &sOrder, 0, &cbOrder);
            SQLBindCol(hstmt, 2, SQL_C_CHAR, szName, MAX_PATH, &cbName);
            SQLBindCol(hstmt, 3, SQL_C_CHAR, szSex, MAX_PATH, &cbSex);
            SQLBindCol(hstmt, 4, SQL_C_ULONG, &sAge, 0, &cbAge);
            int iCount = 0;

            while (TRUE) {
                retcode = SQLFetch(hstmt);

                if (!ReturnIsTRUE(retcode))
                    break;

                if (iCount > 0) {
                    strTemp.Format("%d", iCount);
                    m_ctlListMember.InsertItem(iCount - 1, strTemp);
                    strTemp.Format("%d", sOrder);
                    m_ctlListMember.SetItemText(iCount - 1, LIST_MEMBER_INDEX, strTemp);
                    strTemp.Format("%s", szName);
                    m_ctlListMember.SetItemText(iCount - 1, LIST_MEMBER_NAME, strTemp);
                    strTemp.Format("%s", szSex);
                    m_ctlListMember.SetItemText(iCount - 1, LIST_MEMBER_SEX, strTemp);
                    strTemp.Format("%d", sAge);
                    m_ctlListMember.SetItemText(iCount - 1, LIST_MEMBER_AGE, strTemp);
                }

                iCount++;
            }
        } else {
            strTemp.Format("%d", retcode);
            MessageBox(strTemp, "出错提示", MB_OK);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    } else {
        MessageBox("处理数据错误。请重新生成。", "出错提示", MB_OK);
    }

    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
    MessageBeep(MB_ICONHAND);
}

BOOL ReturnIsTRUE(SQLRETURN retcode)
{
    BOOL bRet = FALSE;

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        bRet = TRUE;

    return bRet;
}

void COdbc4TxtDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void COdbc4TxtDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("确实要退出吗?", "退出系统", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CDialog::OnCancel();
    }
}
