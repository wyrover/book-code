//======================================================================
// 文件： HistogramDib.h
// 内容： 直方图增强函数-头文件
// 功能： （1）直方图统计函数；
//        （2）直方图均衡化函数；
//        （3）直方图匹配（规定化）函数；
// 
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"

class CHistogramDib
{
public:
	// 构造函数，初始化数据成员
	CHistogramDib(CDib *pDib);

	// 析构函数	
	~CHistogramDib(void);

	// 直方图统计函数
	void Histogram_Statistic( float *probability);

	// 直方图均衡化函数
	BOOL Histogram_Equalization( );

	// 直方图规定化（匹配）函数
	BOOL Histogram_Match( BYTE bGray, int *npMap, float *fpPro);

	

private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};
