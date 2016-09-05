// function.h: interface for the function class.
//接口函数类
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTION_H__6E194843_FEB3_491F_8062_765AA3465CBC__INCLUDED_)
#define AFX_FUNCTION_H__6E194843_FEB3_491F_8062_765AA3465CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Dib.h"
#include <math.h>
// 常数π
#define pi 3.1415926535
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#include <direct.h>
#include <complex>
using namespace std;
#define PI 3.14159265358979323846

//#endif // !defined(AFX_FUNCTION_H__6E194843_FEB3_491F_8062_765AA3465CBC__INCLUDED_)

typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;


struct CplexNum
{
	double re;
	double im;
};

/////////////////////////////////////////////////////////
//用于复数运算
CplexNum Add(CplexNum c1,CplexNum c2)
{
	CplexNum c;
	c.re=c1.re+c2.re;
	c.im=c1.im+c2.im;
	return c;
}
CplexNum Sub(CplexNum c1,CplexNum c2)
{
	CplexNum c;
	c.re=c1.re-c2.re;
	c.im=c1.im-c2.im;
	return c;
}
CplexNum Mul(CplexNum c1,CplexNum c2)
{
	CplexNum c;
	c.re=c1.re*c2.re-c1.im*c2.im;
	c.im=c1.re*c2.im+c2.re*c1.im;
	return c;
}

////////////////////////////////////////////////////////
/*************************************************************************
* 函数名称：AngleToRadian(int angle)
* 函数参数:
*   int angle
* 函数类型:float      
* 函数功能:用来角度到弧度的转换
************************************************************************/

double AngleToRadian(int angle)
{
	return ((angle)*pi/180.0);
}


/*************************************************************************
* 函数名称：ThreeCrossMat(double *pMatrix, int rank, double *pQMatrix, 
double *pMainCross, double *pHypoCross)
* 函数参数:
*   double  *pMatrix ，指向矩阵的指针
*   int     rank  ，矩阵阶数
*   double	 *pQMatrix  ， householder变换后的的矩阵的指针
*   double  *pMainCross ，对称三角阵中的主对角元素的指针
*   double  *pHypoCross ， 对称三角阵中的次对角元素的指针
* 函数类型:BOOL
*函数功能:将n阶实对称矩阵化为对称三角阵
************************************************************************/

BOOL  ThreeCrossMat(double *pMatrix, int rank, double *pQMatrix, 
					double *pMainCross, double *pHypoCross)
{	
	int i, j, k, u;//变量声明
	double h, f, g, h2;   

	for(i = 0; i <= rank-1; i++)// 将矩阵pQMatrix初始化
	{
		for(j = 0; j <= rank-1; j++)
		{
			u = i*rank + j; 
			pQMatrix[u] = pMatrix[u];
		}
	}
	for (i = rank-1; i >= 1; i--)
	{
		h = 0.0;
		if (i > 1)
			for (k = 0; k <= i-1; k++)
			{
				u = i*rank + k; 
				h = h + pQMatrix[u]*pQMatrix[u];
			}      

			if (h + 1.0 == 1.0)// 如果一行全部为零
			{
				pHypoCross[i] = 0.0;
				if (i == 1) 
				{
					pHypoCross[i] = pQMatrix[i*rank+i-1];
				}
				pMainCross[i] = 0.0;
			}        		
			else
			{// 否则求正交矩阵的值

				pHypoCross[i] = sqrt(h);// 求次对角元素的值            			
				u = i*rank + i - 1;
				if (pQMatrix[u] > 0.0) // 判断i行i-1列元素是不是大于零
				{
					pHypoCross[i] = -pHypoCross[i];
				}            
				h = h - pQMatrix[u]*pHypoCross[i];
				pQMatrix[u] = pQMatrix[u] - pHypoCross[i];
				f = 0.0;            			
				for (j = 0; j <= i-1; j++)// householder变换
				{ 
					pQMatrix[j*rank+i] = pQMatrix[i*rank+j] / h;
					g = 0.0;

					for (k = 0; k <= j; k++)
					{
						g = g + pQMatrix[j*rank+k]*pQMatrix[i*rank+k];
					}                
					if (j+1 <= i-1)
						for (k = j+1; k <= i-1; k++)
				  {
					  g = g + pQMatrix[k*rank+j]*pQMatrix[i*rank+k];
						}                
						pHypoCross[j] = g / h;
						f = f + g*pQMatrix[j*rank+i];
				}

				h2 = f / (h + h);            			
				for (j = 0; j <= i-1; j++)// 求正交矩阵的值
				{
					f = pQMatrix[i*rank + j];
					g = pHypoCross[j] - h2*f;
					pHypoCross[j] = g;

					for (k = 0; k <= j; k++)
					{
						u = j*rank + k;
						pQMatrix[u] = pQMatrix[u] - f*pHypoCross[k] - g*pQMatrix[i*rank + k];
					}
				}
				pMainCross[i] = h;
			}
	}

	for (i = 0; i <= rank-2; i++) // 赋零值
	{
		pHypoCross[i] = pHypoCross[i + 1];
	}
	pHypoCross[rank - 1] = 0.0;
	pMainCross[0]        = 0.0;    
	for (i = 0; i <= rank-1; i++)
	{ // 主对角元素的计算		
		if ((pMainCross[i] != 0.0) && (i-1 >= 0))
			for (j = 0; j <= i-1; j++)
			{
				g = 0.0;
				for (k = 0; k <= i-1; k++)
				{g = g + pQMatrix[i*rank + k]*pQMatrix[k*rank + j];
				}
				for (k = 0; k <= i-1; k++)
				{ 
					u = k*rank + j;
					pQMatrix[u] = pQMatrix[u] - g*pQMatrix[k*rank + i];
				}
			}       
			u = i*rank + i;// 存储主对角线的元素
			pMainCross[i] = pQMatrix[u]; 
			pQMatrix[u]   = 1.0;       		
			if (i-1 >= 0)// 将三对角外所有的元素赋零值
				for (j = 0; j <= i-1; j++)
				{ 
					pQMatrix[i*rank + j] = 0.0; 
					pQMatrix[j*rank+i]   = 0.0;
				}
	}    
	return(TRUE);// 返回
}


/*************************************************************************
*
* 函数名称：EigenvalueVector(int rank, double *pMainCross, double *pHypoCross, 
double *pMatrix, double Precision, int MaxT)
* 函数参数:
*   int     rank ，矩阵A的阶数
*   double	 *pMainCross  ，对称三角阵中的主对角元素的指针，返回时存放A的特征值
*   double  *pHypoCross  ，对称三角阵中的次对角元素的指针
*	 double  *pMatrix  ，对称矩阵A的特征向量的指针
*   double Precision ，控制精度
*   int MaxT  ， 最大迭代次数
* 函数类型:BOOL 
* 函数功能:用计算实对称三角矩阵的全部特征值以及相应的特征向量
************************************************************************/
BOOL EigenvalueVector(int rank, double *pMainCross, double *pHypoCross, 
					  double *pMatrix, double Precision, int MaxT)
{

	int i, j, k, m, it, u, v;// 变量声明
	double d, f, h, g, p, r, e, s;

	pHypoCross[rank - 1] = 0.0; // 初始化
	d = 0.0; 
	f = 0.0;

	for(j = 0; j <= rank-1; j++)
	{							//  迭代精度的控制		
		it = 0;
		h = Precision * (fabs(pMainCross[j]) + fabs(pHypoCross[j]));
		if(h > d) 
		{
			d = h;
		}
		m = j;

		while((m <= rank-1) && (fabs(pHypoCross[m]) > d)) 
		{
			m = m + 1;    
		}
		if(m != j)
		{
			// 迭代求矩阵A的特征值和特征向量
			do
			{
				// 超过迭代次数，迭代失败
				if(it == MaxT)
				{
					return(FALSE);
				}
				it = it + 1;
				g = pMainCross[j];
				p = (pMainCross[j + 1] - g) / (2.0 * pHypoCross[j]);
				r = sqrt(p*p + 1.0);                
				// 如果p大于0
				if (p >= 0.0)
				{
					pMainCross[j] = pHypoCross[j]/(p + r);
				}
				else
				{
					pMainCross[j] = pHypoCross[j]/(p - r); 
				}
				h = g - pMainCross[j];                
				//  计算主对角线的元素
				for (i = j + 1; i <= rank - 1; i++)
				{
					pMainCross[i] = pMainCross[i] - h;  
				}
				// 赋值
				f = f + h;
				p = pMainCross[m];
				e = 1.0; s = 0.0;

				for(i = m - 1; i >= j; i--)
				{
					g = e * pHypoCross[i];
					h = e * p;

					//  主对角线元素的绝对值是否大于次对角线元素的
					if(fabs(p) >= fabs(pHypoCross[i]))
					{
						e = pHypoCross[i] / p;
						r = sqrt(e*e + 1.0);
						pHypoCross[i + 1] = s*p*r; 
						s = e / r;  e = 1.0 / r;
					}
					else
					{
						e = p / pHypoCross[i]; 
						r = sqrt(e*e + 1.0);
						pHypoCross[i+1] = s * pHypoCross[i] * r;
						s = 1.0 / r; e = e / r;
					}                   
					p = e*pMainCross[i] - s*g;
					pMainCross[i + 1] = h + s*(e*g + s*pMainCross[i]);

					// 重新存储特征向量
					for(k = 0; k <= rank - 1; k++)
					{
						u = k*rank + i + 1; v = u - 1;
						h = pMatrix[u]; 
						pMatrix[u] = s*pMatrix[v] + e*h;
						pMatrix[v] = e*pMatrix[v] - s*h;
					}

				}               
				// 将主对角线和次对角线元素重新赋值
				pHypoCross[j] = s * p; 
				pMainCross[j] = e * p;

			}
			while (fabs(pHypoCross[j]) > d);
		}

		pMainCross[j] = pMainCross[j] + f;
	}   	
	for (i = 0; i <= rank-1; i++)
	{// 返回A的特征值
		k = i; p = pMainCross[i];        		
		if(i+1 <= rank-1)
		{// 将A特征值赋给p
			j = i + 1;
			while((j <= rank-1) && (pMainCross[j] <= p))
			{ k = j; 
			p = pMainCross[j]; 
			j = j+1;
			}
		}        		
		if (k != i)
		{// 存储A的特征值和特征向量
			pMainCross[k] = pMainCross[i];
			pMainCross[i] = p;
			for(j = 0; j <= rank-1; j++)
			{
				u = j*rank + i; 
				v = j*rank + k;
				p = pMatrix[u]; 
				pMatrix[u] = pMatrix[v];
				pMatrix[v] = p;
			}
		}
	}  
	return(TRUE);// 返回
}

