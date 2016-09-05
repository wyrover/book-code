// PowerTranDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "PowerTranDlg.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"
#include "GrayOperator.h"
#include <cmath>


// CPowerTranDlg 对话框

IMPLEMENT_DYNAMIC(CPowerTranDlg, CDialog)

CPowerTranDlg::CPowerTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPowerTranDlg::IDD, pParent)
	, m_nPosC(0)
	, m_nPosR(0)
	, m_nC(0)
	, m_nR(0)
	, m_nPosB(0)
	, m_nB(0)
{

}

CPowerTranDlg::~CPowerTranDlg()
{
	SAFE_DELETE (dlg);
}

void CPowerTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDERC, m_nPosC);
	DDX_Slider(pDX, IDC_SLIDERR, m_nPosR);
	DDX_Text(pDX, IDC_EDITC, m_nC);
	DDX_Text(pDX, IDC_EDITR, m_nR);
	DDX_Control(pDX, IDC_PREV, m_stiPrev);
	DDX_Slider(pDX, IDC_SLIDERB, m_nPosB);
	DDX_Text(pDX, IDC_EDITB, m_nB);
}


BEGIN_MESSAGE_MAP(CPowerTranDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDITC, &CPowerTranDlg::OnEnChangeEditc)
	ON_EN_CHANGE(IDC_EDITR, &CPowerTranDlg::OnEnChangeEditr)
	ON_BN_CLICKED(IDC_SAVE, &CPowerTranDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CPowerTranDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDITB, &CPowerTranDlg::OnEnChangeEditb)
END_MESSAGE_MAP()


// CPowerTranDlg 消息处理程序

BOOL CPowerTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDERC))->SetRange(1, 20);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDERR))->SetRange(0, 2500);

	m_nC = m_nPosC = 10;
	m_nR = m_nPosR = 20;
	m_nPosB = 50;
	UpdateData(FALSE);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDERB))->SetRange(-100, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPowerTranDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	Refresh();
}

void CPowerTranDlg::Refresh()
{
	UpdateData(TRUE);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	PowerTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
			   pView->m_nPicWidth, pView->m_nPicHeight, 
			   m_nB, m_nC * 0.1, m_nR * 0.01);

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

	// 转换坐标,将(10, 280)转换为坐标原点
	Matrix matrix1;
	Matrix matrix2(0, 1, 1, 0, 0, 0);	// 转置矩阵

	matrix1.Translate(10, 280);
	matrix1.Rotate(270);
	matrix1.Multiply(&matrix2);
	graph.SetTransform(&matrix1);


	// 根据映射表绘制曲线
	// 映射表,用于256种灰度变换后的值
	double map[256];

	// 计算当前像素变换后的值
	for (int i = 0; i < 256; i++)
		map[i] = m_nC * 0.1 * pow(i / 255.0, m_nR * 0.01) * 255 + m_nB;

	GraphicsPath path;

	path.StartFigure();
	for (int i = 0; i < 255; i++)
		path.AddLine(i, (int)map[i], i+1, (int)map[i+1]);

	graph.DrawPath(&Pen(Color::Black), &path);

	// 拷贝内存画布内容
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	m_stiPrev.ReleaseDC(pDC);
}

void CPowerTranDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	if (pScrollBar == GetDlgItem(IDC_SLIDERC))
		m_nC = m_nPosC;
	else if (pScrollBar == GetDlgItem(IDC_SLIDERR))
		m_nR = m_nPosR;
	else if (pScrollBar == GetDlgItem(IDC_SLIDERB))
		m_nB = m_nPosB;

	UpdateData(FALSE);
	Refresh();
	m_bModify = true;

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPowerTranDlg::OnEnChangeEditb()
{
	UpdateData(TRUE);

	if (m_nB < -100)
		m_nB = -100;
	else if (m_nB > 100)
		m_nB = 100;

	m_nPosB = m_nB;
	UpdateData(FALSE);
	Refresh();
	m_bModify = true;
}


void CPowerTranDlg::OnEnChangeEditc()
{
	UpdateData(TRUE);

	if (m_nC < 1)
		m_nC = 1;
	else if (m_nC > 20)
		m_nC = 20;

	m_nPosC = m_nC;
	UpdateData(FALSE);
	Refresh();
	m_bModify = true;
}

void CPowerTranDlg::OnEnChangeEditr()
{
	UpdateData(TRUE);

	if (m_nR < 0)
		m_nR = 0;
	else if (m_nR > 2500)
		m_nR = 2500;

	m_nPosR = m_nR;
	UpdateData(FALSE);
	Refresh();
	m_bModify = true;
}

void CPowerTranDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}

void CPowerTranDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}

	OnOK();
}