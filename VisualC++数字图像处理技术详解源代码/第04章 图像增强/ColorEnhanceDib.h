//======================================================================
// 文件： ColorEnhanceDib.h
// 内容： 图像灰度的颜色增强-头文件
// 功能： （1）伪彩色增强
//        
// 
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"

class CColorEnhanceDib
{
public:
	// 构造函数，初始化数据成员
	CColorEnhanceDib(CDib *pDib);

	// 析构函数	
	~CColorEnhanceDib(void);

	// 伪彩色增强
	void Pseudo_Color_Enhance( BYTE * bpColorsTable );

	
private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};
