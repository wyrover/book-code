#include "StdAfx.h"
#include "GeoOperator.h"
#include <algorithm>
using namespace std;
#include <cmath>

#include <iostream>
#include <ctime>

/******************************************************************************
*	作用:			平移变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		xx			平移变换的水平偏移量
*		yy			平移变换的垂直偏移量
*	返回值:			是否成功
******************************************************************************/
bool MoveTransNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					 int xx, int yy)
{
	// 输出图像大小
	int size = width * height * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	// 偏移量是否在合法范围内
	if (xx >= width || yy >= height || xx <= -width || yy <= -height)
		return false;

	int		y = 0;							// 变换后的像素纵坐标
	BYTE*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;				// 指向被复制像素的起始位置
	int		copyWidth = width - abs(xx);	// 需要拷贝图像的宽度

	for (int j = 0; j < height; j++)
	{
		// 指向原始图像第j行的起始位置
		copyPixel = pixel + j * width * 4;

		// 修正被复制像素的起始位置
		if (xx < 0)
			copyPixel += abs(xx) * 4;

		// 计算变换后的像素纵坐标
		y = j + yy;

		if (y >= 0 && y < height)
		{
			objPixel = tempPixel + y * width * 4;

			if (xx > 0)
				objPixel += abs(xx) * 4;

			// 拷贝一行图像
			memcpy(objPixel, copyPixel, copyWidth * 4);
		}
	}

	return true;
}

/******************************************************************************
*	作用:			平移变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		xx			平移变换的水平偏移量
*		yy			平移变换的垂直偏移量
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
******************************************************************************/
void MoveTransSize(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				   int xx, int yy, UINT& outWidth, UINT& outHeight)
{
	outWidth = width + abs(xx);
	outHeight = height + abs(yy);

	int size = outWidth * outHeight * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	int		x = 0;							// 每行复制起点的横坐标
	int		y = 0;							// 每行复制起点的纵坐标
	BYTE*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;				// 指向被复制像素的起始位置

	if (xx > 0)
		x = xx;
	if (yy > 0)
		y = yy;

	for (int j = 0; j < height; j++)
	{
		copyPixel = pixel + j * width * 4;
		objPixel = tempPixel + y * outWidth * 4 + x * 4;
		y++;
		
		memcpy(objPixel, copyPixel, width * 4);
	}
}

/******************************************************************************
*	作用:			水平镜像变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
******************************************************************************/
void HMirrorTrans(BYTE* pixel, BYTE*& tempPixel, int width, int height)
{
	int size = width * height * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);


	// 方法一	通过映射关系进行变换
	int		x = 0;							// 变换后的像素横坐标
	int		y = 0;							// 变换后的像素纵坐标
	BYTE*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;				// 指向被复制像素的起始位置

	for (int j = 0; j < height; j++)
	{
		y = j;

		for (int i = 0; i < width; i++)
		{
			x = width - i - 1;

			copyPixel = pixel + j * width * 4 + i * 4;
			objPixel = tempPixel + y * width * 4 + x * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}

/*	// 方法二	通过交换像素进行变换
	memcpy(tempPixel, pixel, size);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width / 2; j++) 
		{
			swap(tempPixel[j*4 + i*width*4],	 tempPixel[(i+1)*width*4 - j*4 - 4]);
			swap(tempPixel[j*4 + i*width*4 + 1], tempPixel[(i+1)*width*4 - j*4 - 3]);
			swap(tempPixel[j*4 + i*width*4 + 2], tempPixel[(i+1)*width*4 - j*4 - 2]);
			swap(tempPixel[j*4 + i*width*4 + 3], tempPixel[(i+1)*width*4 - j*4 - 1]);
		}
	}*/
}

