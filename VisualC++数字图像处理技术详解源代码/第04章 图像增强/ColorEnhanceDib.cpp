//======================================================================
// 文件： ColorEnhanceDib.cpp
// 内容： 图像灰度的颜色增强-源文件
// 功能： （1）伪彩色增强
//        
// 
//======================================================================
#include "StdAfx.h"
#include "ColorEnhanceDib.h"

//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CColorEnhanceDib::CColorEnhanceDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CColorEnhanceDib::~CColorEnhanceDib(void)
{
	
}


//=======================================================
// 函数功能： 伪彩色增强
// 输入参数： BYTE * bpColorsTable-指向要替换的伪彩色编码表
// 返回值：   无
//=======================================================

void CColorEnhanceDib::Pseudo_Color_Enhance( BYTE * bpColorsTable )
{
    int i;					// 循环变量	
	DWORD wNumColors;		// 颜色表中的颜色数目
    LPRGBQUAD m_lpRgbQuad;

    wNumColors = m_pDib->GetNumOfColor();	// 获取原图像颜色表中的颜色数目
    m_lpRgbQuad= m_pDib->GetRgbQuad();      //获取元图像颜色表指针
		
	if (wNumColors == 256)				// 判断颜色数目是否是256色
	{		
				
		// 读取伪彩色编码，更新调色板
		for (i = 0; i < (int)wNumColors; i++)
		{    
            // 更新调色板红色绿色蓝色分量
		    (m_lpRgbQuad+i)->rgbBlue = bpColorsTable[i * 4];
			(m_lpRgbQuad+i)->rgbGreen = bpColorsTable[i * 4 + 1];
			(m_lpRgbQuad+i)->rgbRed = bpColorsTable[i * 4 + 2];				
			// 更新调色板保留位
			(m_lpRgbQuad+i)->rgbReserved = 0; 			
		}
	}
	
}