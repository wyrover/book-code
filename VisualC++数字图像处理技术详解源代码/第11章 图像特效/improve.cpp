#include "StdAfx.h"
#include "improve.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#pragma warning ( disable : 4018)

int GetAsh(BYTE** imageBuf, int x, int y)
{
	int clr;
	clr = (imageBuf[y][x*4] + imageBuf[y][x*4+1]
			+imageBuf[y][x*4+2]) / 3;
	return clr;
}

/********************************************************
* 把线形存储的像素转化为二维数组形式
* 参数: image 线形存储的像素， width,height 图象的长宽
********************************************************/
BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height, int bt=4)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*)*(height));
	for(int y=0; y<height; y++)
	{
		//使imageBuf中每个指针分别指向其下标表示的行的行首地址
		imageBuf[y] = image+y*width*bt; 
	}
	return imageBuf;
}

/**************************************************
* 功能: 设定指定位置的像素灰度
* 参数: imageBuf为目标图像 x,y为要设定像素的坐标
**************************************************/
void SetPixelXY(BYTE** imageBuf1, int x, int y, int a)
{
	imageBuf1[y][x*4] = a;
	imageBuf1[y][x*4+1] = a;
	imageBuf1[y][x*4+2] = a;
	imageBuf1[y][x*4+3]= 255;
}

/**************************************************
* 功能: 使用模板对彩色图邻域进行运算
* 参数: imageBuf为目标图像 w、h为图像大小
*       templt为模板 tw为邻域大小 
*		x,y为要取得像素的坐标
*       cn为颜色分量编号 0为蓝色 1为绿色 2为红色
**************************************************/
int TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)
{
	int i,j;                        //循环变量
	int m=0;                      //用来存放加权和
	int px,py;   
	//依次对邻域中每个像素进行运算
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			//计算对应模板上位置的像素在原图像中的位置
			py=y-tw/2+i;
			px=x-tw/2+j;
			//加权求和
			m+=imageBuf0[py][px*4+cn] * templt[i*tw+j];
		}
	}
	return m;                     //返回结果
}

/*****************************************************************
* 功能: 使用模板对灰度图邻域进行运算 
* 参数: imageBuf为目标图像 w、h为图像大小
*       templt为模板 tw为邻域大小 
*		x,y为要取得像素的坐标
******************************************************************/
int TempltExcuteAsh(BYTE** imageBuf0, int w, int h, 
					int* templt, int tw, int x, int y)
{
	int i,j;                        //循环变量
	int m=0;                      //用来存放加权和
	int px,py;   
	//依次对邻域中每个像素进行运算
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			//计算对应模板上位置的像素在原图像中的位置
			py=y-tw/2+i;
			px=x-tw/2+j;
			//加权求和
			m+=GetAsh(imageBuf0,px,py) * templt[i*tw+j];
		}
	}
	return m;                     //返回结果
}

