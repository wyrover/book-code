// Exp_13_3_7Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Exp_13_3_7.h"
#include "Exp_13_3_7Dlg.h"
#include "Dib.h"
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


// CExp_13_3_7Dlg 对话框




CExp_13_3_7Dlg::CExp_13_3_7Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_3_7Dlg::IDD, pParent)
    , m_strAppPath(_T(""))
    , m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_3_7Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STC_PIC, m_stcPic);
}

BEGIN_MESSAGE_MAP(CExp_13_3_7Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPEN, &CExp_13_3_7Dlg::OnBnClickedBtnOpen)
    ON_BN_CLICKED(IDC_BTN_CVT, &CExp_13_3_7Dlg::OnBnClickedBtnCvt)
    ON_BN_CLICKED(IDC_BTN_SAVE, &CExp_13_3_7Dlg::OnBnClickedBtnSave)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CExp_13_3_7Dlg::OnBnClickedBtnExit)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CExp_13_3_7Dlg 消息处理程序

BOOL CExp_13_3_7Dlg::OnInitDialog()
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
    // 数据初始化
    m_strFile = "";

    // 设置默认数据文件路径
    TCHAR szCurrentDir[MAX_PATH];   
    GetCurrentDirectory(MAX_PATH, szCurrentDir); 
    m_strAppPath.Format("%s",szCurrentDir);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExp_13_3_7Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExp_13_3_7Dlg::OnPaint()
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
        showPic();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CExp_13_3_7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExp_13_3_7Dlg::OnBnClickedBtnOpen()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog fileDlg(TRUE,
                       "打开位图",
                        NULL,
                        OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                        "位图文件(*.bmp)|*.bmp||");
    if(fileDlg.DoModal() == IDOK)
    {
        // 获取文件名称
        m_strFile = fileDlg.GetPathName();
        // 加载位图
        m_dib.LoadFromFile(m_strFile);
        // 显示位图
        showPic();
    }
}

void CExp_13_3_7Dlg::OnBnClickedBtnCvt()
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
        AfxMessageBox("MATLAB引擎打开失败。");
		return;
	}

    // 是否显示MATLAB窗口：true-显示；false-隐藏。
    nStatus = engSetVisible(ep, false);
    if(nStatus != 0)
    {
        AfxMessageBox("MATLAB属性设置失败。");
		return;
    }

    // 获取位图的高度
    LONG lHeight = m_dib.GetHeight();
    // 获取位图的宽度
    LONG lWidth =  m_dib.GetWidth();

    // 创建位图数据矩阵
    mwSize nDim[3];
    nDim[0] = lHeight;
    nDim[1] = lWidth;
    nDim[2] = 3;
    mxArray *A = mxCreateNumericArray(3,nDim,mxUINT8_CLASS,mxREAL);

    // 获取位图数据
    LPBYTE lpData = m_dib.GetData();

    // 转化为MATLAB位图格式
    BYTE *pbData = new BYTE[lHeight*lWidth*3];
    BYTE *pbR = pbData;
    BYTE *pbG = pbData + lHeight*lWidth;
    BYTE *pbB = pbData + lHeight*lWidth*2;
    int n = 0;
    for(int j = 0; j < lWidth; j++)
    {
        for(int i = lHeight - 1 ; i >= 0 ; i--)
        {
            *(pbR+n) = lpData[i*m_dib.GetLineByte() + 3*j + 2];
            *(pbG+n) = lpData[i*m_dib.GetLineByte() + 3*j + 1];
            *(pbB+n) = lpData[i*m_dib.GetLineByte() + 3*j + 0];
            n++;           
        }
    }

    // 将数据存入位图矩阵
    memcpy(mxGetPr(A),pbData,lHeight*lWidth*3);
    // 释放内存
    delete [] pbData;

    // 将矩阵变量A写入到MATLAB引擎空间
    nStatus = engPutVariable(ep,"img",A);
    if(nStatus != 0)
    {
        AfxMessageBox("矩阵变量写入MATLAB引擎空间失败。");
		return;
    }       

    // 释放矩阵变量空间
    mxDestroyArray(A);

    // 转到M文件所在目录
    CString strCmd;
    strCmd.Format("cd %s\\CvtRgbToGray",m_strAppPath);
    nStatus = engEvalString(ep,strCmd);
    if(nStatus != 0)
    {
        AfxMessageBox("转到M文件所在目录失败。");
		return;
    }

    // 调用Simulink模块
    nStatus = engEvalString(ep,"sim('CvtRgbToGray',[0 10]);");
    if(nStatus != 0)
    {
        AfxMessageBox("调用Simulink模块失败。");
		return;
    }    

    // 获取生成的灰度位图数据
    nDim[0] = lWidth;
    nDim[1] = lHeight;
    mxArray *B = mxCreateNumericArray(2,nDim,mxUINT8_CLASS,mxREAL);
    pbData = new BYTE[lHeight*lWidth]; 
    
    // 将仿真生成的灰度位图数据存入B，并进行转置
    nStatus = engEvalString(ep,"B = grayImg(:,:,1)';");
    if(nStatus != 0)
    {
        AfxMessageBox("MATLAB执行灰度位图数据读取失败。");
		return;
    }    

    // 从MATLAB工作空间读取位图数据
    B = engGetVariable(ep,"B");
    memcpy(pbData,mxGetPr(B),lHeight*lWidth);  

    // 计算灰度位图数据所需空间
    UINT uGrayLineByte = (lWidth + 3) / 4 * 4;
    DWORD dwGrayDataSize = uGrayLineByte * lHeight;
    BYTE *pbGrayData = new BYTE[dwGrayDataSize];

    // 扩展为每行4个字节的整数倍，并上下翻转
    for(int i = 0; i < lHeight; i++)
    {
        for(int j = 0; j < lWidth; j++)
        {
            *(pbGrayData+(lHeight-i-1)*uGrayLineByte+j) = *(pbData+i*lWidth+j);
        }
    }

    // 替换为灰度数据
    m_dib.ReplaceData(pbGrayData,FALSE);

    // 释放内存
    mxDestroyArray(B);
    delete [] pbData;
    delete [] pbGrayData;

    // 关闭MATLAB引擎
    engClose(ep);           

}

void CExp_13_3_7Dlg::OnBnClickedBtnSave()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog fileDlg(FALSE,
                       "保存位图",
                        NULL,
                        OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                        "位图文件(*.bmp)|*.bmp||");
    if(fileDlg.DoModal() == IDOK)
    {
        // 获取文件名称
        CString strFile = fileDlg.GetPathName();
        // 保存文件
        m_dib.SaveToFile(strFile);
    }
}

void CExp_13_3_7Dlg::OnBnClickedBtnExit()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialog::OnOK();
}

void CExp_13_3_7Dlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
}

// 显示位图
void CExp_13_3_7Dlg::showPic(void)
{
    if(m_strFile != "")
    {          
        // 获取图形设备
        CDC *pPicDC = m_stcPic.GetDC();
        // 获取设备客户区大小
        CRect rectPic;
        m_stcPic.GetClientRect(rectPic); 
        CSize sizePic(rectPic.Width(),rectPic.Height());
        // 绘制位图
        m_dib.Draw(pPicDC, CPoint(0,0),sizePic);
        // 释放图形设备
        ReleaseDC(pPicDC);
    }
}
