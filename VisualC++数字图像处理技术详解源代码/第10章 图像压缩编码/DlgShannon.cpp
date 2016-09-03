// DlgShannon.cpp : implementation file
//

#include "stdafx.h"
#include "imageprocessing.h"
#include "DlgCoding.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog


CDlgShannon::CDlgShannon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShannon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShannon)
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;
	m_dEfficiency = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgShannon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShannon)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Text(pDX, IDC_EDIT1, m_dEntropy);
	DDX_Text(pDX, IDC_EDIT2, m_dAvgCodeLen);
	DDX_Text(pDX, IDC_EDIT3, m_dEfficiency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShannon, CDialog)
	//{{AFX_MSG_MAP(CDlgShannon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon message handlers

BOOL CDlgShannon::OnInitDialog() 
{
	// 调用默认得OnInitDialog()函数
	CDialog::OnInitDialog();

	// 初始化变量
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;

	// 字符串变量
	CString	str;
	str = "1";
	
	// 循环变量
	LONG	i;
	LONG	j;

	// 控件ListCtrl的ITEM
	LV_ITEM lvItem;

	// 保存控件ListCtrl中添加的ITEM编号
	int		nItem2View;	
	
	// 中间变量
	double	dT;
	LONG	lTemp;
	
	// 保存计算中间结果的数组
	double * dTemp;
	dTemp = new double[m_nColorNum];
	
	// 数组用来存放灰度值和其位置之间的映射
	LONG *	l4Turn;
	l4Turn = new LONG[m_nColorNum];
	
	// 当前编码区间的概率和
	double	dAccum;
	dAccum = 1.0;

	// 已经编码灰度值概率的统计和
	double	dSum;
	dSum    = 0;
	
	// 已编码灰度值
	LONG	lCount = 0;
		
	// 起始位置
	LONG	lBegin;
	
	// 指示编码是否已经完成一段
	BOOL	* bFinished;
	bFinished = new BOOL[m_nColorNum];
	
	// 分配内存	
	m_strCode = new CString[m_nColorNum];
			
	for (i = 0; i < m_nColorNum; i ++)
	{			
		
		// 初始化为FALSE
		bFinished[i] = FALSE;

		// 将概率赋值dTemp数组
		dTemp[i] = m_dProba[i];
		
		// 按灰度值大小顺序排列
		l4Turn[i] = i;
		
	}
	
	// 用冒泡法对进行灰度值出现的概率排序
	// 同时改变灰度值位置的映射关系
	for (j = 0; j < m_nColorNum - 1; j ++)
		for (i = 0; i < m_nColorNum - j - 1; i ++)
			if (dTemp[i] > dTemp[i + 1])
			{
				// 互换
				dT = dTemp[i];
				dTemp[i] = dTemp[i + 1];
				dTemp[i + 1] = dT;
				
				// 将i和i+1灰度的位置值互换
				lTemp = l4Turn[i];
				l4Turn[i] = l4Turn[i+1];
				l4Turn[i+1] = lTemp;
				
			}
		
	/*******************************************************
	计算香农－弗诺编码表
	*******************************************************/
	
	// 从概率大于0处开始编码
	for (lBegin = 0; lBegin < m_nColorNum - 1; lBegin ++)
		if (dTemp[lBegin] > 0)
			break;

	// 开始编码
	while(lCount < m_nColorNum)
	{
		// 从概率大于零的灰度值开始编码
		lCount = lBegin;
		
		// 对区间进行分割，对每个区间的灰度值编码
		for (i = lBegin; i < m_nColorNum; i ++)
		{
			// 判断是否编码完成
			if (bFinished[i] == FALSE)
			{
				// 增加当前编码区间的概率综合
				dSum += dTemp[i];
				
				// 判断是否超出总和的一半
				if (dSum > dAccum/2.0)
				{
					// 超出，追加的字符改为0
					str = "0";
				}
				
				// 追加字符
				m_strCode[l4Turn[i]] = m_strCode[l4Turn[i]] + str;
				
				// 判断是否编码完一段
				if (dSum == dAccum)
				{
					// 完成一部分编码，重新计算dAccum
					
					// 初始化dSum为0
					dSum = 0;
					
					// 判断是不是对所有灰度值已经编码一遍
					if (i == m_nColorNum - 1)
						j = lBegin;
					else
						j = i + 1;
					
					// 保存j值
					lTemp = j;
					str = m_strCode[l4Turn[j]];
					
					// 计算下一编码区间的概率总和
					dAccum = 0;
					for (; j < m_nColorNum; j++)
					{
						// 判断是否是同一段编码
						if ((m_strCode[l4Turn[j]].Right(1) != str.Right(1)) 
							|| (m_strCode[l4Turn[j]].GetLength() != str.GetLength()))
							break;

						// 当前区间的概率总和增加
						dAccum += dTemp[j];
					}
					
					// 码字增加值为1
					str = "1";
					
					// 判断该段编码已经完成
					if (lTemp + 1 == j)
						bFinished[lTemp] = TRUE;
				}
			}
			else
			{
				//  开始下一轮编码
				lCount ++;
				
				// 重新赋2dSum为0
				dSum = 0;
				
				// 判断是不是对所有灰度值已经编码一遍
				if (i == m_nColorNum - 1)
					j = lBegin;
				else
					j = i + 1;
				
				// 保存j值
				lTemp = j;
				str = m_strCode[l4Turn[j]];
				
				// 计算下一编码区间的概率总和
				dAccum = 0;
				for (; j < m_nColorNum; j++)
				{
					// 判断是否是同一段编码
					if ((m_strCode[l4Turn[j]].Right(1) != str.Right(1)) 
						|| (m_strCode[l4Turn[j]].GetLength() != str.GetLength()))
					{
						// 退出循环
						break;
					}
					
					// 累加
					dAccum += dTemp[j];
				}
				
				str = "1";
				
				// 判断该段编码已经完成
				if (lTemp + 1 == j)
					bFinished[lTemp] = TRUE;
			}
		}
	}
	
	// 计算图像熵
	for (i = 0; i < m_nColorNum; i ++)
	{
		// 判断概率是否大于0
		if (m_dProba[i] > 0)
		{
			// 计算图像熵
			m_dEntropy -= m_dProba[i] * log(m_dProba[i]) / log(2.0);
		}
	}
	
	// 计算平均码字长度
	for (i = 0; i < m_nColorNum; i ++)
	{
		// 累加
		m_dAvgCodeLen += m_dProba[i] * m_strCode[i].GetLength();
	}
	
	// 计算编码效率
	m_dEfficiency = m_dEntropy / m_dAvgCodeLen;
	
	// 保存变动
	UpdateData(FALSE);
	
	/*************************************************
	输出编码结果
	*************************************************/	
	
	// 设置List控件样式
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	
	// 给List控件添加Header
	m_lstTable.InsertColumn(0, "灰度值", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "灰度值概率", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "香农－弗诺编码", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "码字长度", LVCFMT_LEFT, 78, 2);
	
	// 设置样式为文本
	lvItem.mask = LVIF_TEXT;
	
	// 添加显示
	for (i = 0; i < m_nColorNum; i ++)
	{
		// 第一列显示
		lvItem.iItem = m_lstTable.GetItemCount();
		str.Format("%u",i);
		lvItem.iSubItem = 0;
		lvItem.pszText= (LPTSTR)(LPCTSTR)str;
		nItem2View = m_lstTable.InsertItem(&lvItem);
		
		// 其它列显示
		lvItem.iItem = nItem2View;
		
		// 添加灰度值出现的频率
		lvItem.iSubItem = 1;
		str.Format("%f",m_dProba[i]);
		lvItem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvItem);
		
		// 添加香农－弗诺编码
		lvItem.iSubItem = 2;
		lvItem.pszText = (LPTSTR)(LPCTSTR)m_strCode[i];
		m_lstTable.SetItem(&lvItem);
		
		// 添加码字长度
		lvItem.iSubItem = 3;
		str.Format("%u",m_strCode[i].GetLength());
		lvItem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvItem);
	}
	
	//  内存释放
	delete l4Turn;
	delete dTemp;
	delete bFinished;
	
	return TRUE;
}