/******************************************************************************
*	作用:			垂直镜像变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
******************************************************************************/
void VMirrorTrans(BYTE* pixel, BYTE*& tempPixel, int width, int height)
{
	int size = width * height * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	// 方法一	通过映射关系进行变换
	int		x = 0;							// 变换后的像素横坐标
	int		y = 0;							// 变换后的像素纵坐标
	BYTE*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;				// 指向被复制像素的起始位置

	for (int j = 0; j < height; j++)
	{
		y = height - j - 1;

		for (int i = 0; i < width; i++)
		{
			x = i;

			copyPixel = pixel + j * width * 4 + i * 4;
			objPixel = tempPixel + y * width * 4 + x * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}

/*	// 方法二	通过交换像素进行变换
	// 拷贝原始图像
	memcpy(tempPixel, pixel, size);

	for(int i = 0; i < width * 4; i+=4) 
	{
		for (int j = 0; j < height / 2; j++) 
		{
			swap(tempPixel[j*width*4 + i],	   tempPixel[(height-j-1)*width*4 + i]);
			swap(tempPixel[j*width*4 + i + 1], tempPixel[(height-j-1)*width*4 + i + 1]);
			swap(tempPixel[j*width*4 + i + 2], tempPixel[(height-j-1)*width*4 + i + 2]);
			swap(tempPixel[j*width*4 + i + 3], tempPixel[(height-j-1)*width*4 + i + 3]);
		}
	}*/
}


/******************************************************************************
*	作用:			图像的转置函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
******************************************************************************/
void Transpose(BYTE* pixel, BYTE*& tempPixel, int width, int height, UINT& outWidth, UINT& outHeight)
{
	// 转置后图像的宽度和高度
	outWidth = (UINT)height;
	outHeight = (UINT)width;

	int size = outWidth * outHeight * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 0, size);

	BYTE*	copyPixel = NULL;	// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;	// 指向被复制像素的起始位置
	int		x = 0;				// 变换后的像素横坐标
	int		y = 0;				// 变换后的像素纵坐标
	int		tempJ = 0;			// 存储中间值,提高函数速度

	for (UINT j = 0; j < outHeight; j++)
	{
		x = j;

		// 可以提前计算与i无关的中间值
		// 可以大大减少重复计算
		tempJ = j * outWidth * 4;
		for (UINT i = 0; i < outWidth; i++) 
		{
			y = i;
			copyPixel = pixel + y * width * 4 + x * 4;
			objPixel = tempPixel + tempJ + i * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}
}

/******************************************************************************
*	作用:			图像的缩放函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				UINT& outWidth, UINT& outHeight, double fx, double fy)
{
	// 计算缩放后的图像大小
	outWidth = (UINT)(width * fx);
	outHeight = (UINT)(height * fy);
	
	int size = outWidth * outHeight * 4;
	
	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	BYTE*	copyPixel = NULL;	// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;	// 指向被复制像素的起始位置
	int		x = 0;				// 变换后的像素横坐标
	int		y = 0;				// 变换后的像素纵坐标
	long	tempY;				// 存储中间值,提高函数速度
	long	tempJ;				// 存储中间值,提高函数速度

	for (UINT j = 0; j < outHeight; j++)
	{
		// 获得临近像素的纵坐标
		y = (int)(j / fy + 0.5);

		// 修正坐标
		if (y >= height)
			y--;

		// 计算与i,x无关的中间值
		tempY = y * width * 4;
		tempJ = j * outWidth * 4;

		for (UINT i = 0; i < outWidth; i++) 
		{
			// 获得临近像素的横坐标
			x = (int)(i / fx + 0.5);

			// 修正坐标
			if (x >= width)
				x--;
			copyPixel = pixel + tempY + x * 4;
			objPixel = tempPixel + tempJ + i * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}
}


/******************************************************************************
*	作用:			图像的缩放函数(双线性插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					   UINT& outWidth, UINT& outHeight, double fx, double fy)
{
	// 计算缩放后的图像大小
	outWidth = (UINT)(width * fx + 0.5);
	outHeight = (UINT)(height * fy + 0.5);

	int size = outWidth * outHeight * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	BYTE*	copyPixel = NULL;	// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;	// 指向被复制像素的起始位置
	double	x = 0.0;			// 变换后的像素横坐标
	double	y = 0.0;			// 变换后的像素纵坐标
	long	tempJ = 0;			// 存储中间值,提高函数速度

	for (UINT j = 0; j < outHeight; j++)
	{
		y = j / fy;
		tempJ = j * outWidth * 4;

		for (UINT i = 0; i < outWidth; i++) 
		{
			x = i / fx;
			objPixel = tempPixel + tempJ + i * 4;

			// 距离当前点最近的四个点坐标
			int	x1, x2, y1, y2;

			x1 = (int)x;
			x2 = x1 + 1;
			y1 = (int)y;
			y2 = y1 + 1;

			// 对应四个坐标像素的取值
			BYTE clr1, clr2, clr3, clr4;
			double u, v;

			u = x - x1;
			v = y - y1;

			// 对于32位真彩色需要分别计算三原色
			for (int k = 0; k < 3; k++)
			{
				// 如果坐标在图像的右下角
				if ( (x >= width - 1) && (y >= height - 1) )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					objPixel[k] = clr1; 
				}
				// 如果图像在最后一列外侧
				else if ( x >= width - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					objPixel[k] = (BYTE)(clr1 * (1 - v) + clr3 * v); 
				}			
				// 如果图像在最后一行外侧
				else if ( y >= height - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					objPixel[k] = (BYTE)(clr1 * (1 - u) + clr2 * u);
				}
				else
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					clr4 = pixel[(y2 * width + x2) * 4 + k];	// (x2, y2)

					double f1, f2;

					f1 = clr1 * (1 - u) + clr2 * u;
					f2 = clr3 * (1 - u) + clr4 * u;
					objPixel[k] = (BYTE)(f1 * (1 - v) + f2 * v);
				}
			}
		}
	}
}

/******************************************************************************
*	作用:			图像的旋转函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		angle		旋转度数,以弧度表示
******************************************************************************/
void RotateNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				  UINT& outWidth, UINT& outHeight, double angle)
{
	// 原始图像四个角的坐标
	int srcX1, srcX2, srcX3, srcX4;
	int srcY1, srcY2, srcY3, srcY4;

	srcX1 = 0;
	srcY1 = 0;
	srcX2 = width - 1;
	srcY2 = 0;
	srcX3 = 0;
	srcY3 = height - 1;
	srcX4 = width - 1;
	srcY4 = height - 1;

	// 计算旋转角度的正弦和余弦值
	double fSin = sin(angle);
	double fCos = cos(angle);

	// 图像经过旋转后四个角的坐标
	double tranX1, tranX2, tranX3, tranX4;
	double tranY1, tranY2, tranY3, tranY4;

	tranX1 = fCos * srcX1 + fSin * srcY1;
	tranY1 = -fSin * srcX1 + fCos * srcY1;
	tranX2 = fCos * srcX2 + fSin * srcY2;
	tranY2 = -fSin * srcX2 + fCos * srcY2;
	tranX3 = fCos * srcX3 + fSin * srcY3;
	tranY3 = -fSin * srcX3 + fCos * srcY3;
	tranX4 = fCos * srcX4 + fSin * srcY4;
	tranY4 = -fSin * srcX4 + fCos * srcY3;

	// 计算旋转后图像的大小
	outWidth = (UINT)( max( fabs(tranX4-tranX1), fabs(tranX3-tranX2) ) + 1.5 );
	outHeight = (UINT)( max( fabs(tranY4-tranY1), fabs(tranY3-tranY2) ) + 1.5 );

	tempPixel = new BYTE[outWidth * outHeight * 4];
	memset(tempPixel, 255, outWidth * outHeight * 4);

	// 计算两个常量
	double num1 = -0.5*outWidth*fCos - 0.5*outHeight*fSin + 0.5*width;
	double num2 = 0.5*outWidth*fSin - 0.5*outHeight*fCos + 0.5*height;

	BYTE*	copyPixel = NULL;	// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;	// 指向被复制像素的起始位置
	int		x = 0;				// 变换后的像素横坐标
	int		y = 0;				// 变换后的像素纵坐标

	for (long j = 0; j < (long)outHeight; j++)
	{
		for (long i = 0; i < (long)outWidth; i++) 
		{
			x = (int)(i * fCos + j * fSin + num1 + 0.5);
			y = (int)(-i * fSin + j * fCos + num2 + 0.5);

			// 修正坐标
			if (x == width)
				x--;
			if (y == height)
				y--;

			copyPixel = pixel + y * width * 4 + x * 4;
			objPixel = tempPixel + j * outWidth * 4 + i * 4;

			if (x >= 0 && x < width && y >= 0 && y < height)
				memcpy(objPixel, copyPixel, 4);
		}
	}
}

