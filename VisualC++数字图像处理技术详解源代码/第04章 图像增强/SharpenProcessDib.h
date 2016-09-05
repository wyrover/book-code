//======================================================================
// 文件： SharpenProcessDib.h
// 内容： 图像锐化处理函数-头文件
// 功能： （1）门限梯度锐化函数；       
//        （2）卷积运算函数；
//        （3）拉普拉斯掩膜锐化函数        
// 
//======================================================================

#pragma once

#include "Dib.h" 
#include "afx.h"

class CSharpenProcessDib
{
public:
	// 构造函数，初始化数据成员
	CSharpenProcessDib(CDib *pDib);

	// 析构函数	
	~CSharpenProcessDib(void);

	// 门限梯度锐化函数
	BOOL GateGrad(BYTE t);
	
	//卷积运算函数
    BOOL Convolution(int tempH, int tempW, int tempMX, int tempMY, float *fpTempArray, float fCoef); 

	// 拉普拉斯眼膜锐化函数
	void Laplacian( ); 

private:
    // 数据成员,CDib对象的指针 
	CDib *m_pDib; 

};
