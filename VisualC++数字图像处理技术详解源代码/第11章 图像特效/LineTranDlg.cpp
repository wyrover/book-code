// LineTranDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "LineTranDlg.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"
#include <cmath>

#include "GrayOperator.h"


// CLineTranDlg 对话框

IMPLEMENT_DYNAMIC(CLineTranDlg, CDialog)

CLineTranDlg::CLineTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineTranDlg::IDD, pParent)
	, m_dSlope(1)
	, m_dIntercept(0)
{
	dlg = NULL;
	m_bModify = false;
}

CLineTranDlg::~CLineTranDlg()
{
	SAFE_DELETE (dlg);
}

void CLineTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SLOPE, m_dSlope);
	DDX_Text(pDX, IDC_INTERCEPT, m_dIntercept);
	DDX_Control(pDX, IDC_PREV, m_stiPrev);
}

BEGIN_MESSAGE_MAP(CLineTranDlg, CDialog)
	ON_EN_CHANGE(IDC_SLOPE, &CLineTranDlg::OnEnChangeSlope)
	ON_EN_CHANGE(IDC_INTERCEPT, &CLineTranDlg::OnEnChangeIntercept)
	ON_BN_CLICKED(IDC_SAVE, &CLineTranDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CLineTranDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLineTranDlg::OnBnClickedCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


/******************************************************************************
*	作用:		对话框初始化时显示预览对话框
******************************************************************************/
BOOL CLineTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/******************************************************************************
*	作用:		对话框重绘函数
******************************************************************************/
void CLineTranDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	Refresh();
}

/******************************************************************************
*	作用:		刷新预览框
******************************************************************************/
void CLineTranDlg::Refresh()
{
	UpdateData(TRUE);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	LineTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight, m_dSlope, m_dIntercept);

	dlg->Refresh();

	CDC*	pDC = m_stiPrev.GetDC();
	CRect	rect;
	CDC		memDC;
	CBitmap MemBitmap;

	// 获取绘图区域
	m_stiPrev.GetClientRect(rect);

	// 设备描述表初始化
	memDC.CreateCompatibleDC(NULL);

	// 建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// 选取空白位图
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

	Graphics graph(memDC.GetSafeHdc());

	// 使用白色背景
	graph.FillRectangles(&SolidBrush(Color::White), 
		&Rect(0, 0, rect.Width(), rect.Height()), 
		1);

	// 绘制y轴
	graph.DrawLine(&Pen(Color::Black), 10, 10, 10, 280);
	graph.DrawLine(&Pen(Color::Black), 10, 10, 5, 15);
	graph.DrawLine(&Pen(Color::Black), 10, 10, 15, 15);

	// 绘制x轴
	graph.DrawLine(&Pen(Color::Black), 10, 280, 290, 280);
	graph.DrawLine(&Pen(Color::Black), 290, 280, 285, 285);
	graph.DrawLine(&Pen(Color::Black), 290, 280, 285, 275);

	// 绘制坐标原点
	CString strNum;
	Font font(L"宋体", 10);
	strNum = L"0";
	graph.DrawString(strNum, -1, &font, 
		PointF(8, 290), &SolidBrush(Color::Black));

	for (int i = 0; i < 256; i += 5)
	{
		if (i % 50 == 0)
			graph.DrawLine(&Pen(Color::Black), 10 + i, 280, 10 + i, 286);
		else if (i % 10 == 0)
			graph.DrawLine(&Pen(Color::Black), 10 + i, 280, 10 + i, 283);
	}

	// 绘制x轴刻度
	strNum = L"50";
	graph.DrawString(strNum, -1, &font, 
		PointF(53, 290), &SolidBrush(Color::Black));
	strNum = L"100";
	graph.DrawString(strNum, -1, &font, 
		PointF(100, 290), &SolidBrush(Color::Black));
	strNum = L"150";
	graph.DrawString(strNum, -1, &font, 
		PointF(150, 290), &SolidBrush(Color::Black));
	strNum = L"200";
	graph.DrawString(strNum, -1, &font, 
		PointF(200, 290), &SolidBrush(Color::Black));
	strNum = L"255";
	graph.DrawString(strNum, -1, &font, 
		PointF(255, 290), &SolidBrush(Color::Black));
	// y轴
	graph.DrawString(strNum, -1, &font, 
		PointF(10, 25), &SolidBrush(Color::Black));

	// 绘制变换函数图像
	PointF startPs;
	PointF endPs;

	if ( m_dIntercept >= 0.0 || fabs(m_dSlope-0.0) < 0.0001)
	{
		startPs.X = 0;
		startPs.Y = (REAL)m_dIntercept;
	}
	else
	{
		startPs.X = (REAL)(-m_dIntercept/m_dSlope);
		startPs.Y = 0;
	}

	endPs.X = 255;
	endPs.Y = (REAL)(m_dSlope * 255 + m_dIntercept);

	if (endPs.Y < 255 && fabs(m_dSlope-0.0) < 0.000)
	{
		endPs.Y = 255;
		endPs.X = (REAL)(255-m_dIntercept/m_dSlope);
	}

	// 转换坐标,将(10, 280)转换为坐标原点
	Matrix matrix1;
	Matrix matrix2(0, 1, 1, 0, 0, 0);	// 转置矩阵

	matrix1.Translate(10, 280);
	matrix1.Rotate(270);
	matrix1.Multiply(&matrix2);
	graph.SetTransform(&matrix1);

	graph.DrawLine(&Pen(Color::Black), startPs.X, startPs.Y, endPs.X, endPs.Y);

	// 拷贝内存画布内容
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	m_stiPrev.ReleaseDC(pDC);
}

/******************************************************************************
*	作用:		改变斜率时的响应函数
******************************************************************************/
void CLineTranDlg::OnEnChangeSlope()
{
	UpdateData(TRUE);

	Refresh();
	m_bModify = true;
}


/******************************************************************************
*	作用:		改变截距时的响应函数
******************************************************************************/
void CLineTranDlg::OnEnChangeIntercept()
{
	UpdateData(TRUE);

	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	作用:		“保存”按钮响应函数
******************************************************************************/
void CLineTranDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}

/******************************************************************************
*	作用:		“确定”按钮的响应函数
******************************************************************************/
void CLineTranDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}

	OnOK();
}

/******************************************************************************
*	作用:		“取消”按钮的响应函数
******************************************************************************/
void CLineTranDlg::OnBnClickedCancel()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}

	OnCancel();
}