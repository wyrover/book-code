// ************************************************************************
//  文件名：restore.cpp
//
//  图像复原API函数库：
//
//  DIBNoRestriction()			- 图像模糊
//  DIBInverseFilter()			- 图像逆滤波复原
//  DIBNoiseDegeneration()		- 图像模糊加噪
//  DIBWinnerFilter()			- 图像维纳滤波
//	DIBMotionDegeneration()		- 图像运动模糊
//	DIBMotionRestore()			- 图像运动模糊复原
//
// *************************************************************************


#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
 *
 * 函数名称：
 *   DIBNoRestriction()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL WINAPI DIBNoRestriction(CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp;

	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//图像归一化因子
	double MaxNum;
	
	//输入图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// 将数据存入时域数组
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//对源图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//频域相乘
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//对结果图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//选择归一化因子
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//转换为图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBInverseFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE
 *
 * 说明:
 *   该函数用来对DIBInverseFilter()生成的DIB图像进行复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBInverseFilter (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//临时变量
	double tempre, tempim, a, b, c, d;

	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//图像归一化因子
	double MaxNum;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// 将退化图象数据存入时域数组
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	//频域相除
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		a = pCFSrc[i].real();
		b = pCFSrc[i].imag();
		c = pCFH[i].real();
		d = pCFH[i].imag();
		
		//如果频域值太小，不予考虑
		if (c*c + d*d > 1e-3)
		{
			tempre = ( a*c + b*d ) / ( c*c + d*d );
			tempim = ( b*c - a*d ) / ( c*c + d*d );
		}

		pCFSrc[i]= complex<double>(tempre , tempim);
	}

	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//确定归一化因子
	MaxNum=300;

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：//加噪退化已经删除
 *   DIBNoiseDegeneration()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 模糊加噪操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊加噪操作。
 *
 ************************************************************************/

BOOL WINAPI DIBNoiseDegeneration (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//转换为图像，加噪
	unsigned char NoisePoint;
	
	//临时变量
	double temp;

	//图像归一化因子
	double MaxNum;

	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
		
	// 为时域和频域的数组分配空间
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];
	
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针				
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 用来对图象做退化的系统
			if(i < 5 && j <5 )
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			
			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//对源图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//频域相乘
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//对结果图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//选择归一化因子
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//生成伪随机数种子
	srand((unsigned)time(NULL));

	//转换为图像，并加入伪随机噪声
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 产生的噪声
			NoisePoint = rand()/2048-8;
			
			// 指向源图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// 时域加噪，存储象素值
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum + NoisePoint);
			
			//如果象素值过大，直接赋值255
			if(*lpSrc > 255)
				*lpSrc = 255 ;
		}
	}	
	
	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBWinnerFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 维纳滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行维纳滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBWinnerFilter (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp, tempre, tempim, 
			a, b, c, d, norm2;
	
	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];

	// 滤波器加权系数
	double *pCFFilter   = new double [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 退化系统时域及维纳滤波加权系数赋值
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
				pCFFilter[ lLineBytes*j + i ] = 0.5;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);	
				pCFFilter[ lLineBytes*j + i ] = 0.05;
			}

			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	// 计算M
	for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = pCFSrc[i].real();
			b = pCFSrc[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm2 = c * c + d * d;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			temp  = (norm2 ) / (norm2 + pCFFilter[i]);
			{				
				tempre = ( a*c - b*d ) / ( c*c + d*d );
				tempim = ( b*c + a*d ) / ( c*c + d*d );
				// 求得f(u,v)
				pCFSrc[i]= complex<double>(temp*tempre , temp*tempim);
				//pCFSrc[i]= complex<double>((a*c-b*d)/norm2 , (b*c+a*d)/norm2);
			}						
	}

	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2) + 40;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCFFilter;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMotionDegeneration()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像模拟由匀速直线运动造成的模糊
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionDegeneration(CDib *pDib)
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
	
	//循环变量
	long iColumn;
	long jRow;

	//临时变量
	int temp,m;

	// 临时变量
	double p,q;
	
	int nTotTime, nTotLen, nTime;

	//总的运动时间10s
	nTotTime = 10;

	// 总的运动距离10个象素点
	nTotLen = 10;
	
	// 摄像机的暴光系数
	double B;

	B = 0.1;

	//用来存储源图象和变换核的时域数据
	int *nImageDegener;

	// 为时域和频域的数组分配空间
	nImageDegener = new int [lHeight*lLineBytes];
	
	// 将数据存入时域数组
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			temp=0;
			
			// 指向源图像倒数第jRow行，第iColumn个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;

			// 象素点的象素值积累
			for ( nTime = 0; nTime < nTotTime; nTime++ )
			{
				p = (float)iColumn - (float)(nTotLen)*nTime/nTotTime;
				
				if (p > 0)
				{
					q = p - floor((double)p);
					
					if(q >= 0.5)
						m = (int)ceil((double)p);
					else
						m = (int)floor((double)p);

					// 累加
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m;
					temp = temp + *lpSrc;
				}	
			}
			
			// 乘以摄像机的暴光系数
			temp = B * temp;

			temp=(int)ceil((double)temp);
			
			// 使得temp的取值符合取值范围
			if(temp<0)
				temp=0;

			if(temp>255)
				temp=255;

			nImageDegener[lLineBytes*jRow + iColumn] = temp;
		}
	}	
	
	//转换为图像
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// 指向源图像倒数第jRow行，第iColumn个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}
	
	//释放存储空间
	delete nImageDegener;
	
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMotionRestore()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对拟由匀速直线运动造成的模糊图象进行复原
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionRestore(CDib *pDib)
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
	
	//循环变量
	long iColumn;
	long jRow;

	int i,n,m;

	//临时变量
	int temp1,temp2,
		totalq,q1,q2,z;

	double p,q;

	// 常量A赋值
	int A = 80;
	
	//常量B赋值
	int B = 10;
	
	//总的移动距离
	int nTotLen=10;

	// 图象宽度包含多少个ntotlen
	int K=lLineBytes/nTotLen;
	
	int error[10];

	//用来存储源图象时域数据
	int *nImageDegener;

	// 为时域数组分配空间
	nImageDegener = new int [lHeight*lLineBytes];

	// 将象素存入数组中
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;	
			nImageDegener[lLineBytes*jRow + iColumn] = (*lpSrc);
		}
	}	
	
	for (jRow = 0; jRow < lHeight; jRow++)
	{		
		// 计算error[i]
		for(i = 0; i < 10; i++)
		{			
			error[i] = 0;			
			for(n = 0; n < K; n++)
			{
				for(m = 0; m <= n; m++)
				{
					// 象素是否为一行的开始处
					if(i == 0 && m == 0)
					{
						temp1=temp2=0;
					}					
					// 进行差分运算
					else
					{
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i;
						temp1=*lpSrc;						
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i-1;
						temp2 = *lpSrc;
					}					
					error[i] = error[i] + temp1 - temp2;					
				}
			}
			error[i] = B * error[i] / K;
		}		
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{			
			// 计算m，以及z
			m = iColumn / nTotLen;
			z = iColumn - m*nTotLen;				
			// 初始化
			totalq = 0;	q = 0;			
			for(n = 0; n <= m; n++)
			{
				q1 = iColumn - nTotLen*n;				
				if(q1 == 0)
					q = 0;				
				// 进行差分运算
				else
				{
					q2 = q1 - 1;					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q1;
					temp1 = *lpSrc;					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q2;
					temp2 = *lpSrc;					
					q = (temp1 - temp2) * B;			
				}				
				totalq = totalq + q;
			}			
			p = error[z];
			// 得到f(x,y)的值
			temp1 = totalq + A - p;			
			// 使得象素的取值符合取值范围
			if(temp1 < 0)
				temp1 = 0;			
			if(temp1 > 255)
				temp1 = 255;						
			nImageDegener[lLineBytes*jRow + iColumn] = temp1;
		}
	}
	//转换为图像
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// 指向源图像倒数第jRow行，第iColumn个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			// 存储象素值
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}	
	
	//释放存储空间
	delete nImageDegener;
	
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMinDblFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 维纳滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行最小平方滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBMinDblFilter (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp, tempre, tempim, 
			a, b, c, d, e, f, norm1,norm2;
	
	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];

	// 滤波器加权系数
	complex<double> *pCFFilter   = new complex<double> [lHeight*lLineBytes];
	complex<double> *pCFFilterD   = new complex<double> [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			pCFFilterD[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 退化系统时域及维纳滤波加权系数赋值
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);			
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);					
			}
			pCFFilter[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);

			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	pCFFilter[ lLineBytes*0 + 1 ] = complex<double>(-1 , 0.0);
	pCFFilter[ lLineBytes*1 + 0 ] = complex<double>(-1 , 0.0);
	pCFFilter[ lLineBytes*1 + 1 ] = complex<double>( 4 , 0.0);
	pCFFilter[ lLineBytes*1 + 2 ] = complex<double>(-1 , 0.0);
	pCFFilter[ lLineBytes*2 + 1 ] = complex<double>(-1 , 0.0);
	
	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//对过滤器进行变换FFT
	::DIBFFT_2D(pCFFilter, lLineBytes, lHeight, pCFFilterD);
	
	// 计算M
	double gama=0.05;
	for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = pCFSrc[i].real();
			b = pCFSrc[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
			e = pCFFilter[i].real();
			f = pCFFilter[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm1 = c * c + d * d;
			norm2 = e * e + f * f;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			temp  = norm1 + norm2 * gama ;
			{				
				tempre = ( a*c + b*d ) / temp;
				tempim = ( b*c - a*d ) / temp;
				// 求得f(u,v)
				pCFSrc[i]= complex<double>(tempre , tempim);
			}						
	}

	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2) + 40;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCFFilter;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMaxProFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 最大后验滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行最大后验滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBMaxProFilter (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp, tempre, tempim, 
			a, b, c, d, e, f, norm1,norm2;
	
	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据，*pCFHnew新的滤波器
	complex<double>  *pCFSrc,*pCFH,*pGF,*pCFHnew,*f1,*f2;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间,
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];
	pGF	    	= new complex<double> [lHeight*lLineBytes];
	pCFHnew     = new complex<double> [lHeight*lLineBytes];
	f1     = new complex<double> [lHeight*lLineBytes];
    f2     = new complex<double> [lHeight*lLineBytes];
	// 滤波器加权系数
	double *pCFFiletr   = new double [lHeight*lLineBytes];
	complex<double> *pCTNoise   = new complex<double> [lHeight*lLineBytes];
	complex<double> *pCFNoise   = new complex<double> [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
				
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 退化系统时域及维纳滤波加权系数赋值
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);	
				pCFFiletr[ lLineBytes*j + i ] = 0.05;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);			
				pCFFiletr[ lLineBytes*j + i ] = 0.025;
			}			

			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			
			if(i==j)
				pCTNoise[ lLineBytes*j + i ] = complex<double>(10 , 0.0);
			else
				pCTNoise[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	pGF=pCFSrc;
	f1=pCFSrc;
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//对过滤器进行变换FFT
	::DIBFFT_2D(pCTNoise, lLineBytes, lHeight, pCFNoise);
	
	// 计算M
	double gama=0.05;
	double fain=0,faif=0,lamta;
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
		a = f1[i].real();
		b = f1[i].imag();		
		e = pCFNoise[i].real();
		f = pCFNoise[i].imag();
		fain += (e*e+f*f);
		faif += (a*a+b*b);		 
	}
	lamta = fain/faif;
	for (int k=1;k<6;k++)
	{
	
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = f1[i].real();
			b = f1[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
			e = pCFNoise[i].real();
			f = pCFNoise[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm1 = c * c + d * d;
			norm2 = a * a + b * b;
		//	norm3 = e * e + f * f;
		//	fain += norm2;
		//	faif += (a*a+b*b);
		 	//lamta =( e * e + f * f)/norm2;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			//temp  = norm1 + lamta ;
			temp  = norm1 + lamta ;			
			{				
				tempre = ( a*c + b*d ) / temp;
				tempim = ( b*c - a*d ) / temp;
				// 求得f(u,v)
				pCFSrc[i]= complex<double>(tempre , tempim);
			}						
	}
     f2=pCFSrc;
	 for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = f1[i].real();
			b = f1[i].imag();
			c = f2[i].real();
			d = f2[i].imag();
			e = pGF[i].real();
			f = pGF[i].imag();
		
			// |H(u,v)|*|H(u,v)|
			norm1 = a * a + b * b;
			norm2 = c * c + d * d;
			
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			//temp  = norm1 + lamta ;
			temp  = norm1 + norm2/lHeight/lLineBytes ;
			temp=lHeight * lLineBytes *temp;
			{				
				tempre = ( e*c - f*d ) / temp;
				tempim = ( f*c + e*d ) / temp;
				// 求得f(u,v)
				pCFH[i]= complex<double>(tempre , tempim);
			}
			f1=f2;
			
	}
	}
	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2)+10 ;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCTNoise;
	delete pCFNoise;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBProjectFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 投影滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行投影滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBProjectFilter (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;
	int N=lHeight;//初始化
	int K=15;//迭代次数
	double **arr = new double*[N];
	//构造系数阵a
	for(j=0;j<N;j++)
	{
		arr[j] = new double[lLineBytes];
		for(i=0;i<lLineBytes;i++)
			arr[j][i] =1.15;
	}
	//a为系数阵
	int **pDesFi = new int*[lHeight];
	int **tempFi = new int*[lHeight];
	int **gGFi = new int*[lHeight];
	int *Gj = new int[lHeight];
	int *Kj = new int[lHeight];
	for(j=0;j<lHeight;j++)
	{
		pDesFi[j] = new int[lLineBytes];
		gGFi[j] = new int[lLineBytes];
		tempFi[j] = new int[lLineBytes];
		for(i=0;i<lLineBytes;i++)
		{
			pDesFi[j][i]=*((unsigned char *)lpDIBBits + lLineBytes * j + i);//初始化
			gGFi[j][i]=*((unsigned char *)lpDIBBits + lLineBytes * j + i);//初始化
		}
	}
	for(j=1;j<=N*K;j++)
	{
		// ajTcaj是计算系数模
		double ajTcaj = 0;
		for(i=0;i<lLineBytes;i++)
		{			
			ajTcaj = ajTcaj + arr[(j-1)%N][i] * arr[(j-1)%N][i];
		}
		// 
		for(i=0;i<lHeight;i++)
		{
			double t1=0,t2=0;
			for(int m=0;m<lLineBytes;m++)
			{				
				t1 = t1 + arr[(j-1)%N][m]*pDesFi[i][m];
				t2 = t2 + gGFi[m][i];
			}
			Kj[i] = t1;//令a(i)T*f(i-1) = Kj
			Gj[i] = t2;
			Kj[i] = Kj[i]-Gj[i];
		}
		for(int m=0;m<lHeight;m++)
		{
			for(i=0;i<lLineBytes;i++)
			{
				tempFi[m][i] = Kj[i]*arr[(j-1)%N][i]/ajTcaj;
				pDesFi[m][i] = pDesFi[m][i] - tempFi[m][i];
			}
		}		
	}	

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			*lpSrc = (unsigned char) (pDesFi[j][i]);		
		}
	}
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBBlindFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 投影滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行迭代盲目复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBBlindFilter (CDib *pDib)
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
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp, tempre, tempim, 
			a, b, c, d, e, f, norm1,norm2;
	
	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据，*pCFHnew新的滤波器
	complex<double>  *pCFSrc,*pCFH,*pGF,*pCFHnew,*f1,*f2;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间,
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];
	pGF	    	= new complex<double> [lHeight*lLineBytes];
	pCFHnew     = new complex<double> [lHeight*lLineBytes];
	f1     = new complex<double> [lHeight*lLineBytes];
    f2     = new complex<double> [lHeight*lLineBytes];
	// 滤波器加权系数
	double *pCFFiletr   = new double [lHeight*lLineBytes];
	complex<double> *pCTNoise   = new complex<double> [lHeight*lLineBytes];
	complex<double> *pCFNoise   = new complex<double> [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
				
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 退化系统时域及维纳滤波加权系数赋值
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);	
				pCFFiletr[ lLineBytes*j + i ] = 0.05;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);			
				pCFFiletr[ lLineBytes*j + i ] = 0.025;
			}			

			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			
			if(i==j)
				pCTNoise[ lLineBytes*j + i ] = complex<double>(0.1 , 0.0);
			else
				pCTNoise[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	pGF=pCFSrc;
	f1=pCFSrc;
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//对过滤器进行变换FFT
	::DIBFFT_2D(pCTNoise, lLineBytes, lHeight, pCFNoise);
	//计算噪声功率谱
	double alf=0;
	
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
		
		a = pCFNoise[i].real();
		b = pCFNoise[i].imag();
		alf=alf+a*a+b*b;
				 
	}
   alf=alf/lLineBytes/lHeight;
		
	for (int k=1;k<10;k++)
	{
	
		for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = f1[i].real();
			b = f1[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();
	     	e = pGF[i].real();
			f = pGF[i].imag();
			// |H(u,v)|*|H(u,v)|
				norm1 = a * a + b * b;
				norm2 = c * c + d * d;
			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			//temp  = norm1 + lamta ;
			temp  = norm1 + 1/ norm2;			
			{				
				tempre = ( a*e + b*f ) / temp;
				tempim = ( a*f - e*b ) / temp;
				// 求得f(u,v)
				pCFHnew[i]= complex<double>(tempre , tempim);
			}						
	}
     
	 for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = f1[i].real();
			b = f1[i].imag();
			
	    	c = pCFH[i].real();
			d = pCFH[i].imag();
			e = pGF[i].real();
			f = pGF[i].imag();
			// |H(u,v)|*|H(u,v)|
			norm1 = a * a + b * b;
			norm2 = c * c + d * d;
			// a/(|f(u,v)|*|f(u,v)|)+(|H(u,v)|*|H(u,v)|)
			temp  = 1/norm1*alf + norm2 ;
			
			{				
				tempre = ( e*c + f*d ) / temp;
				tempim = ( c*f - d*e ) / temp;
				// 求得f(u,v)
				pCFSrc[i]= complex<double>(tempre , tempim);
			}
				
	}
	 f1=pCFSrc;
	 pCFH=pCFHnew;
	}
	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2)+10 ;
			//norm2  = sqrt(norm2) ;
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCTNoise;
	delete pCFNoise;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMaxEntrFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 最大熵滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行最大熵滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBMaxEntrFilter(CDib *pDib)
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
	
	//循环变量
	long i,j,N=120;
	const int T = 5;
	double PSF[T][T];
	for(i=0;i<T;i++)
		for(j=0;j<T;j++)
			PSF[i][j]=1.0/(T*T);

	double **blurred_image = new double*[lHeight];	
	double **lambda = new double*[lHeight];	
	double **deblurred = new double*[lHeight];
	double **reblurred = new double*[lHeight];
	double **deblurred_image = new double*[lHeight];

	double mean2=0;
	double totd=0;
	double scale=0;
	double entropy=0; 
	
	for(j=0;j<lHeight;j++)
	{
		blurred_image[j] = new double[lLineBytes];
		lambda[j] = new double[lLineBytes];		
		deblurred[j] = new double[lLineBytes];
		reblurred[j] = new double[lLineBytes];
		deblurred_image[j] = new double[lLineBytes];

		for(i=0;i<lLineBytes;i++)
		{			
			blurred_image[j][i]=*((unsigned char *)lpDIBBits + lLineBytes * j + i);//初始化
			mean2 += blurred_image[j][i];			
			lambda[j][i] = 0;//产生全0矩阵		
		}
	}

	mean2 = mean2/( lHeight*lLineBytes );//blurred_image 的均值
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			blurred_image[j][i]=blurred_image[j][i]
				+mean2*1/1000.0;
			totd += blurred_image[j][i];
		}
	}

	double sumT=0,sum=0;
	for(int n=1;n<=N;n++)
	{
		sumT=0;
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				int xx,yy;
				sum=0;
				for(xx=0;xx<T;xx++)
				{
					for(yy=0;yy<T;yy++)
					{
						if( (i+xx-T/2)<lHeight && (i+xx-T/2)>=0 && (j+yy-T/2)<lLineBytes && (j+yy-T/2)>=0 )
						{
							sum = sum + lambda[i+xx-T/2][j+yy-T/2]*PSF[xx][yy];
						}
					}
				}
				deblurred[i][j] = exp(-1+sum);
				sumT += deblurred[i][j];
			}
		}
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				deblurred[i][j] = deblurred[i][j]/sumT;
			}
		}
		sumT = 0;
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				int xx,yy;
				sum=0;
				for(xx=0;xx<T;xx++)
				{
					for(yy=0;yy<T;yy++)
					{
						if( (i+xx-T/2)<lHeight && (i+xx-T/2)>=0 && (j+yy-T/2)<lLineBytes && (j+yy-T/2)>=0 )
						{
							sum = sum + deblurred[i+xx-T/2][j+yy-T/2]*PSF[xx][yy];
						}
					}
				}
				reblurred[i][j] = sum;
				sumT += reblurred[i][j];
			}
		}
		scale = sumT/totd;
		//err没有做，后面没有用到		
		for(i=0;i<lHeight;i++)	
		{
			for(j=0;j<lLineBytes;j++)
			{
				entropy += deblurred[i][j]*log(deblurred[i][j]);
			}
		}
		entropy = -entropy;		
		if(n<30)
		{
			for(i=0;i<lHeight;i++)	
			{
				for(j=0;j<lLineBytes;j++)
				{
					lambda[i][j]=lambda[i][j]+(blurred_image[i][j]*scale-reblurred[i][j]);
				}
			}
		}
		else
		{
			for(i=0;i<lHeight;i++)	
			{
				for(j=0;j<lLineBytes;j++)
				{
					lambda[i][j]=lambda[i][j]+log(blurred_image[i][j]*scale/reblurred[i][j]);
				}
			}
		}
	}
	for(i=0;i<lHeight;i++)	
	{
		for(j=0;j<lLineBytes;j++)
		{
			deblurred_image[i][j]=deblurred[i][j]/scale-mean2/1000;
		}
	}
	
	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			*lpSrc = (unsigned char) ((deblurred_image[j][i]+0.5)>255?255:deblurred_image[j][i]+0.5);		
		}
	}
	return true;
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBGeoFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 几何复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行几何复原操作。
 *
 ************************************************************************/
