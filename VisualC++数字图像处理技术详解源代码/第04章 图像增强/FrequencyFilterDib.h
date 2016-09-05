//======================================================================
// 文件： FrequencyFilterDib.h
// 内容： 图像频域滤波增强-头文件
// 功能： （1）傅立叶变换函数；       
//        （2）理想低通滤波函数
//        （3）理想高通滤波函数
// 
//======================================================================

#pragma once

#include "Dib.h" 
#include "afx.h"

class CFrequencyFilterDib
{
public:
	// 构造函数，初始化数据成员
	CFrequencyFilterDib(CDib *pDib);

	// 析构函数	
	~CFrequencyFilterDib(void);


    //傅立叶变换函数
    void fourier(double * data, int height, int width, int isign);

	// 理想低通滤波函数
	void Perfect_Low_Filter(int u,int v);
	
	// 理想高通滤波函数
	void Perfect_High_Filter(int u,int v); 

private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};