/******************************************************************
* 功能: 灰度图像的简单平滑处理
* 参数: image0为原图形，image1为平滑结果，
*		w、h为图象的宽和高
*       size为进行平滑的邻域边长
******************************************************************/
void SmoothAsh(BYTE* image0, BYTE* image1, 
			   unsigned int w, unsigned int h, unsigned int size)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//定义模板
	int* templt;
	int x,y;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];
	for(x=0; x<size*size; x++)
	{
		templt[x]=1;
	}
	//设定衰减因子
	scale = size*size;

	//依次对原图像的每个像素进行处理
	for(y=size/2; y<h-size/2; y++)
	{
		for(x=size/2; x<w-size/2; x++)
		{
			a=TempltExcuteAsh(imageBuf0,w,h,templt,size,x,y);
			a/= scale;
			//过限处理
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}
	}

	//清理内存
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* 功能: 彩色图像的简单平滑处理
* 参数: image0为原图形，image1平滑结果，
*		w、h为图象的宽和高
*       size为进行平滑的邻域边长
******************************************************************/
void SmoothCl(BYTE* image0, BYTE* image1, 
			  unsigned int w, unsigned int h, unsigned int size)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//定义模板
	int* templt;
	int x,y,c;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];
	for(x=0; x<size*size; x++)
	{
		templt[x]=1;
	}
	//设定衰减因子
	scale = size*size;

	//依次对原图像的每个像素进行处理
	for(y=size/2; y<h-size/2; y++)
	{
		for(x=size/2; x<w-size/2; x++)
		{
			//依次对彩色图像的三个分量灰度进行处理
			for(c=0; c<3; c++)
			{
				a=TempltExcuteCl(imageBuf0,w,h,templt,size,x,y,c);
				a/= scale;
				//过限处理
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}
		}
	}

	//清理内存
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* 功能: 彩色图像的高斯平滑处理
* 参数: image0为原图形，image1平滑结果，
*		w、h为图像的宽和高
******************************************************************/
void SmoothGaussCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int templt[9]={1,2,1,2,4,2,1,2,1};
	int x,y,c;
	int a;
	int scale;

	//设定衰减因子
	scale = 16;

	//依次对原图像的每个像素进行处理
	for(y=1; y<h-1; y++)
		for(x=1; x<w-1; x++)
			for(c=0; c<3; c++)
			{
				//利用高斯模板对邻域进行处理
				a=TempltExcuteCl(imageBuf0,w,h,templt,3,x,y,c);
				a/= scale;
				//过限处理
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}

	//清理内存
	free(imageBuf0);
	free(imageBuf1);
} 

/**************************************************
* 功能: 使用模板对灰度图邻域排序取中值
* 参数: imageBuf为目标图像 w、h为图像大小
*       templt为模板 tw为邻域大小 
*		x,y为当前采样窗口中心像素的坐标
**************************************************/
int MedianValueAsh(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y)
{
	int i,j,k;                   
	int px,py,c;
	int* value;                //用来保存要排序的数值
	int count;                 //用来保存采样窗口的像素数量
	value= new int[tw*tw]; 
	k=0;
	//从采样窗口中取得像素灰度
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			//如果该像素位于采样窗口中
			if(templt[i*tw+j]>0)
			{
				//保存像素灰度
				value[k]=GetAsh(imageBuf0,px,py);
				k++;
			}
		}
	}
	//记录保存的像素个数
	count=k;
	//对保存的像素灰度数据进行排序
	for(i=0; i<count-1; i++)
	{
		k=i;
		for(j=i+1; j<count; j++)
			if(value[j]<value[k]) k=j;
		c=value[i];
		value[i]=value[k];
		value[k]=c;
	}
	//保存中值
	c=value[count/2];
	//清理内存
	delete[] value;
	return c;
}

/******************************************************************
* 功能: 灰度图像的中值滤波平滑处理
* 参数: image0为原图形，image1平滑结果，
*		w、h为图象的宽和高
*       size为进行平滑的邻域边长
******************************************************************/
void SmoothMedianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int* templt;
	int x,y;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];
	for(x=0; x<size*size; x++) templt[x]=1;

	//设定衰减因子
	scale = 1;

	//依次对原图像的每个像素进行处理
	for(y=size/2; y<h-size/2; y++)
		for(x=size/2; x<w-size/2; x++)
		{
			//取采样窗口中像素灰度的中值
			a=MedianValueAsh(imageBuf0,w,h,templt,size,x,y);
			a/= scale;
			//过限处理
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//清理内存
		delete[] templt;
		free(imageBuf0);
		free(imageBuf1);
}

