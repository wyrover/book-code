//======================================================================
// 文件： BinaryMorphDib.h
// 内容： 二值形态学处理基本运算函数-头文件
// 功能： （1）腐蚀运算；
//        （2）膨胀运算；
//        （3）开与闭运算；
//        （4）击中与击不中运算；
//        （5）骨架提取算法；
// 
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"

class CBinaryMorphDib
{
public:
	// 构造函数，初始化数据成员
	CBinaryMorphDib(CDib *pDib);

	// 析构函数	
	~CBinaryMorphDib(void);

	// 腐蚀运算函数
	BOOL Erosion(int Structure[3][3]);

	// 膨胀运算函数
	BOOL Dilation(int Structure[3][3]);

	// 开运算函数
	BOOL Opening(int Structure[3][3]);

	// 闭运算函数
	BOOL Closing(int Structure[3][3]);

	// 击中与击不中运算函数
	void Hit_Miss_Transform(int hStructure[3][3], int mStructure[3][3]);

	// 骨架提取算法
	void SkeletonExtraction(int Structure[3][3]);


public:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};
