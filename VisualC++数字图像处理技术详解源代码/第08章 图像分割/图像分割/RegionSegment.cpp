#include "StdAfx.h"
#include "RegionSegment.h"

CRegionSegment::CRegionSegment(CDib *pDib)
{
    m_pDib=pDib;
}

CRegionSegment::~CRegionSegment(void)
{
}

//==========================================================
// 函数功能： 区域生长
// 输入参数： point:根据用户在原图上点击鼠标左键确定的种子点
// 返回值：   无
//==========================================================
void CRegionSegment::RegionGrow(CPoint point)
{
    // 循环变量
	int i, j;

	// 原图像数据区指针
	LPBYTE p_data;
    p_data = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
	                                                                                                            	
	// 计算种子点的灰度值
	int nSeedPoint = *(p_data + (nHeight - point.y) * nLineByte + point.x);	

	// 对各个像素进行灰度转换
	for (j = 0; j < nHeight; j ++)
		for (i = 0; i < nWidth; i ++)
		{
			//当前像素点的灰度值
			int nTemp = *(p_data + nLineByte * j + i);

            //如果当前点与种子点的灰度值之差的绝对值大于阈值10，则将该点置为255
			if ( abs(nTemp - nSeedPoint) > 10 )    
				*(p_data + nLineByte * j + i) = 255;
		}
}
