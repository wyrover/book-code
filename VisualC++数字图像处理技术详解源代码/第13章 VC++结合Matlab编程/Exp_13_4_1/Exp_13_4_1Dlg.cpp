// Exp_13_4_1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Exp_13_4_1.h"
#include "Exp_13_4_1Dlg.h"
#include "engine.h"

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


// CExp_13_4_1Dlg 对话框




CExp_13_4_1Dlg::CExp_13_4_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_4_1Dlg::IDD, pParent)
    , m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_4_1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_FILE, m_strFile);
}

BEGIN_MESSAGE_MAP(CExp_13_4_1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_FILE, &CExp_13_4_1Dlg::OnBnClickedBtnFile)
    ON_BN_CLICKED(IDC_BTN_EXECUTE, &CExp_13_4_1Dlg::OnBnClickedBtnExecute)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CExp_13_4_1Dlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CExp_13_4_1Dlg 消息处理程序

BOOL CExp_13_4_1Dlg::OnInitDialog()
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
    // 设置默认数据文件路径
    TCHAR szCurrentDir[MAX_PATH];   
    GetCurrentDirectory(MAX_PATH, szCurrentDir);   
    m_strFile.Format(_T("%s\\%s"), szCurrentDir,"data.txt");
    m_strPath.Format(_T("%s"), szCurrentDir);

    // 更新界面数据
    UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExp_13_4_1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExp_13_4_1Dlg::OnPaint()
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
HCURSOR CExp_13_4_1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExp_13_4_1Dlg::OnBnClickedBtnFile()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog fileDlg(FALSE,
                       "保存视频数据文件",
                       "data.txt",
                       OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                       "文本文件(*.txt)|*.txt||");
    if(fileDlg.DoModal() == IDOK)
    {
        m_strFile = fileDlg.GetPathName();
        UpdateData(FALSE);
    }
}

void CExp_13_4_1Dlg::OnBnClickedBtnExecute()
{
    // TODO: 在此添加控件通知处理程序代码
    // 状态变量
    int nStatus = 0;
	
	// 定义MATLAB引擎指针
	Engine *ep;
	
	// 打开MATLAB引擎
	ep = engOpen(NULL);
	if(ep == NULL)
	{
        AfxMessageBox("无法打开MATLAB引擎。");
		return;
	}

    // 是否显示MATLAB窗口：true-显示；false-隐藏。
    nStatus = engSetVisible(ep, false);
    if(nStatus != 0)
    {
        AfxMessageBox("设置MATLAB显示属性失败。");
        return;
    }

    // 切换到M文件和视频所在的目录
    CString strCmd;
    strCmd.Format("cd %s",m_strPath);
    nStatus = engEvalString(ep,strCmd);
     if(nStatus != 0)
    {
        AfxMessageBox("执行MATLAB切换目录命令失败。");
        return;
    }

    // 执行引擎命令获取视频数据
    nStatus = engEvalString(ep,"pixels=getVideo;");
    if(nStatus != 0)
    {
        AfxMessageBox("执行MATLAB获取视频数据命令失败。");
        return;
    }

    // 从MATLAB引擎空间中读取执行结果
    unsigned int nDim[3];
    nDim[0] = 240;
    nDim[1] = 360;
    nDim[2] = 40;
    mxArray *A = mxCreateNumericArray(3,nDim,mxLOGICAL_CLASS,mxREAL); 
    A = engGetVariable(ep,"pixels");
    double *pixels = new double[240*360*40];
    memset(pixels,0,sizeof(double)*240*360*40);
    memcpy(pixels,mxGetPr(A),sizeof(double)*240*360*40);

    // 将数据存入文件
    CFile file;
    file.Open(m_strFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 
    CArchive ar(&file, CArchive::store); 
    ar.Write(pixels,sizeof(double)*240*360*40); 
    ar.Close();
    file.Close();

    // 释放矩阵变量空间
    mxDestroyArray(A);  
    delete [] pixels;

    // 关闭MATLAB引擎
    engClose(ep);

    // 数据获取完成提示
    AfxMessageBox("视频数据提取成功。");

}

void CExp_13_4_1Dlg::OnBnClickedBtnExit()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialog::OnOK();
}
