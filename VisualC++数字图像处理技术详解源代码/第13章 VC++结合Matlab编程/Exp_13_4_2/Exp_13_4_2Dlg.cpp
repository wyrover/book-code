// Exp_13_4_2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Exp_13_4_2.h"
#include "Exp_13_4_2Dlg.h"
#include "tracking.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CExp_13_4_2Dlg 对话框




CExp_13_4_2Dlg::CExp_13_4_2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_4_2Dlg::IDD, pParent)
    , m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_4_2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_FILE, m_strFile);
}

BEGIN_MESSAGE_MAP(CExp_13_4_2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPENFILE, &CExp_13_4_2Dlg::OnBnClickedBtnOpenfile)
    ON_BN_CLICKED(IDC_BTN_EXECUTE, &CExp_13_4_2Dlg::OnBnClickedBtnExecute)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CExp_13_4_2Dlg::OnBnClickedBtnExit)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CExp_13_4_2Dlg 消息处理程序

BOOL CExp_13_4_2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    bool bRes = false;

    // 初始化应用程序
    bRes = mclInitializeApplication(NULL,0);
    if(!bRes)
    {
        AfxMessageBox("应用程序初始化失败。");
        return FALSE;
    }

    // 设置默认数据文件路径
    TCHAR szCurrentDir[MAX_PATH];   
    GetCurrentDirectory(MAX_PATH, szCurrentDir);   
    m_strFile.Format(_T("%s\\%s"), szCurrentDir,"data.txt");

    // 更新界面数据
    UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExp_13_4_2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExp_13_4_2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CExp_13_4_2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExp_13_4_2Dlg::OnBnClickedBtnOpenfile()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog fileDlg(TRUE,
                       "打开视频数据文件",
                        NULL,
                        OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                        "文本文件(*.txt)|*.txt||");
    if(fileDlg.DoModal() == IDOK)
    {
        m_strFile = fileDlg.GetPathName();
        UpdateData(FALSE);
    }

}

void CExp_13_4_2Dlg::OnBnClickedBtnExecute()
{
    // TODO: 在此添加控件通知处理程序代码  
    // 更新数据
    UpdateData(TRUE);

    // 从文件读取数据
    CFile file;
    file.Open(m_strFile, CFile::modeRead);
    CArchive ar(&file, CArchive::load); 
    double *pixels = new double[240*360*40];
    memset(pixels,0,sizeof(double)*240*360*40);
    ar.Read(pixels,sizeof(double)*240*360*40); 
    ar.Close();
    file.Close();

    // 将数据存入mwArray类型
    unsigned int nDim[3];
    nDim[0] = 240;
    nDim[1] = 360;
    nDim[2] = 40;
    mwArray A(3,nDim,mxDOUBLE_CLASS); 
    A.SetData(pixels,240*360*40);

    // 初始化运动跟踪库
    bool bRes = false;     
    bRes = trackingInitialize();
    if(!bRes)
    {
        AfxMessageBox("物体运动跟踪库初始化失败。");
        return;
    }

    try
    { 
        // 调用跟踪函数
        tracking(A);
    }
    catch(mwException e)
    {
        AfxMessageBox(e.what());
    }

    // 结束对物体运动跟踪库的调用
    trackingTerminate();

}

void CExp_13_4_2Dlg::OnBnClickedBtnExit()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialog::OnOK();
}

void CExp_13_4_2Dlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    bool bRes = false;

    // 结束应用程序
    bRes = mclTerminateApplication();
    if(!bRes)
    {
        AfxMessageBox("结束应用程序出错。");
    }           

}
