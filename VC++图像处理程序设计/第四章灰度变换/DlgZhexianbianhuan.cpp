// DlgZhexianbianhuan.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgZhexianbianhuan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgZhexianbianhuan dialog


CDlgZhexianbianhuan::CDlgZhexianbianhuan(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZhexianbianhuan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgZhexianbianhuan)
	m_bX1 = 0;
	m_bX2 = 0;
	m_bY1 = 0;
	m_bY2 = 0;
	//}}AFX_DATA_INIT
}


void CDlgZhexianbianhuan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgZhexianbianhuan)
	DDX_Text(pDX, IDC_EDIT_X1, m_bX1);
	DDX_Text(pDX, IDC_EDIT_X2, m_bX2);
	DDX_Text(pDX, IDC_EDIT_Y1, m_bY1);
	DDX_Text(pDX, IDC_EDIT_Y2, m_bY2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgZhexianbianhuan, CDialog)
	//{{AFX_MSG_MAP(CDlgZhexianbianhuan)
	ON_EN_KILLFOCUS(IDC_EDIT_X1, OnKillfocusEditX1)
	ON_EN_KILLFOCUS(IDC_EDIT_X2, OnKillfocusEditX2)
	ON_EN_KILLFOCUS(IDC_EDIT_Y1, OnKillfocusEditY1)
	ON_EN_KILLFOCUS(IDC_EDIT_Y2, OnKillfocusEditY2)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgZhexianbianhuan message handlers

BOOL CDlgZhexianbianhuan::OnInitDialog() 
{
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgZhexianbianhuan::OnOK() 
{
	// 判断是否下限超过上限
	if (m_bX1 > m_bX2)
	{
		// 互换
		BYTE bTemp = m_bX1;
		m_bX1 = m_bX2;
		m_bX2 = bTemp;
		bTemp = m_bY1;
		m_bY1 = m_bY2;
		m_bY2 = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	CDialog::OnOK();
}

void CDlgZhexianbianhuan::OnKillfocusEditX1() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_bX1 > m_bX2)
	{
		// 互换
		BYTE bTemp = m_bX1;
		m_bX1 = m_bX2;
		m_bX2 = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	Invalidate();
}

void CDlgZhexianbianhuan::OnKillfocusEditX2() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_bX1 > m_bX2)
	{
		// 互换
		BYTE bTemp = m_bX1;
		m_bX1 = m_bX2;
		m_bX2 = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	Invalidate();
}

void CDlgZhexianbianhuan::OnKillfocusEditY1() 
{
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_bY1 > m_bY2)
	{
		// 互换
		BYTE	bTemp = m_bY1;
		m_bY1 = m_bY2;
		m_bY2 = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	Invalidate();
}

void CDlgZhexianbianhuan::OnKillfocusEditY2() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_bY1 > m_bY2)
	{
		// 互换
		BYTE bTemp = m_bY1;
		m_bY1 = m_bY2;
		m_bY2 = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	Invalidate();
}

void CDlgZhexianbianhuan::OnPaint() 
{
	// 字符串
	CString str;
	
	// 设备上下文
	CPaintDC dc(this);
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,280);
	
	// 水平轴
	pDC->LineTo(320,280);
	
	// 写坐标
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// 绘制X轴箭头
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 绘制坐标值
	str.Format("(%d, %d)", m_bX1, m_bY1);
	pDC->TextOut(m_bX1 + 10, 281 - m_bY1, str);
	str.Format("(%d, %d)", m_bX2, m_bY2);
	pDC->TextOut(m_bX2 + 10, 281 - m_bY2, str);
	
	// 绘制用户指定的变换直线
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bX1 + 10, 280 - m_bY1);
	pDC->LineTo(m_bX2 + 10, 280 - m_bY2);
	pDC->LineTo(265, 25);
	
	// 绘制点边缘的小矩形
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// 选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// 绘制小矩形
	pDC->Rectangle(m_bX1 + 10 - 2, 280 - m_bY1 - 2, m_bX1 + 12, 280 - m_bY1 + 2);
	pDC->Rectangle(m_bX2 + 10 - 2, 280 - m_bY2 - 2, m_bX2 + 12, 280 - m_bY2 + 2);
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 绘制边缘
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	
	// Do not call CDialog::OnPaint() for painting messages
}