BOOL WINAPI DIBGeoFilter(CDib *pDib)
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
	
	int  i,j,g,l;
	double xu,yu,xd,yd,r,m,n,p,q;
	char *lpDIBBitsNew = new char[lHeight*lLineBytes];	

	for(i=0;i<lHeight;i++)
	{
		for(j=0;j<lLineBytes;j++)
		{			
			lpDIBBitsNew[i*lLineBytes+j]=*((unsigned char *)lpDIBBits + lLineBytes * i + j);//初始化
		}
	}

	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{          
			xu=(i-382.491)/599.359;
			yu=(j-268.013)/598.340;
			r=xu*xu+yu*yu;
			xd=xu+xu*r*(-0.304484-0.040202*r)+2*0.002783*xu*yu-0.000259*(r+2*xu);
			yd=yu+yu*r*(-0.304484-0.040202*r)+0.002783*(r+2*yu)-2*0.000259*xu*yu;
			m=xd*599.359+382.491;		  
			n=yd*598.340+268.013;
			g=int(m);       
			l=int(n);
			p=m-g;
			q=n-l;
			*(lpDIBBitsNew+j*lLineBytes+i)=
				(1-p)*(1-q)* (*(lpDIBBits+ l     *lLineBytes+  g))+
				(1-p)* q   * (*(lpDIBBits+ (l+1) *lLineBytes+  g))+
				  p  *(1-q)* (*(lpDIBBits+ (l)   *lLineBytes+  g+1))+
				  p  *  q  * (*(lpDIBBits+ (l+1) *lLineBytes+  g+1)); 
		}
	}

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			*lpSrc = (unsigned char) lpDIBBitsNew[j*lLineBytes+i];	
		}
	}
	return true;
}
#undef SWAP