/*************************************************************************
* 函数名称：LogTranslation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lLineBytes)
* 函数参数:
LPSTR	lpSrcStartBits,指向DIB起始像素的指针
long	lWidth，DIB图像的宽度
long	lHeight，DIB图像的高度
long	lLineBytes，DIB图像的行字节数，为4的倍数
* 函数类型:BOOL        
* 函数功能:该函数用来对图像进行对数变换 
************************************************************************/

BOOL  LogTranslation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lLineBytes)
{			
	long i;                 //行循环变量
	long j;                 //列循环变量
	unsigned char*	lpSrcUnChr;	//指向像素的指针
	for(i = 0; i < lHeight; i++)// 行
	{		
		for(j = 0; j < lWidth; j++)// 列
		{	
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;// 指向DIB第i行，第j个像素的指针
			*lpSrcUnChr=50*(unsigned char)log((double)(*lpSrcUnChr+1));
		}
	}
	return TRUE;
}

/*************************************************************************
* 函数名称：
*   Transpose(LPSTR lpSrcDib,LPSTR lpDibBits,long	lWidth,long	lHeight,
long lLineBytes,long lDstLineBytes)
* 函数参数:
*   LPSTR	lpSrcDib，指向源DIB的指针
LPSTR	lpSrcStartBits,指向DIB起始像素的指针
long	lWidth，DIB图像的宽度
long	lHeight，DIB图像的高度
long	lLineBytes，DIB图像的行字节数，为4的倍数
long	lDstLineBytes，临时DIB图像的行字节数，为4的倍数
* 函数类型:BOOL        
* 函数功能:该函数用来转置DIB图像
************************************************************************/

BOOL  Transpose(LPSTR lpSrcDib,LPSTR lpSrcStartBits,long lWidth,long lHeight,
				long lLineBytes,long lDstLineBytes)
{
	long i;                 //行循环变量
	long j;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针
	LPSTR	lpDstStartBits;	//指向临时图像对应像素的指针 		
	HLOCAL	hDstDIBBits;	//临时图像句柄
	LPBITMAPINFOHEADER lpbmi;							  // 指向BITMAPINFOHEADER结构的指针
	lpbmi = (LPBITMAPINFOHEADER)lpSrcDib;
	hDstDIBBits= LocalAlloc(LHND, lWidth * lDstLineBytes);// 分配临时内存
	if (hDstDIBBits== NULL)								 // 判断是否内存分配
	{		
		return FALSE;									// 分配内存失败
	}	
	lpDstStartBits= (char * )LocalLock(hDstDIBBits);	// 锁定内存		
	for(i = 0; i < lHeight; i++)						// 针对图像每行进行操作
	{		
		for(j = 0; j < lWidth; j++)						// 针对每行图像每列进行操作
		{						
			lpSrcDIBBits= (char *)lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;// 指向源DIB第i行，第j个像素的指针
			lpDstDIBBits= (char *)lpDstStartBits + lDstLineBytes * (lWidth - 1 - j) + i;// 指向转置DIB第j行，第i个像素的指针			
			*(lpDstDIBBits)= *(lpSrcDIBBits);			// 复制像素			
		}		
	}	
	memcpy(lpSrcStartBits, lpDstStartBits, lWidth * lDstLineBytes);// 复制转置后的图像
	lpbmi->biWidth = lHeight;		
	lpbmi->biHeight = lWidth;

	LocalUnlock(hDstDIBBits);							// 释放内存
	LocalFree(hDstDIBBits);								
	return TRUE;										// 返回
}

/*************************************************************************
* 函数名称：Mirror(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
* 函数参数:
LPSTR	lpSrcStartBits,指向DIB起始像素的指针
long	lWidth，DIB图像的宽度
long	lHeight，DIB图像的高度
long	lLineBytes，DIB图像的行字节数，为4的倍数
* 函数类型:BOOL        
* 函数功能:该函数用来镜像DIB图像，本程序只实现了水平镜像，垂直镜像的原理书中也谈到。 很容易实现           
************************************************************************/

