//======================================================================
// 文件： SmoothProcessDib.h
// 内容： 图像平滑处理函数-头文件
// 功能： （1）邻域平均平滑函数；
//        （2）加权平均平滑函数；
//        （3）选择式掩膜平滑函数；
//        （4）中值滤波平滑函数       
// 
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"

class CSmoothProcessDib
{
public:
	// 构造函数，初始化数据成员
	CSmoothProcessDib(CDib *pDib);

	// 析构函数	
	~CSmoothProcessDib(void);

	// 邻域平均平滑函数
	BOOL Average_Smooth( );

	// 加权平均平滑函数
	BOOL Value_Average_Smooth( int Structure[3][3]);

	// 选择式掩膜平滑函数
	BOOL Select_Smooth( );

	// 中值滤波平滑函数
	BOOL Middle_Smooth( );

	

private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};
