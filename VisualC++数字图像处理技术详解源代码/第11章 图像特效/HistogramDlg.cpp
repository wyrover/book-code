// HistogramDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "HistogramDlg.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"

// CHistogramDlg 对话框

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialog)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDlg::IDD, pParent)
	, m_nLimitLow(0)
	, m_nLimitUp(255)
	, m_nGray(0)
	, m_dPer(0)
{
	memset(m_lCounts, 0, sizeof(long) * 256);
	m_nIsDraging = DT_NULL;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LIMIT_LOWER, m_nLimitLow);
	DDX_Text(pDX, IDC_LIMIT_UP, m_nLimitUp);
	DDX_Control(pDX, IDC_HISTOGRAM, m_stiHistogram);
	DDX_Text(pDX, IDC_STATIC_GRAY, m_nGray);
	DDX_Text(pDX, IDC_STATIC_PER, m_dPer);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	ON_EN_CHANGE(IDC_LIMIT_LOWER, &CHistogramDlg::OnEnChangeLimitLower)
	ON_EN_CHANGE(IDC_LIMIT_UP, &CHistogramDlg::OnEnChangeLimitUp)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CHistogramDlg 消息处理程序

/******************************************************************************
*	作用:		对话框初始化时计算各级灰度数量
******************************************************************************/
BOOL CHistogramDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	for (UINT i = 0; i < pView->m_nPicWidth * pView->m_nPicHeight; i++)
	{
		int value = pView->m_pImageTempBuffer[i * 4];

		m_lCounts[value]++;
	}

	// 计算像素总个数
	m_nPixelCount = pView->m_nPicWidth * pView->m_nPicHeight;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


/******************************************************************************
*	作用:		灰度下限改变的响应函数
******************************************************************************/
void CHistogramDlg::OnEnChangeLimitLower()
{
	UpdateData(TRUE);

	// 限定取值范围
	if (m_nLimitLow < 0)
		m_nLimitLow = 0;
	else if (m_nLimitLow > 255)
		m_nLimitLow = 255;

	// 如果下限比上限大，则互换
	if (m_nLimitLow > m_nLimitUp)
	{
		int nTemp = m_nLimitLow;
		m_nLimitLow = m_nLimitUp;
		m_nLimitUp = nTemp;
	}

	Refresh();
	UpdateData(FALSE);
}


/******************************************************************************
*	作用:		灰度上限改变的响应函数
******************************************************************************/
void CHistogramDlg::OnEnChangeLimitUp()
{
	UpdateData(TRUE);

	// 限定取值范围
	if (m_nLimitUp < 0)
		m_nLimitUp = 0;
	else if (m_nLimitUp > 255)
		m_nLimitUp = 255;

	if (m_nLimitLow > m_nLimitUp)
	{
		int nTemp = m_nLimitLow;
		m_nLimitLow = m_nLimitUp;
		m_nLimitUp = nTemp;
	}

	Refresh();
	UpdateData(FALSE);
}

/******************************************************************************
*	作用:		绘制对话框
******************************************************************************/
void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Refresh();
}

/******************************************************************************
*	作用:		刷新直方图
*	备注:		双缓存绘制方法
******************************************************************************/
void CHistogramDlg::Refresh()
{
	CDC*	pDC = m_stiHistogram.GetDC();
	CRect	rect;
	CDC		memDC;
	CBitmap MemBitmap;

	// 获取绘图区域
	m_stiHistogram.GetClientRect(rect);

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

	// 绘制当前灰度区域
	Pen pen(Color::Blue);

	pen.SetDashStyle(DashStyleDash);

	graph.DrawLine(&pen, 10 + m_nLimitLow, 280, 10 + m_nLimitLow, 10);
	graph.DrawLine(&pen, 10 + m_nLimitUp, 280, 10 + m_nLimitUp, 10);

	long lMax = 0;
	REAL dHeight = 0.0;

	// 查找最大值
	for (int i = m_nLimitLow; i <= m_nLimitUp; i++)
		lMax = max(lMax, m_lCounts[i]);

	// y轴刻度
	strNum.Format(L"%d", lMax);
	graph.DrawString(strNum, -1, &font, 
					 PointF(10, 25), &SolidBrush(Color::Black));

	// 绘制柱状图
	for (int i = m_nLimitLow; i <= m_nLimitUp; i++)
	{
		dHeight = (REAL)(m_lCounts[i]) / lMax * 250;
		graph.DrawLine(&Pen(Color::Gray), i + 10.0f, 280.0f, i + 10.0f, 280 - dHeight);
	}

	// 拷贝内存画布内容
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	m_stiHistogram.ReleaseDC(pDC);
}

/******************************************************************************
*	作用:		鼠标移动时响应托拽动作
******************************************************************************/
void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;

	m_stiHistogram.GetWindowRect(rect);

	if ( (nFlags & MK_LBUTTON) && m_nIsDraging )
	{
		// 托拽偏移量
		int offset = point.x - m_psMove.x;

		// 如果托拽的是上限
		if (m_nIsDraging == DT_UP)
		{
			// 如果没有超界
			if ( (offset + m_nLimitUp) <= 255 )
			{
				if ( (offset + m_nLimitUp) >= m_nLimitLow )
					m_nLimitUp += offset;
				else
					m_nLimitUp = m_nLimitLow;
			}
			else
				m_nLimitUp = 255;
		}
		else
		{
			// 如果没有超界
			if ( (offset + m_nLimitLow) >= 0 )
			{
				if ( (offset + m_nLimitLow) <= m_nLimitUp )
					m_nLimitLow += offset;
				else
					m_nLimitLow = m_nLimitUp;
			}
			else
				m_nLimitLow = 0;
		}

		UpdateData(FALSE);
		Refresh();

		m_psMove = point;
	}
	else
		m_nIsDraging = DT_NULL;

	ClientToScreen(&point);

	// 鼠标当前所在灰度位置,如果不在0～255之间则表示鼠标不在指定区域内
	int x = point.x - rect.left - 10;

	if (abs(x - m_nLimitUp) > 3 && abs(x - m_nLimitLow) > 3)
		m_nIsDraging = DT_NULL;

	// 如果鼠标在直方图区域中
	if (rect.PtInRect(point))
	{
		if (x >= m_nLimitLow && x <= m_nLimitUp)
		{
			m_nGray = x;
			m_dPer = float(m_lCounts[x]) / m_nPixelCount * 100;
		}

		UpdateData(FALSE);
	}
}

/******************************************************************************
*	作用:		托拽时改变鼠标光标
******************************************************************************/
BOOL CHistogramDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rect;
	CPoint point;

	GetCursorPos(&point);
	m_stiHistogram.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		int x = point.x - rect.left - 10;

		if (abs(x - m_nLimitUp) <= 3 || abs(x - m_nLimitLow) <= 3)
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			return TRUE;
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/******************************************************************************
*	作用:		鼠标按下时判断是否在灰度上下限直线范围中
******************************************************************************/
void CHistogramDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	CPoint oldPoint = point;

	m_stiHistogram.GetWindowRect(rect);
	ClientToScreen(&point);
	int x = point.x - rect.left - 10;

	if (abs(x - m_nLimitUp) <= 3)
	{
		m_psMove = oldPoint;
		m_nIsDraging = DT_UP;
	}
	else if (abs(x - m_nLimitLow) <= 3)
	{
		m_psMove = oldPoint;
		m_nIsDraging = DT_LOW;
	}
}

/******************************************************************************
*	作用:		释放鼠标的响应函数
******************************************************************************/
void CHistogramDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nIsDraging = DT_NULL;
}
