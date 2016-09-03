// ************************************************************************
//  文件名：ImageView.cpp
//
//  图像复原API函数库：
//
//  LimbPatternBayer()				- 用BAYER表抖动显示图象
//  DitherFloydSteinberg()			- 用Floyd－Steinberg算法抖动生成图象
//
// *************************************************************************


#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

/*************************************************************************
 *
 * 函数名称：
 *   LimbPatternBayer()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数利用BAYER表抖动显示图象。
 *
 ************************************************************************/


BOOL LimbPatternBayer(CDib *pDib)
{
	// Bayer表的定义
	BYTE BayerPattern[8][8]={ 0, 32,  8, 40,  2, 34, 10, 42,
							 48, 16, 56, 24, 50, 18, 58, 26,
							 12, 44,  4, 36, 14, 46,  6, 38,
							 60, 28, 52, 20, 62, 30, 54, 22,
							  3, 35, 11, 43,  1, 33,  9, 41,
							 51, 19, 59, 27, 49, 17, 57, 25,
							 15, 47,  7, 39, 13, 45,  5, 37,
							 63, 31, 55, 23, 61, 29, 53, 21};

	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;;

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 循环变量
	int i, j;

	// 象素的值
	int nPixelValue;

	// 将图象二值化，利用BAYER表抖动显示图象
	for (j = 0; j < lHeight ; j++)
	{
		for(i = 0; i < lLineBytes ; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			
			nPixelValue = (*lpSrc);

			nPixelValue =nPixelValue;
			
			// 右移两位后做比较
			if ( (nPixelValue>>2) > BayerPattern[j&7][i&7]) 
                //打白点    
				*(lpSrc)=(unsigned char)255; 
						
			else 
				//打黑点
				*(lpSrc)=(unsigned char)0; 
		}		
	}

	return true;
}


/*************************************************************************
 *
 * 函数名称：
 *   DitherFloydSteinberg()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来用Floyd－Steinberg算法抖动生成图象。
 *
 ************************************************************************/
BOOL DitherFloydSteinberg(CDib *pDib)
{	
	// 指向源图像的指针
	BYTE *	lpSrc;
	
	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();
		
	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	// 循环变量
	int i, j;

	// 误差传播系数
	double temp, error;
	
	// 象素值
	int nPixelValue;
	
	// 将图象二值化，并用Floyd－Steinberg算法抖动生成图象
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			
			nPixelValue = *lpSrc;
			
			//128是中值
			if ( nPixelValue > 128 )
			{ 				
				//打白点
				*lpSrc=255; 
				
				//计算误差
				error = (double)(nPixelValue-255.0); 							
			}
			
			else
			{
				//打黑点
				*lpSrc=0; 
				
				//计算误差
				error = (double)nPixelValue; 
			}
			
			// 如果不是边界
			if(i < lLineBytes-1)
			{ 
				//向右传播
				temp = (float)*(lpSrc+1);
				
				temp = temp + error * (1.5/8.0);
				
				if(temp > 255.0)
					temp = 255.0;

				*(lpSrc+1)=(int)temp; 
				
			}
			
			// 如果不是边界			
			if(j < lHeight - 1)
			{ 				
				// 向下传播
				temp = (float)*(lpSrc + lLineBytes);
				
				temp = temp + error * (1.5/8.0);				
				
				*(lpSrc+lLineBytes) = (int)temp;
				
				if(i < lLineBytes-1)
				{
					// 向右下传播
					temp = (float)*(lpSrc + lLineBytes + 1);
					
					temp = temp + error * (2.0/16.0);
				
					*(lpSrc + lLineBytes + 1) = (int)temp;				
				}
			}
			
		}
		
	}
	
	return true;	
}
