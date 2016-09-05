//======================================================================
// 文件： GrayTransformDib.cpp
// 内容： 图像灰度变换增强函数-源文件
// 功能： （1）线性灰度增强；
//        （2）分段线性灰度增强；
//        （3）对数函数非线性灰度增强；
// 
//======================================================================

#include "StdAfx.h"
#include "GrayTransformDib.h"
#include<math.h>


//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CGrayTransformDib::CGrayTransformDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CGrayTransformDib::~CGrayTransformDib(void)
{
	
}


//=======================================================
// 函数功能： 线性灰度增强
// 输入参数： BYTE gMin-变换后目标图像的最小灰度值
//            BYTE gMax-变换后目标图像的最大灰度值
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CGrayTransformDib::Linear_Transform( BYTE gMin, BYTE gMax)
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	BYTE pixel;	//像素值

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//逐个扫面图像中的像素点,进行灰度线性变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像倒数第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)*lpSrc;

			//根据公式(5-2)求出目标图像中与当前点对应的像素点的灰度值

			*lpDst = (BYTE)(((float)(gMax-gMin)/255)*pixel+gMin+0.5);

		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}



//=======================================================
// 函数功能： 分段线性灰度增强
// 输入参数： BYTE gSrc1, BYTE gSrc2-原图像灰度区间分段点
//            BYTE gDst1, BYTE gDst2-变换后的目标图像灰度区间分段点
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CGrayTransformDib::Seg_Linear_Transform( BYTE gSrc1, BYTE gSrc2,BYTE gDst1, BYTE gDst2)
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	BYTE pixel;	//像素值

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//逐个扫面图像中的像素点,进行灰度分段线性变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像倒数第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)*lpSrc;

			//根据公式(5-3)右边上式求出目标图像中与当前点对应的像素点的灰度值
			if(pixel<gSrc1)
			{
				*lpDst = (BYTE)(((float)gDst1/gSrc1)*pixel+0.5);
			}

			//根据公式(5-3)右边中式求出目标图像中与当前点对应的像素点的灰度值
			if((pixel>=gSrc1)&&(pixel<=gSrc2))
			{
				*lpDst = (BYTE)(((float)(gDst2-gDst1)/(gSrc2-gSrc1))*(pixel-gSrc1)+gDst1+0.5);
			}

			//根据公式(5-3)右边下式求出目标图像中与当前点对应的像素点的灰度值
			if((pixel>gSrc2)&&(pixel<=255))
			{
				*lpDst = (BYTE)(((float)(255-gDst2)/(255-gSrc2))*(pixel-gSrc2)+gDst2+0.5);
			}

		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;

}

//=======================================================
// 函数功能： 对数函数非线性灰度增强
// 输入参数： double a, double b,double c-调整曲线位置和形状的参数
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CGrayTransformDib::Log_Transform( double a, double b, double c)
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	BYTE pixel;	//像素值

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//逐个扫面图像中的像素点,进行对数函数非线性灰度变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像倒数第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)*lpSrc;

			//根据公式(5-4)求出目标图像中与当前点对应的像素点的灰度值
             
			*lpDst = (BYTE)((log((double)(pixel+1)))/(b*log(c))+a+0.5);

		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;

}

