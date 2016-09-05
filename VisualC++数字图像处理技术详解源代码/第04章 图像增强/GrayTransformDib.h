//======================================================================
// 文件： GrayTransformDib.h
// 内容： 图像灰度变换增强函数-头文件
// 功能： （1）线性灰度增强；
//        （2）分段线性灰度增强；
//        （3）对数函数非线性灰度增强；
//        
// 
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"

class CGrayTransformDib
{
public:
	// 构造函数，初始化数据成员
	CGrayTransformDib(CDib *pDib);

	// 析构函数	
	~CGrayTransformDib(void);

	// 线性灰度增强
	BOOL Linear_Transform( BYTE gMin, BYTE gMax);

	// 分段线性灰度增强
	BOOL Seg_Linear_Transform( BYTE gSrc1, BYTE gSrc2,BYTE gDst1, BYTE gDst2);

	// 对数函数非线性灰度增强
	BOOL Log_Transform( double a, double b, double c);

	

private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};
