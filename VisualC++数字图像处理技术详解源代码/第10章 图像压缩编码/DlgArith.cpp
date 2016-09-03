/////////////////////////////////////////////////////////////////////////////
// CDlgArith dialog
#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgCoding.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define fPro4Zero  0.25;
#define fPro4One   0.75;
CDlgArith::CDlgArith(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgArith::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgArith)
	m_ArithSerial = _T("");
	m_ArithOutput = _T("");
	m_ArithDecode = _T("");
	//}}AFX_DATA_INIT
}


void CDlgArith::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgArith)
	DDX_Control(pDX, IDC_EDIT1, m_ConArithSer);
	DDX_Control(pDX, IDCODING, m_coding);
	DDX_Control(pDX, IDDECODING, m_decoding);
	DDX_Text(pDX, IDC_EDIT1, m_ArithSerial);
	DDV_MaxChars(pDX, m_ArithSerial, 15);
	DDX_Text(pDX, IDC_EDIT2, m_ArithOutput);
	DDX_Text(pDX, IDC_EDIT4, m_ArithDecode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgArith, CDialog)
	//{{AFX_MSG_MAP(CDlgArith)
	ON_BN_CLICKED(IDDECODING, OnDecoding)
	ON_BN_CLICKED(IDCODING, OnCoding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgArith message handlers

//void CDlgArith::OnOK() 
//{
//	CDialog::OnOK();
//}
///////////////////////////////////////////////////////////////
// DlgCodingIArith dialog



/***********************************************************
 实现对已经编码的码字进行解码的功能
 *********************************************************
*/

void CDlgArith::OnDecoding() 
{
	// 二值序列的长度
	int nOutLength;

	// 算术编码的长度
	int nInLength;

	// 编码区间的上限和下限  
	double dHigh=1.0;
	double dLow=0.0;

	// 编码区间的长度
	double dRange=1.0;

	// 判断二值序列是否全零
	int nNo1=0;
	
	// 循环变量
	int i;
	
	// 二进制表示为十进制
	double dTenCode=0;
	
	// 中间变量
	double d2Pow;
	double dTemp;

	// 接收数据
	UpdateData(TRUE);

	// 解码显示清空
	m_ArithDecode = _T("");
	
	// 显示数据
	UpdateData(FALSE);
	
	// 算术编码的长度
	nInLength =m_ArithOutput.GetLength();
	
	// 将二进制序列转化为十进制，并判断是否为零
	for (i=0; i<nInLength;i++ )
	{
		// 二进制的每位对应十进制的值
		d2Pow = pow(0.5,i+1);
		if(m_ArithOutput.Mid(i,1)=='1')
		{
			dTenCode=dTenCode+d2Pow;
			nNo1++;
		}
	}

	// 二值序列的长度
	nOutLength = m_ArithSerial.GetLength();

	// 分全零和非全零两种情况解码
	if(nNo1!=0)
	{
		for(i=0;i<nOutLength;i++)
		{
			dTemp=dLow+dRange*fPro4Zero;

			if(dTemp>dTenCode)
			{
				// 输出0
				m_ArithDecode=m_ArithDecode+'0';
				
				// 编码区间上下限的计算
				dLow=dLow;
				dHigh=dLow+dRange*fPro4Zero;
				
				// 区间范围
				dRange=dHigh-dLow;
			}
			else
			{
				// 输出1
				m_ArithDecode=m_ArithDecode+'1';
				
				// 编码区间上下限的计算
				dLow=dLow+dRange*fPro4Zero;
				dHigh=dHigh;

				// 区间范围
				dRange=dHigh-dLow;
			}
		}
	}
	else
	{
		for(i=0;i<nOutLength;i++)
			m_ArithDecode=m_ArithDecode+'1';
	}
	

	// 数据输出更新
	UpdateData(FALSE);

	// 在重新输入编码前不允许输入
	m_decoding.EnableWindow(FALSE);

	// 允许输入
	m_ConArithSer.EnableWindow(TRUE);
	
	// 允许解码
	m_coding.EnableWindow(TRUE);
	
}

/***********************************************************
 实现对输入二进制序列进行编码的功能
 *********************************************************
*/

void CDlgArith::OnCoding() 
{
	
	// 输入二值序列长度
	int nInLength;

	// 输出码字长度
	int nOutLength;

	// 循环变量
	int i;

	// 中间变量
	double dTemp;

	// 编码区间的上限和下限  
	double dHigh=1.0;
	double dLow=0.0;

	// 编码区间的长度
	double dRange=1.0;

	// 累积概率
	double dAccuPro=1.0;
	
    // 接收输入的数据
	UpdateData(TRUE);
	
	// 清除输出的编码码字
	m_ArithOutput = _T("");
	
	// 输出
	UpdateData(FALSE);
	
	// 令编码按钮无效
	m_decoding.EnableWindow(FALSE);
	
	// 输入的二值序列长度
	nInLength = m_ArithSerial.GetLength();
 
	//  对二值序列进行编码
	for (i = 0; i < nInLength; i++ )
	{
		
		// 如果输入为1
		if(m_ArithSerial.Mid(i,1) == "1")
		{
			// 编码区间上下限的计算
			dHigh = dHigh;
			dLow  = dLow + dRange*fPro4Zero;

			// 编码区间长度
			dRange = dHigh - dLow;

			// 二值序列出现概率的计算
			dAccuPro = dAccuPro * fPro4One;			
		}
		// 如果输入为0
		else if(m_ArithSerial.Mid(i,1) == "0")
		{
			// 编码区间上下限的计算
			dHigh = dLow + dRange*fPro4Zero;
			dLow  = dLow;

			// 编码区间长度
			dRange = dHigh - dLow;

			// 二值序列出现概率的计算
			dAccuPro = dAccuPro * fPro4Zero;			
		}
		// 如果输入非二值序列
		else
		{
			// 重新输入
			MessageBox("请输入二值序列!", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
			return;
		}
	}

	// 计算输出码字的长度
	dTemp = floor( -log10(dAccuPro) / log10(2.0f) ) + 1;
	nOutLength = (int)dTemp;

	dTemp = dLow;
	double d2Pow;
	
	// 将十进制的小数转化成二进制的小数表示
	for (i = 0; i < nOutLength; i++ )
	{
		// 二进制小数第i对应的十进制值
		d2Pow = pow(0.5,i + 1);
		
		// 判断当前位赋值0或者1
		if(dTemp >= d2Pow)
		{
			m_ArithOutput = m_ArithOutput + "1";
			dTemp = dTemp - d2Pow;
		}
		else 
			m_ArithOutput = m_ArithOutput + "0";
	}
	
	// 转化后是否有余数
	if(dTemp > 0)
	{
		// 二进制小数进行进位
		for(i = nOutLength-1; i >= 0; i--)
		{
			// 进位，1转化为0
			if(m_ArithOutput.Mid(i,1) == '1')
			{
				m_ArithOutput.Delete(i,1);
				m_ArithOutput.Insert(i,"0");
			}
			// 进位完成，最后的0位转化为1
			else
			{
				m_ArithOutput.Delete(i,1);
				m_ArithOutput.Insert(i,"1");
				break;
			}
		}
	}
     
	// 编码完成，数据更新
	UpdateData(FALSE);

	// 允许进行解码
	m_decoding.EnableWindow(TRUE);

	// 解码前禁止编码
	m_coding.EnableWindow(FALSE);

	// 解码前禁止输入新的二进制序列
	m_ConArithSer.EnableWindow(FALSE);

}