/**************************************************
* 功能: 使用模板对彩色图邻域排序取中值
* 参数: imageBuf为目标图像 w、h为图像大小
*       templt为模板 tw为邻域大小 
*		x,y为当前采样窗口中心像素的坐标
*       cn为颜色分量编号 0为蓝色 1为绿色 2为红色
**************************************************/
int MedianValueCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)
{
	int i,j,k;                   
	int px,py,c;
	int* value;                //用来保存要排序的数值
	int count;                 //用来保存采样窗口的像素数量
	value = new int[tw*tw]; 
	k=0;
	//从采样窗口中取得像素灰度
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			//如果该像素位于采样窗口中
			if(templt[i*tw+j]>0)
			{
				//保存像素灰度
				value[k]=imageBuf0[py][px*4+cn];
				k++;
			}
		}
	}

	count = k;

	vector<int> nums(tw*tw);
	
	for (int i = 0; i < tw*tw; i++)
		nums[i] = value[i];

	//对保存的像素灰度数据进行排序
	sort(nums.begin(), nums.end());

	//保存中值
	c=nums[count/2];
	//清理内存
	delete[] value;
	return c;
}

/******************************************************************
* 功能: 彩色图像的中值滤波平滑处理
* 参数: image0为原图形，image1平滑结果，
*		w、h为图象的宽和高
*       size为进行平滑的邻域边长
******************************************************************/
void SmoothMedianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int* templt;
	int x,y,c;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];

	for(x=0; x<size*size; x++)
	{
		templt[x]=1;
	}
	//设定衰减因子
	scale = 1;

	//依次对原图像的每个像素进行处理
	for(y=size/2; y<h-size/2; y++)
	{
		for(x=size/2; x<w-size/2; x++)
		{
			for(c=0; c<3; c++)
			{
				//取采样窗口中像素灰度的中值
				a=MedianValueCl(imageBuf0,w,h,templt,size,x,y,c);
				a/= scale;
				//过限处理
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}
		}
	}
	//清理内存
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}


