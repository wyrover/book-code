#include "StdAfx.h"
#include "GrayOperator.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;


/******************************************************************************
*	作用:			线性变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		slope		线性函数的斜率
*		inter		线性函数的截距
*	备注:			此函数对于彩色图同样适用
******************************************************************************/
void LineTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double slope, double inter)
{
	// 灰度映射表
	BYTE map[256];

	// 保存运算后的临时值
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// 计算当前像素变换后的值
		dTemp = slope * i + inter;

		// 如果超界则修改其值
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// 四舍五入
		map[i] = int(dTemp + 0.5);
	}

	// 线性变换后的值直接在映射表中查找
	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}

/******************************************************************************
*	作用:			对数变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		a			控制参数,表示曲线的上下偏移量
*		b			控制参数,表示曲线的弯曲程度
*	备注:			此函数对于彩色图同样适用
******************************************************************************/
void LogTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double a, double b)
{
	// 映射表,用于256种灰度变换后的值
	BYTE map[256];

	// 保存运算后的临时值
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// 计算当前像素变换后的值
		dTemp = log((double)i + 1.0) / b + a;

		// 如果超界则修改其值
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// 四舍五入
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}

/******************************************************************************
*	作用:			幂次变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		b			控制参数,表示曲线的上下偏移量
*		c			控制参数,表示曲线的弯曲程度
*		r			控制参数,表示函数的幂次
*	备注:			此函数对于彩色图同样适用
******************************************************************************/
void PowerTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			    double b, double c, double r)
{
	// 映射表,用于256种灰度变换后的值
	BYTE map[256];

	// 保存运算后的临时值
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// 计算当前像素变换后的值
		dTemp = c * pow(i / 255.0, r) * 255 + b;

		// 如果超界则修改其值
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// 四舍五入
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}

/******************************************************************************
*	作用:			指数变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		a			控制参数,表示曲线的左右偏移量
*		b,c			控制参数,表示曲线的弯曲程度
*	备注:			此函数对于彩色图同样适用
******************************************************************************/
void ExpTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
				double a, double b, double c)
{
	// 映射表,用于256种灰度变换后的值
	BYTE map[256];

	// 保存运算后的临时值
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// 计算当前像素变换后的值
		dTemp = pow(b, c * (i - a)) - 1;

		// 如果超界则修改其值
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// 四舍五入
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}


/******************************************************************************
*	作用:			灰度阈值变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		thr			阈值
******************************************************************************/
void ThresholdTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, int thr)
{
	// 灰度映射表
	BYTE map[256];

	for (int i = 0; i < 256; i++)
	{
		if (i >= thr)
			map[i] = 255;
		else
			map[i] = 0;
	}

	// 变换后的值直接在映射表中查找
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i*4] = tempPixel[i*4+1] = tempPixel[i*4+2] = map[x];
		tempPixel[i*4+3] = 255;
	}
}

/******************************************************************************
*	作用:			灰度拉伸函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		X1,Y1		控制点1的坐标
*		X2,Y2		控制点2的坐标
******************************************************************************/
void GrayStretch(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
				 float X1, float Y1, float X2, float Y2)
{
	// 映射表,用于256种灰度变换后的值
	BYTE map[256];

	// 保存运算后的临时值
	double dTemp;

	for (int x = 0; x < 256; x++)
	{
		// 如果在第一条直线上
		if ( x < X1)
		{
			if ( fabs(X1 - 0) > 0.0001 )
				dTemp = (float)Y1 / (float)X1 * x;
			else
				dTemp = 0;
		}
		// 如果在第二条直线上
		else if ( x <= X2 )
		{
			if ( fabs(X2 - X1) > 0.0001 )
				dTemp = (float)(Y2 - Y1) / (float)(X2 - X1) * (x - X1) + Y1;
			else
				dTemp = x;
		}
		// 如果在第三条直线上
		else
		{
			if ( fabs(Y2 - 255) > 0.0001 )
				dTemp = (float)(255 - Y2) / (float)(255 - X2) * (x - X2) + Y2;
			else
				dTemp = 255;
		}

		// 四舍五入
		map[x] = int(dTemp + 0.5);
	}

	// 变换后的值直接在映射表中查找
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i*4] = tempPixel[i*4+1] = tempPixel[i*4+2] = map[x];
		tempPixel[i*4+3] = 255;
	}
}

/******************************************************************************
*	作用:			灰度均衡函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
******************************************************************************/
void GrayEqualize(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height)
{

	// 灰度映射表
	BYTE map[256];
	long lCounts[256];

	memset(lCounts, 0, sizeof(long) * 256);

	// 计算各灰度值个数
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];
		lCounts[x]++;
	}

	// 保存运算中的临时值
	long lTemp;

	for (int i = 0; i < 256; i++)
	{
		lTemp = 0;
		for (int j = 0; j <= i; j++)
			lTemp += lCounts[j];

		map[i] = (BYTE)(lTemp * 255.0f / width / height);
	}

	// 变换后的值直接在映射表中查找
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i*4] = tempPixel[i*4+1] = tempPixel[i*4+2] = map[x];
		tempPixel[i*4+3] = 255;
	}
}