BOOL  Mirror(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
{	
	long i;                 //行循环变量
	long j;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针 		
	HLOCAL	hDstDIBBits;	//临时图像句柄
	LPSTR	lpBits;	// 指向中间像素的指针，当复制图像时，提供临时的像素内存空间
	hDstDIBBits= LocalAlloc(LHND, lLineBytes);// 分配临时内存保存行图像
	if (hDstDIBBits == NULL)		
	{		
		return FALSE;									// 分配内存失败
	}		
	lpDstDIBBits= (char * )LocalLock(hDstDIBBits);// 锁定				
	for(i = 0; i < lHeight; i++)// 水平镜像，针对图像每行进行操作
	{			
		for(j = 0; j < lWidth / 2; j++)// 针对每行图像左半部分进行操作
		{							
			lpSrcDIBBits= (char *)lpSrcStartBits + lLineBytes * i + j;// 指向倒数第i行，第j个像素的指针								
			lpBits= (char *)lpSrcStartBits + lLineBytes * (i + 1) - j;// 指向倒数第i+1行，倒数第j个像素的指针								
			*lpDstDIBBits=*lpBits;//保存中间像素								
			*lpBits = *lpSrcDIBBits;// 将倒数第i行，第j个像素复制到倒数第i行，倒数第j个像素								
			*lpSrcDIBBits=*lpDstDIBBits;// 将倒数第i行，倒数第j个像素复制到倒数第i行，第j个像素
		}			
	}		
	LocalUnlock(hDstDIBBits);// 释放内存
	LocalFree(hDstDIBBits);
	return TRUE;
}


/*************************************************************************
* 函数名称：Mirror2(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
* 函数参数:
LPSTR	lpSrcStartBits,指向DIB起始像素的指针
long	lWidth，DIB图像的宽度
long	lHeight，DIB图像的高度
long	lLineBytes，DIB图像的行字节数，为4的倍数
* 函数类型:BOOL        
* 函数功能:该函数用来垂直镜像DIB图像         
************************************************************************/

BOOL  Mirror2(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
{	
	long i;                 //行循环变量
	long j;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针 		
	HLOCAL	hDstDIBBits;	//临时图像句柄
	LPSTR	lpBits;	// 指向中间像素的指针，当复制图像时，提供临时的像素内存空间
	hDstDIBBits= LocalAlloc(LHND, lLineBytes);// 分配临时内存保存行图像
	if (hDstDIBBits == NULL)		
	{		
		return FALSE;									// 分配内存失败
	}		
	lpDstDIBBits= (char * )LocalLock(hDstDIBBits);// 锁定				
	for(i = 0; i < lHeight / 2; i++)// 垂直镜像，针对图像每行进行操作
	{			
		//for(j = 0; j < lWidth / 2; j++)// 针对每行图像左半部分进行操作
		//{							
		lpSrcDIBBits= (char *)lpSrcStartBits + lLineBytes * i ;//+ j;// 指向倒数第i行，第j个像素的指针								
		lpBits= (char *)lpSrcStartBits + lLineBytes * (lHeight - i + 1);// - j;// 指向倒数第i+1行，倒数第j个像素的指针								
		memcpy(lpDstDIBBits, lpBits, lLineBytes);
		memcpy(lpBits, lpSrcDIBBits, lLineBytes);
		memcpy(lpSrcDIBBits, lpDstDIBBits, lLineBytes);
		//*lpDstDIBBits=*lpBits;//保存中间像素								
		//*lpBits = *lpSrcDIBBits;// 将倒数第i行，第j个像素复制到倒数第i行，倒数第j个像素								
		//*lpSrcDIBBits=*lpDstDIBBits;// 将倒数第i行，倒数第j个像素复制到倒数第i行，第j个像素
		//}			
	}		
	LocalUnlock(hDstDIBBits);// 释放内存
	LocalFree(hDstDIBBits);
	return TRUE;
}

/*************************************************************************
* 函数名称：Translation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lXOffset, 
long lYOffset,long lLineBytes,long lDstLineBytes)
* 函数参数:
*   LPSTR	lpSrcStartBits，指向源DIB起始像素的指针
* 	 long	lWidth，DIB图像的宽度
* 	 long	lHeight，DIB图像的高度
* 	 long	lXOffset,X方向偏移量
* 	 long	lYOffset,Y方向偏移量
* 	 long	lLineBytes，DIB图像的行字节数，为4的倍数
* 	 long	lDstLineBytes，临时DIB图像的行字节数，为4的倍数
* 函数类型:BOOL        
* 函数功能:该函数用来平移DIB图像
************************************************************************/
BOOL  Translation(LPSTR lpSrcStartBits, long lWidth, long lHeight, 
				  long lXOffset, long lYOffset,long lLineBytes,long lDstLineBytes)					 
{	
	long i;                 //行循环变量
	long j;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针
	LPSTR	lpDstStartBits;	//指向临时图像对应像素的指针 		
	HLOCAL	hDstDIBBits;	//临时图像句柄

	hDstDIBBits= LocalAlloc(LHND, lWidth * lDstLineBytes);// 分配临时内存
	lpDstStartBits= (char * )LocalLock(hDstDIBBits);// 锁定内存	
	if (hDstDIBBits== NULL)// 判断是否内存分配		
		return FALSE;// 分配内存失败				
	for(i = 0; i < lHeight; i++)// 行
	{
		for(j = 0; j < lWidth; j++)	// 列
		{
			lpDstDIBBits=(char*)lpDstStartBits+lLineBytes*(lHeight-1-i)
				+j;// 指向新DIB第i行，第j个像素的指针												
			if( (j-lYOffset>= 0) && (j-lYOffset< lWidth) && // 像素在源DIB中的坐标j-lXOffset
				(i-lXOffset>= 0) && (i-lXOffset < lHeight))// 判断是否在源图范围内
			{
				lpSrcDIBBits=(char *)lpSrcStartBits+lLineBytes*(lHeight-1-
					(i-lXOffset))+(j-lYOffset);// 指向源DIB第i0行，第j0个像素的指针								
				*lpDstDIBBits= *lpSrcDIBBits;// 复制像素
			}
			else
			{				
				* ((unsigned char*)lpDstDIBBits) = 255;// 源图中没有的像素，赋为255
			}			
		}
	}
	memcpy(lpSrcStartBits, lpDstStartBits, lLineBytes * lHeight);// 复制图像		
	LocalUnlock(hDstDIBBits);// 释放内存
	LocalFree(hDstDIBBits);		
	return TRUE;
}

/*************************************************************************
* 函数名称：Rotate(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
long lLineBytes,	WORD palSize, long lDstWidth, 
long lDstHeight,long lDstLineBytes,float	fSina, float fCosa)
* 函数参数:
*   LPSTR lpSrcDib,指向源DIB的指针
*   LPSTR lpSrcStartBits,指向源DIB的起始像素的指针
*   long lWidth,源DIB图像宽度
*   long lHeight,源DIB图像高度
*	 long lLineBytes,源DIB图像字节宽度（4的倍数）
*	 WORD palSize,源DIB图像调色板大小
*	 long lDstWidth,目标图像宽度
*	 long lDstHeight,目标DIB图像高度
*	 long lDstLineBytes,目标DIB图像行字节数（4的倍数）
*	 float	fSina,旋转角的余弦，说明：为了避免两次求取正余弦，这里作为两个函数参数来用
*	 float fCosa,旋转角的正弦
* 函数类型:HGLOBAL
* 函数功能:用来旋转DIB图像
************************************************************************/
HGLOBAL  Rotate(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
				long lLineBytes,	WORD palSize, long lDstWidth, 
				long lDstHeight,long lDstLineBytes,float	fSina, float fCosa)
{	
	float varFloat1;        //浮点参数变量1
	float varFloat2;        //浮点参数变量2
	LPSTR	lpDstDib;		//指向临时图像的指针

	long i;                 //行循环变量
	long j;                 //列循环变量
	long i1;                 //行循环变量
	long j1;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针
	LPSTR	lpDstStartBits;	//指向临时图像对应像素的指针 			

	LPBITMAPINFOHEADER lpbmi;// 指向BITMAPINFOHEADER结构的指针		
	varFloat1= (float) (-0.5 * (lDstWidth - 1) * fCosa - 0.5 * (lDstHeight - 1) * fSina// 将经常用到的两个常数事先求出，以便作为常数使用
		+ 0.5 * (lDstWidth  - 1));
	varFloat2= (float) ( 0.5 * (lDstWidth - 1) * fSina - 0.5 * (lDstHeight - 1) * fCosa
		+ 0.5 * (lDstHeight - 1));		
	HGLOBAL hDIB = (HGLOBAL) ::GlobalAlloc(GHND, lDstLineBytes * lDstHeight + *(LPDWORD)lpSrcDib +palSize);// 分配内存，以保存新DIB		
	if (hDIB == NULL)// 判断是否是有效的DIB对象
	{		
		return FALSE;// 不是，则返回
	}	
	lpDstDib=  (char * )::GlobalLock((HGLOBAL) hDIB);// 锁定内存		
	memcpy(lpDstDib,lpSrcDib, *(LPDWORD)lpSrcDib +palSize);// 复制DIB信息头和调色板			

	lpbmi = (LPBITMAPINFOHEADER)lpDstDib;// 获取指针	
	lpbmi->biHeight=lDstHeight;// 更新DIB中图像的高度和宽度
	lpbmi->biWidth =lDstWidth;

	lpDstStartBits=lpDstDib+ *(LPDWORD)lpDstDib
		+palSize;// 求像素起始位置,作用如同::FindDIBBits(gCo.lpSrcDib)，这里尝试使用了这种方法，以避免对全局函数的调用

	for(i = 0; i < lDstHeight; i++)// 行操作
	{		
		for(j = 0; j < lDstWidth; j++)// 列操作
		{			
			lpDstDIBBits= (char *)lpDstStartBits+ lDstLineBytes * (lDstHeight - 1 - i) + j;// 指向新DIB第i行，第j个像素的指针			
			i1= (long) (-((float) j) * fSina + ((float) i) * fCosa + varFloat2 + 0.5);// 计算该像素在源DIB中的坐标
			j1= (long) ( ((float) j) * fCosa + ((float) i) * fSina + varFloat1 + 0.5);						
			if( (j1>= 0) && (j1< lWidth) && (i1>= 0) && (i1< lHeight))
			{// 判断是否在源图内				
				lpSrcDIBBits= (char *)lpSrcStartBits+ lLineBytes * (lHeight - 1 -i1) + j1;// 指向源DIB第i0行，第j0个像素的指针								
				*lpDstDIBBits= *lpSrcDIBBits;// 复制像素
			}
			else
			{
				* ((unsigned char*)lpDstDIBBits) = 255;// 源图中不存在的像素，赋为255
			}			
		}		
	}
	return hDIB;
}

/*************************************************************************
* 函数名称：Zoom(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
long lLineBytes,	WORD palSize, long lDstWidth, 
long lDstHeight,long lDstLineBytes,float	fXZoomRatio, float fYZoomRatio)
* 函数参数:
*   LPSTR lpSrcDib,指向源DIB的指针
*   LPSTR lpSrcStartBits,指向源DIB的起始像素的指针
*   long lWidth,源DIB图像宽度
*   long lHeight,源DIB图像高度
*	 long lLineBytes,源DIB图像字节宽度（4的倍数）
*	 WORD palSize,源DIB图像调色板大小
*	 long lDstWidth,目标图像宽度
*	 long lDstHeight,目标DIB图像高度
*	 long lDstLineBytes,目标DIB图像行字节数（4的倍数）
*	 float	fhorRatio,水平缩放比率
*	 float fverRatio,垂直缩放比率
* 函数类型:HGLOBAL
* 函数功能:用来缩放DIB图像
************************************************************************/

HGLOBAL  Zoom(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
			  long lLineBytes,	WORD palSize,long lDstWidth,long lDstLineBytes,long lDstHeight,
			  float fhorRatio,float fverRatio)
{			
	LPSTR	lpDstDib;		//指向临时图像的指针	
	long i;                 //行循环变量
	long j;                 //列循环变量
	long i1;                 //行循环变量
	long j1;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针
	LPSTR	lpDstStartBits;	//指向临时图像对应像素的指针 		

	LPBITMAPINFOHEADER lpbmi;// 指向BITMAPINFO结构的指针

	// 分配内存，以保存缩放后的DIB
	HGLOBAL hDIB = (HGLOBAL) ::GlobalAlloc(GHND, lDstLineBytes* lDstHeight + *(LPDWORD)lpSrcDib +palSize);	
	if (hDIB == NULL)// 判断是否是有效的DIB对象
	{		
		return FALSE;// 不是，则返回
	}		
	lpDstDib=  (char * )::GlobalLock((HGLOBAL) hDIB);// 锁定内存		
	memcpy(lpDstDib, lpSrcDib, *(LPDWORD)lpSrcDib +palSize);// 复制DIB信息头和调色板		

	lpDstStartBits=lpDstDib+ *(LPDWORD)lpDstDib// 找到新DIB像素起始位置
		+palSize;// 求像素起始位置,作用如同::FindDIBBits(lpSrcDib)，这里尝试使用了这种方法，以避免对全局函数的调用		
	lpbmi = (LPBITMAPINFOHEADER)lpDstDib;// 获取指针

	lpbmi->biWidth = lDstWidth;// 更新DIB中图像的高度和宽度
	lpbmi->biHeight =lDstHeight;	

	for(i = 0; i < lDstHeight; i++)// 行操作
	{		
		for(j = 0; j < lDstWidth; j++)// 列操作
		{
			// 指向新DIB第i行，第j个像素的指针
			lpDstDIBBits= (char *)lpDstStartBits + lDstLineBytes * (lDstHeight-1-i)+j;						
			i1= (long) (i / fverRatio + 0.5);// 计算该像素在源DIB中的坐标
			j1= (long) (j / fhorRatio + 0.5);			

			if( (j1>= 0) && (j1< lWidth) && (i1>= 0) && (i1< lHeight))
			{// 判断是否在源图内				
				lpSrcDIBBits= (char *)lpSrcStartBits+ lLineBytes * (lHeight - 1 -i1) + j1;// 指向源DIB第i行，第j个像素的指针								
				*lpDstDIBBits= *lpSrcDIBBits;// 复制像素
			}
			else
			{
				* ((unsigned char*)lpDstDIBBits) = 255;// 源图中不存在的像素，赋为255
			}				
		}		
	}
	return hDIB;
}

/*************************************************************************
* 函数名称：FastFourierTran(CplexNum * pTd, CplexNum* pFd, int power)
* 函数参数:
*   CplexNum * pTd,指向时域数组的指针
*   CplexNum * pFd,指向频域数组的指针
*   int             power,2的幂数，即迭代次数
* 函数类型:void 
函数功能:用来实现快速付立叶变换
************************************************************************/
void  FastFourierTran(CplexNum * pTd, CplexNum * pFd, int power)
{	
	long i;                 //行循环变量
	long j;                 //列循环变量

	long	dotCount;// 付立叶变换点数		
	int		k;// 循环变量		
	int		bfsize,p;// 中间变量		
	double	angle;// 角度	
	CplexNum *pWn,*temReg1,*temReg2,*temReg;	

	dotCount= 1 <<power;// 计算付立叶变换点数		
	pWn= new CplexNum[sizeof(CplexNum)*dotCount/ 2];// 分配运算所需存储器
	temReg1 = new CplexNum[sizeof(CplexNum)*dotCount];
	temReg2 = new CplexNum[sizeof(CplexNum)*dotCount];		
	for(i = 0; i < dotCount/ 2; i++)// 计算加权系数
	{
		angle = -i * pi* 2 / dotCount;
		pWn[i].re = cos(angle);
		pWn[i].im=sin(angle);
	}	
	memcpy(temReg1, pTd, sizeof(CplexNum)*dotCount);// 将时域点写入temReg1		
	for(k = 0; k < power; k++)// 采用蝶形算法进行快速付立叶变换
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (power-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				temReg2[i+p]=Add(temReg1[i+p],temReg1[i+p+bfsize/2]);
				temReg2[i+p+bfsize/2]=Mul(Sub(temReg1[i+p],temReg1[i+p+bfsize/2]),
					pWn[i*(1<<k)]);
			}
		}
		temReg  = temReg1;
		temReg1 = temReg2;
		temReg2 = temReg;
	}		
	for(j = 0; j <dotCount; j++)// 重新排序
	{
		p = 0;
		for(i = 0; i <power; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(power-i-1);
			}
		}
		pFd[j]=temReg1[p];
	}		
	delete pWn;// 释放内存
	delete temReg1;
	delete temReg2;
}