/******************************************************************
* 功能: 灰度图像的拉普拉斯锐化处理（scale = 3）
* 参数: image0为原图形，image1锐化结果，
*		w、h为图象的宽和高
******************************************************************/
void SharpLaplacianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int templt[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	int x,y;
	int a;
	int scale;

	scale = 3;//设定衰减因子

	//依次对原图像的每个像素进行处理
	for(y=1; y<h-1; y++)
		for(x=1; x<w-1; x++)
		{
			//利用拉普拉斯模板对邻域进行处理
			a=TempltExcuteAsh(imageBuf0,w,h,templt,3,x,y);
			a/= scale;
			//对中心像素进行增强
			a=GetAsh(imageBuf0,x,y)+a;
			//过限处理
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//清理内存
		free(imageBuf0);
		free(imageBuf1);
}

/******************************************************************
* 功能: 彩色图像的拉普拉斯锐化处理（scale = 3）
* 参数: image0为原图形，image1锐化结果，
*		w、h为图象的宽和高
******************************************************************/
void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int templt[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	int x,y,c;
	int a;
	int scale;

	//设定衰减因子
	scale = 3;

	//依次对原图像的每个像素进行处理
	for(y=1; y<h-1; y++)
	{
		for(x=1; x<w-1; x++)
		{
			for(c=0; c<3; c++)
			{
				//利用拉普拉斯模板对邻域进行处理
				a=TempltExcuteCl(imageBuf0,w,h,templt,3,x,y,c);
				a/= scale;
				//对中心像素进行增强
				a=imageBuf0[y][x*4+c]+a;
				//过限处理
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}
		}
	}
	//清理内存
	free(imageBuf0);
	free(imageBuf1);
}

void SetPixel(BYTE* image1, unsigned int n, int a)
{
	image1[n] = a;
	image1[n+1] = a;
	image1[n+2] = a;
	image1[n+3] = 255;
}     


/***********************************************************************
* Sobel边缘检测  （scale=0.5）
* 参数: image0为原图形，image1为边缘检测结果，w、h为图像的宽和高
* 当type为true时，差分结果取水平和垂直方向差分中较大者，否则取平均值
************************************************************************/
void SideSobel(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, bool type)
{
	int x, y, a, aHr, aHg, aHb, aVr, aVg, aVb, aH, aV;
	long n;
	double scale = 0.5;
	//依次处理每个像素
	for(y = 1; y < h-1; y++)
		for(x = 1; x < w-1; x++)
		{
			//计算像素的偏移位置
			n = (y*w+x)*4;
			//计算红色分量水平灰度差
			aHr = abs( (image0[n-w*4-4]+image0[n-4]*2+image0[n+w*4-4])
				- (image0[n-w*4+4]+image0[n+4]*2+image0[n+w*4+4]) );
			//计算红色分量垂直灰度差
			aVr = abs( (image0[n-w*4-4]+image0[n-w*4]*2+image0[n-w*4+4])
				- (image0[n+w*4-4]+image0[n+w*4]*2+image0[n+w*4+4]) );
			//计算绿色分量水平灰度差
			aHg = abs( (image0[n-w*4-4+1]+image0[n-4+1]*2+image0[n+w*4-4+1])
				- (image0[n-w*4+4+1]+image0[n+4+1]*2+image0[n+w*4+4+1]) );
			//计算绿色分量垂直灰度差
			aVg = abs( (image0[n-w*4-4+1]+image0[n-w*4+1]*2+image0[n-w*4+4+1])
				- (image0[n+w*4-4+1]+image0[n+w*4+1]*2+image0[n+w*4+4+1]) );
			//计算蓝色分量水平灰度差
			aHb = abs( (image0[n-w*4-4+2]+image0[n-4+2]*2+image0[n+w*4-4+2])
				- (image0[n-w*4+4+2]+image0[n+4+2]*2+image0[n+w*4+4+2]) );
			//计算蓝色分量垂直灰度差
			aVb = abs( (image0[n-w*4-4+2]+image0[n-w*4+2]*2+image0[n-w*4+4+2])
				- (image0[n+w*4-4+2]+image0[n+w*4+2]*2+image0[n+w*4+4+2]) );

			//计算水平综合灰度差
			aH = aHr + aHg + aHb;
			//计算垂直综合灰度差
			aV = aVr + aVg + aVb;

			if(type)
			{
				//取水平和垂直方向差分中较大者
				if(aH > aV) a = aH;
				else a = aV;
			}
			else
			{
				//取水平和垂直方向差分的平均值
				a = (aH + aV)/2;
			}
			a = a *scale;
			a = a>255?255:a;
			//生成边缘扫描结果
			SetPixel(image1,n,a);
		}
}

/******************************************************************
* 功能: 对图像进行二值化处理
* 参数: image0为原图形，image1为处理的结果图像，
*		w、h为图象的宽和高
*       K为阈值
******************************************************************/
void ToTwoValue(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, int K)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	int x,y;

	//依次对原图像的每个像素进行处理
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			//如果当前点已经为单点 则在结果图中用黑色标记
			if( GetAsh(imageBuf0,x,y) >=K )
			{
				SetPixelXY (imageBuf1,x,y,255 );
			}
			//如果当前点不是单点 则在结果图中用白色标记
			else
			{
				SetPixelXY(imageBuf1,x,y,0);
			}
		}

	//清理内存
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* 功能: 对两副图像进行减法运算
* 参数: image0为原图形，image1为要减去的图像，
*		w、h为图象的宽和高
******************************************************************/
void Subtract(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	int x,y;
	int a;

	//依次对原图像的每个像素进行处理
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			//取得原图像灰度
			a=GetAsh(imageBuf0,x,y);
			//进行减法运算
			a=a-GetAsh(imageBuf1,x,y);
			//过限处理
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//清理内存
		free(imageBuf0);
		free(imageBuf1);
}

/******************************************************************
* 功能: 对两副图像进行加法运算
* 参数: image0为原图形，image1为要减去的图像，
*		w、h为图象的宽和高
******************************************************************/
void AshAdd(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	int x,y,c;
	int a;

	//依次对原图像的每个像素进行处理
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			//取得原图像灰度
			a=GetAsh(imageBuf0,x,y);
			//进行减法运算
			a=a+GetAsh(imageBuf1,x,y);
			//过限处理
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//清理内存
		free(imageBuf0);
		free(imageBuf1);
}