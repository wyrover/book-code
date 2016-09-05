#include "StdAfx.h"
#include "ColorSeg.h"
#include "math.h"

CColorSeg::CColorSeg(CDib *pDib)
{
    m_pDib = pDib;
}

CColorSeg::~CColorSeg(void)
{
}

//=======================================================
// 函数功能： 彩色模板
// 输入参数： LPBYTE lpData: 指向原图像数据区指针 
//            int nLineByte: 原图像每行像素所占的字节数                                   
//            int nHeight: 原图像高度                                  
//            int nTemH: 模板高度                                     
//            int nTemW: 模板宽度                                     
//            int nTemCX: 模板中心元素的X坐标(<nTemW-1)               
//            int nTemCY: 模板的中心元素的Y坐标(<nTemH-1)               
//            float *fpArray: 指向模板数组的指针                      
//            float fCoef：模板系数         
// 返回值：   void 
//=======================================================
void CColorSeg::Template24bit(LPBYTE lpData, int nLineByte, int nWidth, int nHeight, int nTemH, int nTemW, int nTemCX, int nTemCY, float *fpArray, float fCoef)
{   
	// 循环变量
    int i, j, k, l; 

    // 新图像缓冲区的指针
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，设定初始值为255
	memcpy(lpTemp, lpData, nLineByte * nHeight);

    // 像素值计算结果
    float fResult; 

	for(j = nTemCY; j < nHeight - nTemH + nTemCY + 1; j ++)
		for(i = 3 * nTemCX; i < nLineByte - 3*nTemW + 3*nTemCX + 1; i ++)
		{
			// 计算当前像素值
			fResult = 0;

			for(k = 0; k < nTemH; k ++)
				for(l = 0; l < nTemW; l ++)
                    // 卷积运算
					fResult += lpData[(j - nTemCY + k) * nLineByte + (i - 3 * nTemCX + 3 * l)]
                             * fpArray[k * nTemW + l];
			
            //用系数乘以当前像素值计算结果
			fResult *= fCoef;

			//取绝对值
			fResult = (float) fabs(fResult);

			// 判断是否超过255
			if(fResult > 255)
				
                // 若超过255，在新图像当前像素位置直接赋值为255
                lpTemp[j * nLineByte + i] = 255;
			else
				
                // 未超过255，在新图像当前像素位置赋值为计算结果
				lpTemp[j * nLineByte + i] = (int) (fResult + 0.5);
		}

    // 将处理后的新图像复制到原图像中
	memcpy(lpData,lpTemp, nLineByte * nHeight);

	// 释放内存
	delete [] lpTemp;
}

//=======================================================
// 函数功能： Prewitt边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CColorSeg::Prewitt(void)
{
    // 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

    // 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
       
    // 模板宽度和高度
    int nTemW, nTemH; 

    // 模板系数
	float fCoef; 

    //模板中心元素的X坐标和Y坐标
	int nTemCX, nTemCY;  

	//模板数组
    float arTemplate[9]; 

    // 两幅新图像缓冲区的指针
	LPBYTE lpTemp1, lpTemp2; 
    lpTemp1 = new BYTE[nLineByte * nHeight];
    lpTemp2 = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，复制原图像
	memcpy(lpTemp1, lpData, nLineByte * nHeight);
    memcpy(lpTemp2, lpData, nLineByte * nHeight);
  
	// 设置Prewitt模板一的参数
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -1.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 0.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 0.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 1.0;
	arTemplate[8] = 1.0;

	// 调用Template()函数
	Template24bit(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // 设置Prewitt模板二的参数
    arTemplate[0] = 1.0;
    arTemplate[1] = 0.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 1.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -1.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 0.0;
	arTemplate[8] = -1.0;

	// 调用Template()函数
	Template24bit(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nLineByte; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将第一个缓存图像复制到原图像中
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// 删除缓冲区
	delete  [] lpTemp1;
	delete  [] lpTemp2;	
}