/*************************************************************************
*
* 函数名称：InverFastFourierTran(CplexNum * pFd, CplexNum * pTd, int power)
* 函数参数:
*   CplexNum  * pFd，指向频域值的指针
*   CplexNum * pTd，指向时域值的指针
*   int power	，2的幂数
*函数类型:void
*函数功能:用来实现快速付立叶反变换
************************************************************************/
void  InverFastFourierTran(CplexNum * pFd, CplexNum * pTd, int power)
{	
	LONG	dotCount;			// 付立叶变换点数	
	int		i;				// 循环变量
	CplexNum *pX;		
	dotCount= 1 << power;		// 计算付立叶变换点数		
	pX = new CplexNum[sizeof(CplexNum)*dotCount];// 分配运算所需存储器		
	memcpy(pX, pFd, sizeof(CplexNum)*dotCount);// 将频域点写入X		
	for(i = 0; i <dotCount; i++)// 求共轭
	{
		pX[i].re= pX[i].re;
		pX[i].im=-pX[i].im;
	}	
	FastFourierTran(pX, pTd, power);// 调用快速付立叶变换		
	for(i = 0; i <dotCount; i++)// 求时域点的共轭
	{
		pTd[i].re =pTd[i].re/dotCount;
		pTd[i].im=-pTd[i].im/dotCount;

	}		
	delete pX;				// 释放内存
}

/*************************************************************************
* 函数名称：Fourier(CplexNum * pTd, int lWidth, int lHeight, CplexNum * pFd)
* 函数参数:
*   CplexNum * pTd，指向时域值的指针
*   int    lWidth，图像宽度
*   int    lHeight，图像高度
*   CplexNum * pFd	，指向频域值的指针
*
* 函数类型:void
* 函数功能:二维快速傅立叶变换
*************************************************************************/
void  Fourier(CplexNum * pTd, int lWidth, int lHeight, CplexNum * pFd)
{

	// 循环控制变量
	int	j;
	int	i;
	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		wid=1;
	int 	hei=1;
	int		widpor=0,heiPor=0;//2的幂数

	while(wid * 2 <= lWidth)// 计算进行付立叶变换的宽度和高度（2的整数次方）
	{
		wid *= 2;
		widpor++;
	}	
	while(hei * 2 <= lHeight)
	{
		hei *= 2;
		heiPor++;
	}	

	for(i = 0; i < hei; i++)
	{
		// x方向进行快速傅立叶变换
		FastFourierTran(&pTd[wid * i], &pFd[wid * i], widpor);
	}

	// pFd中目前存储了pTd经过行变换的结果
	// 为了直接利用FastFourierTran，需要把pFd的二维数据转置，再一次利用FastFourierTran进行
	// 傅立叶行变换（实际上相当于对列进行傅立叶变换）
	for(i = 0; i < hei; i++)
	{
		for(j = 0; j < wid; j++)
		{
			pTd[hei * j + i] = pFd[wid * i + j];
		}
	}

	for(j = 0; j < wid; j++)
	{
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图像数据进行列方向的
		// 傅立叶变换
		FastFourierTran(&pTd[j * hei], &pFd[j * hei], heiPor);
	}

	// pFd中目前存储了pTd经过二维傅立叶变换的结果，但是为了方便列方向
	// 的傅立叶变换，对其进行了转置，现在把结果转置回来
	for(i = 0; i < hei; i++)
	{
		for(j = 0; j < wid; j++)
		{
			pTd[wid * i + j] = pFd[hei * j + i];
		}
	}

	memcpy(pTd, pFd, sizeof(CplexNum) * hei * wid );
}
/*************************************************************************
* 函数名称：InverseFourier(CplexNum * pFd, CplexNum * pTd, int lWidth, int lHeight)
* 函数参数:
*   CplexNum * pTd，指向时域值的指针
*   CplexNum * pFd	，指向频域值的指针
*   int    lWidth，图像宽度
*   int    lHeight，图像高度
* 函数类型:void
* 函数功能:二维快速傅立叶反变换
*************************************************************************/
void  InverseFourier(CplexNum * pFd, CplexNum * pTd, int lWidth, int lHeight)
{
	// 循环控制变量
	int	j;
	int	i;
	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		wid=1;
	int 	hei=1;
	int		widpor=0,heiPor=0;//2的幂数

	while(wid * 2 <= lWidth)// 计算进行付立叶变换的宽度和高度（2的整数次方）
	{
		wid *= 2;
		widpor++;
	}	
	while(hei * 2 <= lHeight)
	{
		hei *= 2;
		heiPor++;
	}		
	// 分配工作需要的内存空间
	CplexNum *pCWork= new CplexNum[sizeof(CplexNum)*wid * hei];

	//临时变量
	CplexNum *pCTmp ;

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for(i = 0; i < hei; i++)
	{
		for(j = 0; j < wid; j++)
		{
			pCTmp = &pFd[wid * i + j] ;
			pCWork[wid * i + j].re=pCTmp->re;
			pCWork[wid * i + j].im=-pCTmp->im;
			//= complex<double>( pCTmp->real() , -pCTmp->imag() );
		}
	}

	// 调用傅立叶正变换
	Fourier(pCWork, lWidth, lHeight, pTd) ;

	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数
	for(i = 0; i < hei; i++)
	{
		for(j = 0; j < wid; j++)
		{
			pCTmp = &pTd[wid * i + j];
			pTd[wid * i + j].re=pCTmp->re/(wid*hei);
			pTd[wid * i + j].im=-pCTmp->im/(wid*hei);			

		}
	}
	delete pCWork ;
}

/*************************************************************************
* 函数名称：DisFCosTran(double *pTd, double *pFd, int power)
* 函数参数:
*   double * pTd,指向时域值的指针
*   double * pFd,指向频域值的指针
*   int    power,2的幂数
*
* 函数类型:void
* 函数功能: 用来实现快速离散余弦变换
************************************************************************/
void  DisFCosTran(double *pTd, double *pFd, int power)
{	
	long i;                 //行循环变量				
	long	dotCount;// 离散余弦变换点数			
	double	dTemp;// 临时变量	
	CplexNum *temReg;		
	dotCount = 1<<power;// 计算离散余弦变换点数		
	temReg = new CplexNum[sizeof(CplexNum) *dotCount*2];// 分配内存		
	memset(temReg, 0, sizeof(CplexNum) * dotCount * 2);// 赋为0		
	for(i=0;i<dotCount;i++)// 将时域点写入数组temReg
	{
		temReg[i].re=pTd[i];
		temReg[i].im=0;
	}		
	FastFourierTran(temReg,temReg,power+1);// 调用快速付立叶变换		
	dTemp = 1/sqrt((double)dotCount);// 调整系数		
	pFd[0] = temReg[0].re*dTemp;// 求pFd[0]	
	dTemp *= sqrt(2.0f);		
	for(i = 1; i < dotCount; i++)// 求pFd[u]	
	{
		pFd[i]=(temReg[i].re* cos(i*pi/(dotCount*2)) + temReg[i].im* sin(i*pi/(dotCount*2))) * dTemp;
	}		
	delete temReg;// 释放内存
}