/******************************************************************************
*	作用:			图像的旋转函数(双线性插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		angle		旋转度数,以弧度表示
******************************************************************************/
void RotateInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
						 UINT& outWidth, UINT& outHeight, double angle)
{
	// 原始图像四个角的坐标
	int srcX1, srcX2, srcX3, srcX4;
	int srcY1, srcY2, srcY3, srcY4;

	srcX1 = 0;
	srcY1 = 0;
	srcX2 = width - 1;
	srcY2 = 0;
	srcX3 = 0;
	srcY3 = height - 1;
	srcX4 = width - 1;
	srcY4 = height - 1;

	// 计算旋转角度的正弦和余弦值
	double fSin = sin(angle);
	double fCos = cos(angle);

	// 图像经过旋转后四个角的坐标
	double tranX1, tranX2, tranX3, tranX4;
	double tranY1, tranY2, tranY3, tranY4;

	tranX1 = fCos * srcX1 + fSin * srcY1;
	tranY1 = -fSin * srcX1 + fCos * srcY1;
	tranX2 = fCos * srcX2 + fSin * srcY2;
	tranY2 = -fSin * srcX2 + fCos * srcY2;
	tranX3 = fCos * srcX3 + fSin * srcY3;
	tranY3 = -fSin * srcX3 + fCos * srcY3;
	tranX4 = fCos * srcX4 + fSin * srcY4;
	tranY4 = -fSin * srcX4 + fCos * srcY3;

	// 计算旋转后图像的大小
	outWidth = (UINT)( max( fabs(tranX4-tranX1), fabs(tranX3-tranX2) ) + 1.5 );
	outHeight = (UINT)( max( fabs(tranY4-tranY1), fabs(tranY3-tranY2) ) + 1.5 );

	tempPixel = new BYTE[outWidth * outHeight * 4];
	memset(tempPixel, 255, outWidth * outHeight * 4);

	//计算矩阵(2.9)中的两个常数，这样不用以后每次都计算了

	double num1 = -0.5*outWidth*fCos - 0.5*outHeight*fSin + 0.5*width;
	double num2 = 0.5*outWidth*fSin - 0.5*outHeight*fCos + 0.5*height;

	BYTE*	copyPixel = NULL;	// 指向原始图像中需要拷贝像素的起始位置
	BYTE*	objPixel = NULL;	// 指向被复制像素的起始位置
	double	x = 0.0;			// 变换后的像素横坐标
	double	y = 0.0;			// 变换后的像素纵坐标
	long	tempJ = 0;			// 存储中间值,提高函数速度

	for (long j = 0; j < (long)outHeight; j++)
	{
		tempJ = j * outWidth * 4;

		for (long i = 0; i < (long)outWidth; i++) 
		{
			x = i * fCos + j * fSin + num1;
			y = -i * fSin + j * fCos + num2;

			// 超界则计算下一个像素
			if (x > width || x < 0 || y > height || y < 0)
				continue;

			objPixel = tempPixel + tempJ + i * 4;

			// 距离当前点最近的四个点坐标
			int	x1, x2, y1, y2;

			x1 = (int)x;
			x2 = x1 + 1;
			y1 = (int)y;
			y2 = y1 + 1;

			// 对应四个坐标像素的取值
			BYTE clr1, clr2, clr3, clr4;
			double u, v;

			u = x - x1;
			v = y - y1;

			// 对于32位真彩色需要分别计算三原色
			for (int k = 0; k < 3; k++)
			{
				// 如果坐标在图像的右下角
				if ( (x >= width - 1) && (y >= height - 1) )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					objPixel[k] = clr1; 
				}
				// 如果图像在最后一列外侧
				else if ( x >= width - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					objPixel[k] = (BYTE)(clr1 * (1 - v) + clr3 * v); 
				}			
				// 如果图像在最后一行外侧
				else if ( y >= height - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					objPixel[k] = (BYTE)(clr1 * (1 - u) + clr2 * u);
				}
				else
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					clr4 = pixel[(y2 * width + x2) * 4 + k];	// (x2, y2)

					double f1, f2;

					f1 = clr1 * (1 - u) + clr2 * u;
					f2 = clr3 * (1 - u) + clr4 * u;
					objPixel[k] = (BYTE)(f1 * (1 - v) + f2 * v);
				}
			}
		}
	}
}