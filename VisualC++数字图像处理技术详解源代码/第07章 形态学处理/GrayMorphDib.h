//======================================================================
// 文件： GrayMorphDib.h
// 内容： 灰度形态学处理基本运算函数-头文件
// 功能： （1）腐蚀运算；
//        （2）膨胀运算；
//        （3）开与闭运算；
//        （4）灰度形态学梯度；
//        （5）Top-Hat变换；
//
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"


class CGrayMorphDib
{
public:
	// 构造函数，初始化数据成员
	CGrayMorphDib(CDib *pDib);

	// 析构函数
	~CGrayMorphDib(void);

	// 腐蚀运算函数
	void Erosion(int Structure[3][3]);

	// 膨胀运算函数
	void Dilation(int Structure[3][3]);

	// 开运算函数
	void Opening(int Structure[3][3]);

	// 闭运算函数
	void Closing(int Structure[3][3]);

	// 灰度形态学梯度算法
	void Grads(int Structure[3][3]);

	// Top-Hat变换
	void Top_Hat(int Structure[3][3]);


private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 
};