/*************************************************************************
* 函数名称：CosTran(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
* 函数参数:
*   LPSTR lpSrcStartBits,指向DIB起始像素的指针
*   long lWidth,DIB的宽度
*   long lHeight,DIB的高度
*	 long lLineBytes,DIB的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能: 用来对图像进行离散余弦变换
************************************************************************/
BOOL  CosTran(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
{		
	unsigned char*	lpSrcUnChr;	//指向像素的指针
	long i;                 //行循环变量
	long j;                 //列循环变量 			
	long	wid=1,hei=1;// 进行付立叶变换的宽度和高度，初始化为1
	double	dTemp;// 中间变量	
	int		widpor=0,heiPor=0;//2的幂数

	while(wid * 2 <= lWidth)// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	{
		wid *= 2;
		widpor++;
	}	
	while(hei * 2 <= lHeight)
	{
		hei *= 2;
		heiPor++;
	}		
	double *pTd= new double[wid * hei];// 分配内存
	double *pFd = new double[wid * hei];		
	for(i = 0; i < hei; i++)// 行
	{		
		for(j = 0; j < wid; j++)// 列
		{
			// 指向DIB第i行，第j个像素的指针
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight- 1 - i) + j;						
			pTd[j + i * wid] = *(lpSrcUnChr);// 给时域赋值
		}
	}	
	for(i = 0; i < hei; i++)
	{		
		DisFCosTran(&pTd[wid * i], &pFd[wid * i], widpor);// 对y方向进行离散余弦变换
	}		
	for(i = 0; i < hei; i++)// 保存计算结果
	{
		for(j = 0; j < wid; j++)
		{
			pTd[j * hei + i] = pFd[j + wid * i];
		}
	}	
	for(j = 0; j < wid; j++)
	{		
		DisFCosTran(&pTd[j * hei], &pFd[j * hei], heiPor);// 对x方向进行离散余弦变换
	}		
	for(i = 0; i < hei; i++)// 行
	{		
		for(j = 0; j < wid; j++)// 列
		{			
			dTemp = fabs(pFd[j*hei+i]);// 计算频谱


			if (dTemp > 255)// 是否超过255,超过的，直接设置为255
			{
				dTemp = 255;
			}			
			// 指向DIB第y行，第x个像素的指针
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight- 1 - i) + j;						
			* (lpSrcUnChr) = (BYTE)(dTemp);// 更新源图像
		}
	}		
	delete pTd;// 释放内存
	delete pFd;	
	return TRUE;
}

/*************************************************************************
* 函数名称：Walshei_Har(double *pTd, double *pFd, int power)
* 函数参数:
*   double * pTd,指向时域值的指针
*   double * pFd,指向频域值的指针
*   int    power,2的幂数
*
* 函数类型:void
* 函数功能: 用来实现快速沃尔什-哈达玛变换
************************************************************************/

void  Walshei_Har(double *pTd, double *pFd, int power)
{	
	long	dotCount;// 沃尔什-哈达玛变换点数		
	int		i,j,k;// 循环变量	
	int		bfsize,p;// 中间变量	
	double *temReg1,*temReg2,*temReg;		
	dotCount = 1 << power;// 计算快速沃尔什变换点数		
	temReg1 = new double[dotCount];// 分配运算所需的数组
	temReg2 = new double[dotCount];
	memcpy(temReg1, pTd, sizeof(double) * dotCount);// 将时域点写入数组temReg1		
	for(k = 0; k < power; k++)// 蝶形运算
	{
		for(j = 0; j < 1<<k; j++)
		{
			bfsize = 1 << (power-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				temReg2[i + p] = temReg1[i + p] + temReg1[i + p + bfsize / 2];
				temReg2[i + p + bfsize / 2] = temReg1[i + p] - temReg1[i + p + bfsize / 2];
			}
		}				
		temReg = temReg1;// 互换temReg1和temReg2  
		temReg1 = temReg2;
		temReg2 = temReg;
	}	
	for(j = 0; j < dotCount; j++)// 调整系数
	{
		p = 0;
		for(i = 0; i < power; i++)
		{
			if (j & (1<<i))
			{
				p += 1 << (power-i-1);
			}
		}
		pFd[j] = temReg1[p] / dotCount;
	}		
	delete temReg1;// 释放内存
	delete temReg2;
}

/*************************************************************************
* 函数名称：Walsh_HarTran(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
* 函数参数:
*   LPSTR lpSrcStartBits,指向源DIB图像指针
*   long  lWidth,源DIB图像宽度
*   long  lHeight,源DIB图像高度
*   long lLineBytes,源DIB图像的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:用来对图像进行沃尔什-哈达玛变换
************************************************************************/

BOOL  Walsh_HarTran(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
{
	unsigned char*	lpSrcUnChr;	//指向像素的指针	
	long i;                 //行循环变量
	long j;                 //列循环变量		

	long	wid=1,hei=1;// 进行付立叶变换的宽度和高度，初始化为1
	double	dTemp;// 中间变量	
	int		widpor=0,heiPor=0;//2的幂数

	while(wid * 2 <= lWidth)// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	{
		wid *= 2;
		widpor++;
	}	
	while(hei * 2 <= lHeight)
	{
		hei *= 2;
		heiPor++;
	}		
	double *pTd = new double[wid * hei];// 分配内存
	double *pFd = new double[wid * hei];		
	for(i = 0; i < hei; i++)// 行
	{

		for(j = 0; j < wid; j++)// 列
		{
			// 指向DIB第i行，第j个像素的指针
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight- 1 - i) + j;						
			pTd[j + i * wid] = *(lpSrcUnChr);// 给时域赋值
		}
	}	
	for(i = 0; i < hei; i++)
	{		
		Walshei_Har(pTd + wid * i, pFd + wid * i, widpor);// 对y方向进行沃尔什-哈达玛变换
	}		
	for(i = 0; i < hei; i++)// 保存计算结果
	{
		for(j = 0; j < wid; j++)
		{
			pTd[j * hei + i] = pFd[j + wid * i];
		}
	}	
	for(j = 0; j < wid; j++)
	{		
		Walshei_Har(pTd + j * hei, pFd+ j * hei, heiPor);// 对x方向进行沃尔什-哈达玛变换
	}		
	for(i = 0; i < hei; i++)// 行
	{		
		for(j = 0; j < wid; j++)// 列
		{			
			dTemp = fabs(pFd[j * hei + i] * 1000);// 计算频谱						
			if (dTemp > 255)// 对于超过255的，直接设置为255
			{
				dTemp = 255;
			}			
			// 指向DIB第i行，第j个像素的指针
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight- 1 - i) + j;						
			* (lpSrcUnChr) = (BYTE)(dTemp);// 更新源图像
		}
	}		
	delete pTd;//释放内存
	delete pFd;
	return TRUE;
}

/*************************************************************************
*
* 函数名称：
*   DisK_L(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
*函数参数:
*   LPSTR lpSrcStartBits,指向源DIB图像指针
*   long  lWidth,源DIB图像宽度
*   long  lHeight,源DIB图像高度
*   long lLineBytes,源DIB图像的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:用来对图像进行旋转
************************************************************************/

BOOL DisK_L(LPSTR lpSrcStartBits, long lWidth, long lHeight,long lLineBytes)
{
	unsigned char*	lpSrcUnChr;	//指向像素的指针	
	LONG	i,j,// 循环变量
		lMaxRange,// 经过变换后图像最大可能范围		
		AverEx,AverEy,//  目标坐标均值		
		ToaCount;//  目标总的像素数	
	double  Matr4C[2][2],// 坐标值的协方差矩阵
		QMatrix[2][2],// 存放协方差矩阵的特征向量		
		MainCross[2],HypoCross[2],//  三对角阵的主对角和次对角线元素		
		dTemp;// 临时变量
	LONG    lTempI,lTempJ;

	if(lWidth>lHeight)// 估计图像经过旋转后可能最大的宽度和高度
	{
		lMaxRange = lWidth;
	}
	else
	{
		lMaxRange =lHeight;
	}

	AverEx=0.0;// 初始化
	AverEy=0.0;
	ToaCount = 0;
	Matr4C[0][0] = Matr4C[0][1] = Matr4C[1][0] = Matr4C[1][1] = 0.0;

	double *F = new double[lWidth*lHeight];// 分配内存		
	for(i = 0; i < lHeight; i++)// 行
	{		
		for(j = 0; j < lWidth; j++)// 列
		{			
			F[i*lWidth + j] = 255;// 给旋转后坐标轴的每个点赋零值
			// 指向位图i行j列像素的指针
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes*i + j;

			// 值小于255（非背景色白色）的像素认为目标的一部分
			// 并将其坐标值x和y看作二维随机矢量
			if((*lpSrcUnChr) < 255)
			{				

				AverEx=AverEx+i;// 属于目标像素的Y坐标和X坐标累计值
				AverEy=AverEy+j;				
				ToaCount++;// 目标总的像素数加一

				// 随机矢量协方差矩阵的累计值
				Matr4C[0][0] = Matr4C[0][0] + i*i;
				Matr4C[0][1] = Matr4C[0][1] + i*j;
				Matr4C[1][0] = Matr4C[1][0] + j*i;
				Matr4C[1][1] = Matr4C[1][1] + j*j;
			}
		}
	}

	AverEx = AverEx/ToaCount;// 计算随机矢量的均值
	AverEy = AverEy/ToaCount;

	Matr4C[0][0] = Matr4C[0][0]/ToaCount - AverEx*AverEx;//  计算随机矢量的协方差矩阵
	Matr4C[0][1] = Matr4C[0][1]/ToaCount - AverEx*AverEy;
	Matr4C[1][0] = Matr4C[1][0]/ToaCount - AverEx*AverEy;
	Matr4C[1][1] = Matr4C[1][1]/ToaCount - AverEy*AverEy;

	double Precision = 0.000001;// 规定迭代的计算精度

	ThreeCrossMat(*Matr4C, 2, *QMatrix, MainCross, HypoCross);// 将协方差矩阵化作三对角对称阵		
	EigenvalueVector(2, MainCross,HypoCross, *Matr4C, Precision, 50);// 求协方差矩阵的特征值和特征矢向量

	dTemp = Matr4C[0][1];// 将特征列向量转化称特征列向量
	Matr4C[0][1] = Matr4C[1][0];
	Matr4C[1][0] = dTemp;

	for(i=0;i<=1;i++)
	{// 对特征列向量进行归一化
		dTemp = pow(Matr4C[i][0],2) + pow(Matr4C[i][1],2);
		dTemp = sqrt(dTemp);
		Matr4C[i][0] = Matr4C[i][0]/dTemp;
		Matr4C[i][1] = Matr4C[i][1]/dTemp;
	}	
	// 查找经离散K-L变换后的坐标点在原坐标系中的坐标    
	for(i = -lMaxRange+1; i < lMaxRange; i++)
	{
		for(j = -lMaxRange+1; j < lMaxRange; j++)
		{
			//  将新坐标值映射到旧的坐标系
			int Cx = (int)(i*Matr4C[0][0]-j*Matr4C[0][1])+AverEx;
			int Cy = (int)(-i*Matr4C[1][0]+j*Matr4C[1][1])+AverEy;

			//  映射值是否属于源图像
			if( Cx>=0 && Cx<lHeight && Cy>=0 && Cy<lWidth )
			{
				lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes*Cx + Cy;

				// 映射值是否属于原来的目标
				if(*(lpSrcUnChr)<255)
				{
					//  将新坐标系原点平移到中心，以便显示
					lTempI=(LONG)(lHeight/2)+j;
					lTempJ=(LONG)(lWidth/2)+i;

					// 看如果能够进行显示，赋值给数组，进行存储
					if( lTempI>=0 && lTempI<lHeight && lTempJ>=0 && lTempJ<lWidth )
						F[lTempJ+ (lTempI) * lWidth]=*(lpSrcUnChr);
				}

			}		
		}
	}

	for(i = 0; i < lMaxRange; i++)// 行
	{		
		for(j = 0; j < lMaxRange; j++)// 列
		{			
			dTemp = F[i * lMaxRange + j] ;// 离散K-L变换后的像素值			
			// 指向位图i行j列像素的指针
			lpSrcUnChr= (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;						
			* (lpSrcUnChr) = (BYTE)(dTemp);// 更新源图像
		}
	}		
	delete F;// 释放内存	
	return TRUE;// 返回
}





/*************************************************************************
*
* \函数名称：
*   FFT_1D()
*
* \输入参数:
*   complex<double> * pCTData	- 指向时域数据的指针，输入的需要变换的数据
*   complex<double> * pCFData	- 指向频域数据的指针，输出的经过变换的数据
*   nLevel						－傅立叶变换蝶形算法的级数，2的幂数，
*
* \返回值:
*   无
*
* \说明:
*   一维快速傅立叶变换。
*
*************************************************************************
*/
void FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel)
{
	// 循环控制变量
	int		i;
	int     j;
	int     k;

	// 傅立叶变换点数
	int	nCount =0 ;

	// 计算傅立叶变换点数
	nCount =(int)pow((double)2,(double)nLevel) ;

	// 某一级的长度
	int		nBtFlyLen;
	nBtFlyLen = 0 ;

	// 变换系数的角度 ＝2 * PI * i / nCount
	double	dAngle;

	complex<double> *pCW ;

	// 分配内存，存储傅立叶变化需要的系数表
	pCW  = new complex<double>[nCount / 2];

	// 计算傅立叶变换的系数
	for(i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * PI * i / nCount;
		pCW[i] = complex<double> ( cos(dAngle), sin(dAngle) );
	}

	// 变换需要的工作空间
	complex<double> *pCWork1,*pCWork2; 

	// 分配工作空间
	pCWork1 = new complex<double>[nCount];

	pCWork2 = new complex<double>[nCount];


	// 临时变量
	complex<double> *pCTmp;

	// 初始化，写入数据
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount);

	// 临时变量
	int nInter; 
	nInter = 0;

	// 蝶形算法进行快速傅立叶变换
	for(k = 0; k < nLevel; k++)
	{
		for(j = 0; j < (int)pow((double)2,(double)k); j++)
		{
			//计算长度
			nBtFlyLen = (int)pow( (double)2,(double)(nLevel-k) );

			//倒序重排，加权计算
			for(i = 0; i < nBtFlyLen/2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] = 
					pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2]) 
					* pCW[(int)(i * pow((double)2,(double)k))];
			}
		}

		// 交换 pCWork1和pCWork2的数据
		pCTmp   = pCWork1	;
		pCWork1 = pCWork2	;
		pCWork2 = pCTmp		;
	}

	// 重新排序
	for(j = 0; j < nCount; j++)
	{
		nInter = 0;
		for(i = 0; i < nLevel; i++)
		{
			if ( j&(1<<i) )
			{
				nInter += 1<<(nLevel-i-1);
			}
		}
		pCFData[j]=pCWork1[nInter];
	}

	// 释放内存空间
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW		=	NULL	;
	pCWork1 =	NULL	;
	pCWork2 =	NULL	;

}
/*************************************************************************
*
* \函数名称：
*    IFFT_1D()
*
* \输入参数:
*   complex<double> * pCTData	- 指向时域数据的指针，输入的需要反变换的数据
*   complex<double> * pCFData	- 指向频域数据的指针，输出的经过反变换的数据
*   nLevel						－傅立叶变换蝶形算法的级数，2的幂数，
*
* \返回值:
*   无
*
* \说明:
*   一维快速傅立叶反变换。
*
************************************************************************
*/
void IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel)
{

	// 循环控制变量
	int		i;

	// 傅立叶反变换点数
	int nCount;

	// 计算傅立叶变换点数
	nCount = (int)pow((double)2,(double)nLevel) ;

	// 变换需要的工作空间
	complex<double> *pCWork;	

	// 分配工作空间
	pCWork = new complex<double>[nCount];

	// 将需要反变换的数据写入工作空间pCWork
	memcpy(pCWork, pCFData, sizeof(complex<double>) * nCount);

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for(i = 0; i < nCount; i++)
	{
		pCWork[i] = complex<double> (pCWork[i].real(), -pCWork[i].imag());
	}

	// 调用快速傅立叶变换实现反变换，结果存储在pCTData中
	FFT_1D(pCWork, pCTData, nLevel);

	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数nCount
	for(i = 0; i < nCount; i++)
	{
		pCTData[i] = 
			complex<double> (pCTData[i].real() / nCount, -pCTData[i].imag() / nCount);
	}

	// 释放内存
	delete pCWork;
	pCWork = NULL;
}

/*************************************************************************
*
* \函数名称：
*   FFT_2D()
*
* \输入参数:
*   complex<double> * pCTData	- 图像数据
*   int    nWidth				- 数据宽度
*   int    nHeight				- 数据高度
*   complex<double> * pCFData	- 傅立叶变换后的结果
*
* \返回值:
*   无
*
* \说明:
*   二维傅立叶变换。
*
************************************************************************
*/
void FFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{

	// 循环控制变量
	int	x;
	int	y;

	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;

	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
	int 	nTransHeight;

	// 计算进行傅立叶变换的宽度	（2的整数次幂）
	dTmpOne = log((double)nWidth)/log((double)2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;

	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log((double)nHeight)/log((double)2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;	

	// x，y（行列）方向上的迭代次数
	int		nXLev;
	int		nYLev;

	// 计算x，y（行列）方向上的迭代次数
	nXLev = (int) ( log((double)nTransWidth)/log((double)2) +  0.5 );
	nYLev = (int) ( log((double)nTransHeight)/log((double)2) + 0.5 );

	for(y = 0; y < nTransHeight; y++)
	{
		// x方向进行快速傅立叶变换
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}

	// pCFData中目前存储了pCTData经过行变换的结果
	// 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行
	// 傅立叶行变换（实际上相当于对列进行傅立叶变换）
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}

	for(x = 0; x < nTransWidth; x++)
	{
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图像数据进行列方向的
		// 傅立叶变换
		FFT_1D(&pCTData[x * nTransHeight], &pCFData[x * nTransHeight], nYLev);
	}

	// pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向
	// 的傅立叶变换，对其进行了转置，现在把结果转置回来
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y];
		}
	}

	memcpy(pCTData, pCFData, sizeof(complex<double>) * nTransHeight * nTransWidth );
}

/*************************************************************************
*
* \函数名称：
*   IFFT_2D()
*
* \输入参数:
*   complex<double> * pCFData	- 频域数据
*   complex<double> * pCTData	- 时域数据
*   int    nWidth				- 图像数据宽度
*   int    nHeight				- 图像数据高度
*
* \返回值:
*   无
*
* \说明:
*   二维傅立叶反变换。
*
************************************************************************
*/
void IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{
	// 循环控制变量
	int	x;
	int	y;

	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;

	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
	int 	nTransHeight;

	// 计算进行傅立叶变换的宽度	（2的整数次幂）
	dTmpOne = log((double)nWidth)/log((double)2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;

	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log((double)nHeight)/log((double)2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow((double)2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;

	// 分配工作需要的内存空间
	complex<double> *pCWork= new complex<double>[nTransWidth * nTransHeight];

	//临时变量
	complex<double> *pCTmp ;

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x] ;
			pCWork[nTransWidth * y + x] = complex<double>( pCTmp->real() , -pCTmp->imag() );
		}
	}

	// 调用傅立叶正变换
	FFT_2D(pCWork, nWidth, nHeight, pCTData) ;

	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCTData[nTransWidth * y + x] ;
			pCTData[nTransWidth * y + x] = 
				complex<double>( pCTmp->real()/(nTransWidth*nTransHeight),
				-pCTmp->imag()/(nTransWidth*nTransHeight) );
		}
	}
	delete pCWork ;
	pCWork = NULL ;
}




/////////////////////////////////////////////////
const double hCoef[10][20] =
{
	{ .707106781187,  .707106781187},

	{ .482962913145,  .836516303738,  .224143868042, -.129409522551 },

	{ .332670552950,  .806891509311,  .459877502118, -.135011020010, -.085441273882,  .035226291882 },

	{ .230377813309,  .714846570553,  .630880767930, -.027983769417,
	-.187034811719,  .030841381836,  .032883011667, -.010597401785 },

	{ .160102397974,  .603829269797,  .724308528438,  .138428145901, -.242294887066,
	-.032244869585,  .077571493840, -.006241490213, -.012580751999,  .003335725285 },

	{ .111540743350,  .494623890398,  .751133908021,  .315250351709, -.226264693965,
	-.129766867567,  .097501605587,  .027522865530, -.031582039318,  .000553842201,
	.004777257511, -.001077301085 },

	{ .077852054085,  .396539319482,  .729132090846,  .469782287405, -.143906003929,
	-.224036184994,  .071309219267,  .080612609151, -.038029936935, -.016574541631,
	.012550998556,  .000429577973, -.001801640704,  .000353713800 },

	{ .054415842243,  .312871590914,  .675630736297,  .585354683654, -.015829105256,
	-.284015542962,  .000472484574,  .128747426620, -.017369301002, -.044088253931,
	.013981027917,  .008746094047, -.004870352993, -.000391740373,  .000675449406,
	-.000117476784 },

	{ .038077947364,  .243834674613,  .604823123690,  .657288078051,  .133197385825,
	-.293273783279, -.096840783223,  .148540749338,  .030725681479, -.067632829061,
	.000250947115,  .022361662124, -.004723204758, -.004281503682,  .001847646883,
	.000230385764, -.000251963189,  .000039347320 },

	{ .026670057901,  .188176800078,  .527201188932,  .688459039454,  .281172343661,
	-.249846424327, -.195946274377,  .127369340336,  .093057364604, -.071394147166,
	-.029457536822,  .033212674059,  .003606553567, -.010733175483,  .001395351747,
	.001992405295, -.000685856695, -.000116466855,  .000093588670, -.000013264203 }
};
/*************************************************************************
*
* \函数名称：
*   DWTStep_1D()
*
* \输入参数:
*   double * pDbSrc		- 指向源数据的指针
*   int nCurLevel		- 当前分界的层数
*   int nInv			- 是否为DWT，1表示为IDWT，0表示DWT
*   int nStep			- 当前的计算层数
*   int nSupp			- 小波基的紧支集的长度
*
* \返回值:
*   BOOL			- 成功则返回TRUE，否则返回FALSE
*
* \说明:
*   该函数用对存放在pDBSrc中的数据进行一层的一维DWT或者IDWT。其中，nInv为表示进行
*   DWT或者IDWT的标志。nCurLevel为当前需要进行分界的层数。nStep为已经分界的层数
*   计算后数据仍存放在pDbSrc中
*
*************************************************************************
*/
BOOL DWTStep_1D(double* pDbSrc, int nCurLevel,
				int nInv, int nStep,int nSupp)
{
	double s = sqrt((double)2);
	// 获得小波基的指针
	double* h = (double*)hCoef[nSupp-1];
	// 确认当前层数有效
	ASSERT(nCurLevel>=0);

	// 计算当前层数的长度
	int CurN = 1<<nCurLevel;
	if (nInv) CurN <<= 1;

	// 确认所选择的小波基和当前层数的长度有效
	if (nSupp<1 || nSupp>10 || CurN<2*nSupp) 
		return FALSE;

	// 分配临时内存用于存放结果
	double *ptemp = new double[CurN];
	if (!ptemp) return FALSE;

	double	s1, s2;
	int	Index1, Index2;

	// 判断是进行DWT还是IDWT
	if (!nInv)
	{	// DWT
		Index1=0;
		Index2=2*nSupp-1;

		// 进行卷积，其中s1为低频部分，s2为高频部分的结果
		for (int i=0; i<CurN/2; i++)
		{	
			s1 = s2 = 0;
			double t = -1;
			for (int j=0; j<2*nSupp; j++, t=-t)
			{
				s1 += h[j]*pDbSrc[(Index1 & CurN-1) * nStep];
				s2 += t*h[j]*pDbSrc[(Index2 & CurN-1) * nStep];

				Index1++;
				Index2--;
			}

			// 将结果存放在临时内存中
			ptemp[i] = s1/s;
			ptemp[i+CurN/2] = s2/s;

			Index1 -= 2*nSupp;
			Index2 += 2*nSupp;
			Index1 += 2;
			Index2 += 2;
		}
	}

	// 否则进行IDWT
	else
	{	// IDWT
		Index1 = CurN/2;
		Index2 = CurN/2-nSupp+1;

		// 进行卷积，其中其中s1为低频部分，s2为高频部分的结果
		for (int i=0; i<CurN/2; i++)
		{
			s1 = s2 = 0;
			int Index3 = 0;
			for (int j=0; j<nSupp; j++)
			{
				s1 += h[Index3]*pDbSrc[(Index1 & CurN/2-1) * nStep]
				+h[Index3+1]*pDbSrc[((Index2 & CurN/2-1) + CurN/2) * nStep];
				s2 += h[Index3+1]*pDbSrc[(Index1 & CurN/2-1) * nStep]
				-h[Index3]*pDbSrc[((Index2 & CurN/2-1) + CurN/2) * nStep];

				Index3+=2;
				Index1--,		Index2++;
			}

			// 将结果存入临时内存
			ptemp[2*i] = s1*s;
			ptemp[2*i+1] = s2*s;

			Index1 += nSupp;
			Index2 -= nSupp;
			Index1++;
			Index2++;
		}
	}

	// 将结果存入源图像中
	for (int i=0; i<CurN; i++)
		pDbSrc[i*nStep] = ptemp[i];

	// 释放临时内存，并返回
	delete[] ptemp;
	return TRUE;
}

/*************************************************************************
*
* \函数名称：
*   DWTStep_2D()
*
* \输入参数:
*   double * pDbSrc		- 指向源数据的指针
*   int nCurWLevel		- X方向上当前分解的层数
*   int nCurHLevel		- Y方向上当前分解的层数
*   int nMaxWLevel		- X方向上最大可分解的层数
*   int nMaxHLevel		- Y方向上最大可分解的层数
*   int nInv			- 是否为DWT，1表示为IDWT，0表示DWT
*   int nStep			- 当前的计算层数
*   int nSupp			- 小波基的紧支集的长度
*
* \返回值:
*   BOOL			- 成功则返回TRUE，否则返回FALSE
*
* \说明:
*   该函数用对存放在pDBSrc中的数据进行一层的二维DWT或者IDWT。
*   计算后数据仍存放在pDbSrc中
*
*************************************************************************
*/
BOOL DWTStep_2D(double* pDbSrc, int nCurWLevel, int nCurHLevel,
				int nMaxWLevel, int nMaxHLevel, int nInv, int nStep, int nSupp)
{
	// 计算图像的长度和宽度（2次幂对齐）
	int W = 1<<nMaxWLevel, H = 1<<nMaxHLevel;

	// 计算当前分解的图像的长度和宽度
	int CurW = 1<<nCurWLevel, CurH = 1<<nCurHLevel;

	// 判断是进行DWT还是IDWT
	if (!nInv)
	{	
		int i = 0;
		// 对行进行一维DWT
		for (i=0; i<CurH; i++)
			if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, nInv, nStep, nSupp)) return FALSE;
		// 对列进行一维DWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, nInv, W*nStep, nSupp)) return FALSE;
	}

	// 否则进行IDWT
	else
	{
		// 计算当前变换的图像的长度和宽度
		CurW <<= 1;
		CurH <<= 1;

		int i = 0;
		// 对列进行IDWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, nInv, W*nStep, nSupp)) return FALSE;
		// 对行进行IDWT
		for (i=0; i<CurH; i++)
			if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, nInv, nStep, nSupp)) return FALSE;
	}

	// 返回
	return TRUE;
}


/*************************************************************************
*
* \函数名称：
*   FloatToByte()
*
* \输入参数:
*   double  f			- 输入双精度变量
*
* \返回值:
*   BYTE			- 返回比特型变量
*
* \说明:
*   该函数将输入的双精度变量转换为BYTE型的变量
*
*************************************************************************
*/
BYTE FloatToByte(double f)
{
	if (f<=0) return (BYTE)0;
	else if (f>=255) return (BYTE)255;
	else return (BYTE)(f+0.5);
}

/*************************************************************************
*
* \函数名称：
*   FloatToChar()
*
* \输入参数:
*   double  f			- 输入双精度变量
*
* \返回值:
*   Char			- 返回字符变量
*
* \说明:
*   该函数将输入的双精度变量转换为Char型的变量
*
*************************************************************************
*/
char FloatToChar(double f)
{
	if (f>=0)
		if (f>=127.0)
			return (char)127;
		else return (char)(f+0.5);
	else
		if (f<=-128)
			return (char)-128;
		else return -(char)(-f+0.5);
}

/*************************************************************************
*
* \函数名称：
*   Log2()
*
* \输入参数:
*   int  n			- 输入整型变量
*
* \返回值:
*   int			- 返回输入参数的对数
*
* \说明:
*   该函数求取输入参数的以2为底的对数，并转换为整型输出。
*
*************************************************************************
*/
int Log2(int n)
{
	int rsl = 0;
	while (n >>= 1) rsl++;
	return rsl;
}




BOOL DIBDWTStep(LPSTR lpDIBBits,double*m_pDbImage, int nWidth,int nHeight, int nInv,int m_nDWTCurDepth,int m_nSupp)
{
	// 循环变量
	int i, j;

	// 获取变换的最大层数
	int nMaxWLevel = Log2(nWidth);
	int nMaxHLevel = Log2(nHeight);
	int nMaxLevel;
	if (nWidth == 1<<nMaxWLevel && nHeight == 1<<nMaxHLevel)
		nMaxLevel = min(nMaxWLevel, nMaxHLevel);

	// 临时变量
	double	*pDbTemp;
	BYTE	*pBits;

	// 如果小波变换的存储内存还没有分配，则分配此内存
	if(!m_pDbImage){			
		m_pDbImage = new double[nWidth*nHeight];
		if (!m_pDbImage)	return FALSE;

		// 将图像数据放入m_pDbImage中 
		for (j=0; j<nHeight; j++)
		{
			pDbTemp = m_pDbImage + j*nWidth;
			pBits = (unsigned char *)lpDIBBits + (nHeight-1-j)*nWidth;		
			for (i=0; i<nWidth; i++)
				pDbTemp[i] = pBits[i];
		}
	}

	// 进行小波变换（或反变换）
	if (!DWTStep_2D(m_pDbImage, nMaxWLevel-m_nDWTCurDepth, nMaxHLevel-m_nDWTCurDepth,
		nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp))
		return FALSE;

	// 如果是反变换，则当前层数减1
	if (nInv)
		m_nDWTCurDepth --;
	// 否则加1
	else
		m_nDWTCurDepth ++;

	// 然后，将数据拷贝回原CDib中，并进行相应的数据转换
	int lfw = nWidth>>m_nDWTCurDepth, lfh = nHeight>>m_nDWTCurDepth;
	for (j=0; j<nHeight; j++)
	{
		pDbTemp = m_pDbImage + j*nWidth;
		pBits = (unsigned char *)lpDIBBits + (nHeight-1-j)*nWidth;
		for (i=0; i<nWidth; i++)
		{
			if (j<lfh && i<lfw)
				pBits[i] = FloatToByte(pDbTemp[i]);
			else
				pBits[i] = BYTE(FloatToChar(pDbTemp[i]) ^ 0x80);			
		}
	}

	// 返回
	return TRUE;
}


/*************************************************************************
* 函数名称：
*   Interpolation()
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（像素数）
*   LONG  lHeight      - 源图像高度（像素数）
*   FLOAT x			- 插值元素的x坐标
*   FLOAT y		    - 插值元素的y坐标
* 返回值:
*   unsigned char      - 返回插值计算结果。
* 说明:
*   该函数利用双线性插值算法来估算像素值。对于超出图像范围的像素，
* 直接返回255。
************************************************************************/
unsigned char Interpolation (LPSTR lpDIBBits, LONG lWidth, 
							 LONG lHeight, FLOAT x, FLOAT y)
{
	// 四个最临近像素的坐标(i1, j1), (i2, j1), (i1, j2), (i2, j2)
	LONG	i1, i2;
	LONG	j1, j2;

	unsigned char	f1, f2, f3, f4;	// 四个最临近像素值	
	unsigned char	f12, f34;		// 二个插值中间值	

	// 定义一个值，当像素坐标相差小于改值时认为坐标相同
	FLOAT			EXP;	

	LONG lLineBytes;				// 图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	EXP = (FLOAT) 0.0001;

	// 计算四个最临近像素的坐标
	i1 = (LONG) x;
	i2 = i1 + 1;
	j1 = (LONG) y;
	j2 = j1 + 1;

	// 根据不同情况分别处理
	if( (x < 0) || (x > lWidth - 1) || (y < 0) || (y > lHeight - 1))
	{		
		return 255;		// 要计算的点不在源图范围内，直接返回255。
	}
	else
	{
		if (fabs(x - lWidth + 1) <= EXP)
		{
			// 要计算的点在图像右边缘上
			if (fabs(y - lHeight + 1) <= EXP)
			{
				// 要计算的点正好是图像最右下角那一个像素，直接返回该点像素值
				f1 = *((unsigned char *)lpDIBBits + lLineBytes * 
					(lHeight - 1 - j1) + i1);
				return f1;
			}
			else
			{
				// 在图像右边缘上且不是最后一点，直接一次插值即可
				f1 = *((unsigned char *)lpDIBBits + lLineBytes * 
					(lHeight - 1 - j1) + i1);
				f3 = *((unsigned char *)lpDIBBits + lLineBytes * 
					(lHeight - 1 - j1) + i2);

				// 返回插值结果
				return ((unsigned char) (f1 + (y -j1) * (f3 - f1)));
			}
		}
		else if (fabs(y - lHeight + 1) <= EXP)
		{
			// 要计算的点在图像下边缘上且不是最后一点，直接一次插值即可
			f1 = *((unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
			f2 = *((unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);

			// 返回插值结果
			return ((unsigned char) (f1 + (x -i1) * (f2 - f1)));
		}
		else
		{
			// 计算四个最临近像素值
			f1 = *((unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
			f2 = *((unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);
			f3 = *((unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i2);
			f4 = *((unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i2);

			// 插值1
			f12 = (unsigned char) (f1 + (x - i1) * (f2 - f1));			
			// 插值2
			f34 = (unsigned char) (f3 + (x - i1) * (f4 - f3));			
			// 插值3
			return ((unsigned char) (f12 + (y -j1) * (f34 - f12)));
		}
	}
}	

/////////////////////////////////////////

/*************************************************************************
* 函数名称：
*   RotateDIB2()
* 参数:
*   LPSTR lpDIB		- 指向源DIB的指针
*   int iRotateAngle	- 旋转的角度（0-360度）
* 返回值:
*   HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
* 说明:
*   该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。
* 调用该函数会自动扩大图像以显示所有的像素。函数中采用双线性插
* 值算法进行插值。
************************************************************************/
HGLOBAL RotateDIB2(LPSTR lpSrcDib, float fRotateAngle,LPSTR lpSrcStartBits,long lWidth, 
				   long lHeight,WORD palSize)
{	
	//LONG	lWidth;				// 源图像的宽度
	//LONG	lHeight;			// 源图像的高度	
	LONG	lNewWidth;			// 旋转后图像的宽度
	LONG	lNewHeight;			// 旋转后图像的高度
	LONG	lNewLineBytes;		// 旋转后图像的宽度（lNewWidth'，必须是4的倍数）	
	LPSTR	lpDIBBits;			// 指向源图像的指针	
	HGLOBAL	hDIB;				// 旋转后新DIB句柄	
	LPSTR	lpDst;				// 指向旋转图像对应像素的指针	
	LPSTR	lpNewDIB;			// 指向旋转图像的指针
	LPSTR	lpNewDIBBits;	
	LPBITMAPINFOHEADER lpbmi;	// 指向BITMAPINFO结构的指针（Win3.0）	
	LPBITMAPCOREHEADER lpbmc;	// 指向BITMAPCOREINFO结构的指针

	LONG	i;					// 循环变量（像素在新DIB中的坐标）
	LONG	j;	
	FLOAT	i0;					// 像素在源DIB中的坐标
	FLOAT	j0;	

	//float	fRotateAngle;		// 旋转角度（弧度）	
	float	fSina, fCosa;		// 旋转角度的正弦和余弦	
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;	
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	float	f1,f2;


	lpDIBBits = lpSrcStartBits;		// 找到源DIB图像像素起始位置	
	//lWidth = DIBWidth(lpDIB);			// 获取图像的宽度
	//lHeight = DIBHeight(lpDIB);			// 获取图像的高度

	// 将旋转角度从度转换到弧度
	//float fRotateAngle = (float) AngleToRadian(RotPara.m_rotAngle);

	fSina = (float) sin((double)fRotateAngle);	// 计算旋转角度的正弦
	fCosa = (float) cos((double)fRotateAngle);	// 计算旋转角度的余弦

	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);

	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// 计算旋转后的图像实际宽度
	lNewWidth  = (LONG)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// 计算旋转后的图像高度
	lNewHeight = (LONG)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));

	// 分配内存，以保存新DIB
	hDIB = (HGLOBAL) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + 
		*(LPDWORD)lpSrcDib + palSize);	
	if (hDIB == NULL)
	{
		return NULL;
	}

	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpSrcDib, *(LPDWORD)lpSrcDib + palSize);

	// 找到新DIB像素起始位置
	lpNewDIBBits = lpNewDIB+ *(LPDWORD)lpNewDIB +palSize;;//FindDIBBits(lpNewDIB);
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	/*
	// 更新DIB中图像的高度和宽度
		if (IS_WIN30_DIB(lpNewDIB))
		{
			// 对于Windows 3.0 DIB
			lpbmi->biWidth = lNewWidth;
			lpbmi->biHeight = lNewHeight;
		}
		else
		{
			// 对于其它格式的DIB
			lpbmc->bcWidth = (unsigned short) lNewWidth;
			lpbmc->bcHeight = (unsigned short) lNewHeight;
		}*/

	lpbmi->biWidth = lNewWidth;
	lpbmi->biHeight = lNewHeight;
	

	for(i = 0; i < lNewHeight; i++)		// 针对图像每行进行操作
	{			
		for(j = 0; j < lNewWidth; j++)	// 针对图像每列进行操作
		{
			// 指向新DIB第i行，第j个像素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// 计算该像素在源DIB中的坐标
			i0 = -((float) j) * fSina + ((float) i) * fCosa + f2;
			j0 =  ((float) j) * fCosa + ((float) i) * fSina + f1;

			// 利用双线性插值算法来估算像素值
			*lpDst = Interpolation (lpDIBBits, lWidth, lHeight, j0, i0);			
		}		
	}

	return hDIB;
}









#endif // !defined(AFX_FUNCTION_H__6E194843_FEB3_491F_8062_765AA3465CBC__INCLUDED_)
