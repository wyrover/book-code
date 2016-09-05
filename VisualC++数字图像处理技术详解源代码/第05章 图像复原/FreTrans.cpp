#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

// FOURBYTES就是用来计算离4最近的整倍数
#define FOURBYTES(bits)    (((bits) + 31) / 32 * 4)

/**************************************************************************
 *  文件名：FreTrans.cpp
 *
 *  正交变换API函数库：
 *
 *  THREECROSS()		- 将实对称矩阵化作三对角矩阵
 *  BSTQ()			    - 求三对角对称矩阵的特征值和特征向量
 *  FFT_1D()			- 快速一维傅立叶变换
 *  IFFT_1D()			- 快速一维傅立叶反变换
 *  IFFT_2D()			- 快速二维傅立叶反变换
 *  DCT()				- 离散余弦变换
 *	IDCT()				- 离散余弦反变换
 *  WALSH()				- 沃尔什－哈达玛变换
 *  IWALSH()			- 沃尔什－哈达玛反变换
 *
 *
 *  DIBFFT_2D()			- 图象的二维离散傅立叶快速变换
 *  DIBDFT_2D()			- 图象的二维离散傅立叶变换
 *  DIBHOTELLING()		- 图象的霍特林变换
 *  DIBDct()			- 图像的离散余弦变换
 *  DIBWalsh()			- 图像的沃尔什－哈达玛变换
 *
 ************************************************************************
*/
 

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

VOID WINAPI FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel)
{	
	// 循环控制变量
	int		i;
	int     j;
	int     k;

	double PI = 3.1415926; 

	// 傅立叶变换点数
	int	nCount =0 ;

	// 计算傅立叶变换点数
	nCount =(int)pow(2,nLevel) ;
	
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
		for(j = 0; j < (int)pow(2,k); j++)
		{
			//计算长度
			nBtFlyLen = (int)pow( 2,(nLevel-k) );
			
			//倒序重排，加权计算
			for(i = 0; i < nBtFlyLen/2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] = 
					pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2]) 
					* pCW[(int)(i * pow(2,k))];
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
VOID WINAPI IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel)
{
	
	// 循环控制变量
	int		i;

	// 傅立叶反变换点数
	int nCount;

	// 计算傅立叶变换点数
	nCount = (int)pow(2,nLevel) ;
	
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
VOID WINAPI DIBFFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
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
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;	
	
	// x，y（行列）方向上的迭代次数
	int		nXLev;
	int		nYLev;

	// 计算x，y（行列）方向上的迭代次数
	nXLev = (int) ( log(nTransWidth)/log(2) +  0.5 );
	nYLev = (int) ( log(nTransHeight)/log(2) + 0.5 );
	
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
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的
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
VOID WINAPI IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
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
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
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
	::DIBFFT_2D(pCWork, nWidth, nHeight, pCTData) ;
	
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


/*************************************************************************
 *
 * 函数名称：
 *   DCT()
 *
 * 参数:
 *   double * dpf				- 指向时域值的指针
 *   double * dpF				- 指向频域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   实现一维快速离散余弦变换。
 *
 ***********************************************************************
*/
VOID WINAPI DCT(double *dpf, double *dpF, int r)
{
	double PI = 3.1415926; 

	// 离散余弦变换点数
	LONG	lNum;

	// 循环变量
	int		i;
	
	// 中间变量
	double	dTemp;	
	
	complex<double> *comX;
	
	// 离散余弦变换点数
	lNum = 1<<r;
	
	// 分配内存
	comX = new complex<double>[lNum*2];
	
	// 赋初值0
	memset(comX, 0, sizeof(complex<double>) * lNum * 2);
	
	// 将时域点转化为复数形式
	for(i=0;i<lNum;i++)
	{
		comX[i] = complex<double> (dpf[i], 0);
	}
	
	// 调用快速付立叶变换
	FFT_1D(comX,comX,r+1);
	
	// 调整系数
	dTemp = 1/sqrt(lNum);
	
	// 求dpF[0]
	dpF[0] = comX[0].real() * dTemp;
	
	dTemp *= sqrt(2);
	
	// 求dpF[u]	
	for(i = 1; i < lNum; i++)
	{
		dpF[i]=(comX[i].real() * cos(i*PI/(lNum*2)) 
				+ comX[i].imag() * sin(i*PI/(lNum*2))) * dTemp;
	}
	
	// 释放内存
    delete comX;
}


/*************************************************************************
 *
 * 函数名称：
 *   IDCT()
 *
 * 参数:
 *   double * dpF				- 指向频域值的指针
 *   double * dpf				- 指向时域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   实现一维快速离散余弦反变换。
 *
 ************************************************************************/
VOID WINAPI IDCT(double *dpF, double *dpf, int r)
{
	double PI = 3.1415926; 

	// 离散余弦反变换点数
	LONG	lNum;

	// 计算离散余弦变换点数
	lNum = 1<<r;
		
	// 循环变量
	int		i;
	
	// 中间变量
	double	dTemp, d0;
	
	complex<double> *comX;
		
    // 给复数变量分配内存
	comX = new complex<double>[lNum*2];
	
	// 赋初值0
	memset(comX, 0, sizeof(complex<double>) * lNum * 2);
	
	// 将频域变换后点写入数组comX
	for(i=0;i<lNum;i++)
	{
		comX[i] = complex<double> (cos(i*PI/(lNum*2)) * dpF[i], sin(i*PI/(lNum*2) * dpF[i]));
	}
	
	// 调用快速付立叶反变换
	IFFT_1D(comX,comX,r+1);
	
	// 调整系数
	dTemp = sqrt(2.0/lNum);
	d0 = (sqrt(1.0/lNum) - dTemp) * dpF[0];

	// 计算dpf(x)
	for(i = 0; i < lNum; i++)
	{
		dpf[i] = d0 + 2 * lNum * comX[i].real()* dTemp ;
	}
	
	// 释放内存
	delete comX;
}


/*************************************************************************
 *
 * 函数名称：
 *   WALSH()
 *
 * 参数:
 *   double * dpf				- 指向时域值的指针
 *   double * dpF				- 指向频域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现一维快速沃尔什-哈达玛变换。
 *
 ***********************************************************************
*/

VOID WINAPI WALSH(double *dpf, double *dpF, int r)
{
	// 沃尔什-哈达玛变换点数
	LONG	lNum;
	
	// 快速沃尔什变换点数
	lNum = 1 << r;

	// 循环变量
	int		i,j,k;
	
	// 中间变量
	int		nTemp,m;
	
	double *X1,*X2,*X;
			
	// 分配运算所需的数组
	X1 = new double[lNum];
	X2 = new double[lNum];
	
	// 将时域点写入数组X1
	memcpy(X1, dpf, sizeof(double) * lNum);
		
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1<<k; j++)
		{	
			// 按照蝶形运算图进行运算
			nTemp = 1 << (r-k);
			for(i = 0; i < nTemp / 2; i++)
			{
				m = j * nTemp;
				X2[i + m] = X1[i + m] + X1[i + m + nTemp / 2];
				X2[i + m + nTemp / 2] = X1[i + m] - X1[i + m + nTemp / 2];
			}
		}
		
		// 互换  
		X = X2;
		X2 = X1;
		X1 = X;
	}
	
	// 对系数做调整
	for(j = 0; j < lNum; j++)
	{
		m = 0;
		for(i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				m += 1 << (r-i-1);
			}
		}

		dpF[j] = X1[m] / lNum;
	}
	
	// 释放内存
	delete X1;
	delete X2;
}


/*************************************************************************
 *
 * 函数名称：
 *   THREECROSS()
 *
 * 参数:
 *   double  *Matrix     - 用来存放矩阵A
 *   int     Rank        - 矩阵A的阶数
 *   double	 *QMatrix    - 返回householder变换的矩阵Q
 *   double  *MainCross  - 对称三角阵中的主对角元素
 *   double  *HypoCross  - 对称三角阵中的次对角元素
 *
 * 返回值:
 *   BOOL                - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用householder变换将n阶实对称矩阵化为对称三角阵
 *
 *
 ***********************************************************************
*/

BOOL WINAPI THREECROSS(double *Matrix, int Rank, double *QMatrix, 
					    double *MainCross, double *HypoCross)
{
	//   循环变量
	int i, j, k, u;
    double h, f, g, h2;
    
	// 对矩阵QMatrix赋值
	for(i = 0; i <= Rank-1; i++)
		for(j = 0; j <= Rank-1; j++)
		{
			u = i*Rank + j; 
			QMatrix[u] = Matrix[u];
		}

    for (i = Rank-1; i >= 1; i--)
    {
		h = 0.0;
        if (i > 1)
          for (k = 0; k <= i-1; k++)
          {
			  u = i*Rank + k; 
			  h = h + QMatrix[u]*QMatrix[u];
		  }
        
		// 如果一行全部为零
		if (h + 1.0 == 1.0)
        {
			HypoCross[i] = 0.0;
            if (i == 1) 
				HypoCross[i] = QMatrix[i*Rank+i-1];
            MainCross[i] = 0.0;
        }
        
		// 否则进行householder变换，求正交矩阵的值
		else
        {
			// 求次对角元素的值
			HypoCross[i] = sqrt(h);
            
			// 判断i行i-1列元素是不是大于零
			u = i*Rank + i - 1;
            if (QMatrix[u] > 0.0) 
				HypoCross[i] = -HypoCross[i];
            
			h = h - QMatrix[u]*HypoCross[i];
            QMatrix[u] = QMatrix[u] - HypoCross[i];
            f = 0.0;
            
			// householder变换
		    for (j = 0; j <= i-1; j++)
            { 
				QMatrix[j*Rank+i] = QMatrix[i*Rank+j] / h;
                g = 0.0;
                
				for (k = 0; k <= j; k++)
                  g = g + QMatrix[j*Rank+k]*QMatrix[i*Rank+k];
                
				if (j+1 <= i-1)
                  for (k = j+1; k <= i-1; k++)
                    g = g + QMatrix[k*Rank+j]*QMatrix[i*Rank+k];
                
				HypoCross[j] = g / h;
                f = f + g*QMatrix[j*Rank+i];
            }
            
			h2 = f / (h + h);
            
			// 求正交矩阵的值
			for (j = 0; j <= i-1; j++)
            {
				f = QMatrix[i*Rank + j];
                g = HypoCross[j] - h2*f;
                HypoCross[j] = g;
                
				for (k = 0; k <= j; k++)
                {
					u = j*Rank + k;
                    QMatrix[u] = QMatrix[u] - f*HypoCross[k] - g*QMatrix[i*Rank + k];
                 }
            }
            MainCross[i] = h;
         }
    }

    // 赋零值
    for (i = 0; i <= Rank-2; i++) 
		HypoCross[i] = HypoCross[i + 1];
    HypoCross[Rank - 1] = 0.0;
    MainCross[0]        = 0.0;
    
	for (i = 0; i <= Rank-1; i++)
    { 
		// 主对角元素的计算
		if ((MainCross[i] != 0.0) && (i-1 >= 0))
			for (j = 0; j <= i-1; j++)
			{
				g = 0.0;
				for (k = 0; k <= i-1; k++)
					g = g + QMatrix[i*Rank + k]*QMatrix[k*Rank + j];
				for (k = 0; k <= i-1; k++)
				{ 
					u = k*Rank + j;
					QMatrix[u] = QMatrix[u] - g*QMatrix[k*Rank + i];
				}
			}

        // 将主对角线的元素进行存储，以便返回
		u = i*Rank + i;
        MainCross[i] = QMatrix[u]; 
		QMatrix[u]   = 1.0;
        
		// 将三对角外所有的元素赋零值
		if (i-1 >= 0)
          for (j = 0; j <= i-1; j++)
          { 
			  QMatrix[i*Rank + j] = 0.0; 
			  QMatrix[j*Rank+i]   = 0.0;
		  }
    }
    
	// 返回值
	return(TRUE);
}


/*************************************************************************
 *
 * 函数名称：
 *   BSTQ()
 *
 * 参数:
 *   int     Rank        - 矩阵A的阶数
 *   double	 *MainCross  - 对称三角阵中的主对角元素，返回时存放A的特征值
 *   double  *HypoCross  - 对称三角阵中的次对角元素
 *	 double  *Matrix     - 返回对称矩阵A的特征向量
 *   double Eps          - 控制精度
 *   int MaxT            - 最大迭代次数
 *
 * 返回值:
 *   BOOL                - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用变形QR方法计算实对称三角矩阵的全部特征值以及相应的特征向量
 *
 *
 ***********************************************************************
*/
BOOL WINAPI BSTQ(int Rank, double *MainCross, double *HypoCross, 
				  double *Matrix, double Eps, int MaxT)
{
	// 变量的定义
	int i, j, k, m, it, u, v;
    double d, f, h, g, p, r, e, s;

	// 赋零值
    HypoCross[Rank - 1] = 0.0; 
	d = 0.0; 
	f = 0.0;
    
	for(j = 0; j <= Rank-1; j++)
    {
		//  迭代精度的控制
		it = 0;
        h = Eps * (fabs(MainCross[j]) + fabs(HypoCross[j]));
        if(h > d) 
			d = h;
        m = j;
        
		while((m <= Rank-1) && (fabs(HypoCross[m]) > d)) 
			m = m + 1;
        
		if(m != j)
        {
			// 进行迭代，求得矩阵A的特征值和特征向量
			do
            {
				// 超过迭代次数，返回迭代失败
				if(it == MaxT)
					return(FALSE);
                it = it + 1;
                g = MainCross[j];
                p = (MainCross[j + 1] - g) / (2.0 * HypoCross[j]);
                r = sqrt(p*p + 1.0);
                
				// 如果p大于0
				if (p >= 0.0)
					MainCross[j] = HypoCross[j]/(p + r);
                else
					MainCross[j] = HypoCross[j]/(p - r);
                
				h = g - MainCross[j];
                
				//  计算主对角线的元素
				for (i = j + 1; i <= Rank - 1; i++)
                  MainCross[i] = MainCross[i] - h;
                
				// 赋值
				f = f + h;
				p = MainCross[m];
				e = 1.0; s = 0.0;
                
				for(i = m - 1; i >= j; i--)
                {
					g = e * HypoCross[i];
					h = e * p;
                    
					//  主对角线元素的绝对值是否大于次对角线元素的
					if(fabs(p) >= fabs(HypoCross[i]))
                    {
						e = HypoCross[i] / p;
						r = sqrt(e*e + 1.0);
                        HypoCross[i + 1] = s*p*r; 
						s = e / r;  e = 1.0 / r;
                     }
                    else
					{
						e = p / HypoCross[i]; 
						r = sqrt(e*e + 1.0);
                        HypoCross[i+1] = s * HypoCross[i] * r;
                        s = 1.0 / r; e = e / r;
                      }
                    
					p = e*MainCross[i] - s*g;
                    MainCross[i + 1] = h + s*(e*g + s*MainCross[i]);
                    
					// 重新存储特征向量
					for(k = 0; k <= Rank - 1; k++)
                    {
						u = k*Rank + i + 1; v = u - 1;
                        h = Matrix[u]; 
						Matrix[u] = s*Matrix[v] + e*h;
                        Matrix[v] = e*Matrix[v] - s*h;
                    }
                
				}
                
				// 将主对角线和次对角线元素重新赋值
				HypoCross[j] = s * p; 
				MainCross[j] = e * p;
            
			}
            while (fabs(HypoCross[j]) > d);
        }

        MainCross[j] = MainCross[j] + f;
    }
    
	// 返回A的特征值
	for (i = 0; i <= Rank-1; i++)
    {
		k = i; p = MainCross[i];
        
		// 将A特征值赋给p
		if(i+1 <= Rank-1)
        {
			j = i + 1;
            while((j <= Rank-1) && (MainCross[j] <= p))
            { k = j; 
			  p = MainCross[j]; 
			  j = j+1;
			}
        }
        
		// 存储A的特征值和特征向量
		if (k != i)
        {
			MainCross[k] = MainCross[i];
			MainCross[i] = p;
            for(j = 0; j <= Rank-1; j++)
            {
				u = j*Rank + i; 
				v = j*Rank + k;
                p = Matrix[u]; 
				Matrix[u] = Matrix[v];
				Matrix[v] = p;
            }
        }
    }

  // 返回值
  return(TRUE);
}



/*************************************************************************
 *
 * 函数名称：
 *   IWALSH()
 *
 * 参数:
 *   double * dpF				- 指向频域值的指针
 *   double * dpf				- 指向时域值的指针
 *   n						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现一维快速沃尔什-哈达玛反变换。
 *
 ***********************************************************************
 */

VOID WINAPI IWALSH(double *dpF, double *dpf, int n)
{
	// 变换点数
	LONG	lNum;
	
	// 循环变量
	int		i;
	
	// 计算变换点数
	lNum = 1 << n;
	
	// 用快速沃尔什－哈达玛变换进行反变换
	WALSH(dpF, dpf, n);
	
	// 对系数进行调整
	for(i = 0; i < lNum; i++)
	{
		dpf[i] *= lNum;
	}
}


/*************************************************************************
 *
 * \函数名称：
 *   DFT_2D()
 *
 * \输入参数:
 *   CDib * pDib				- 指向CDib类的指针，含有图像数据
 *   double * pTrRstRpart		- 指向傅立叶系数实部的指针
 *   double * pTrRstIpart		- 指向傅立叶系数虚部的指针
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   二维傅立叶变换。
 *
 *************************************************************************
 */
VOID WINAPI DIBDFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart)
{
	double PI = 3.14159;
	//遍历图象的纵坐标
	int y;

	//遍历图象的横坐标
	int x;

	//频域的横坐标
	int m;

	//频域的纵坐标
	int n; 

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	int nSaveWidth = sizeImageSave.cx;

	//图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;

	//初始化结果数据
	for(n=0; n<nHeight ; n++ )
		for(m=0 ; m<nWidth ; m++ )
		{
			*(	pTrRstRpart + n*nWidth + m	) =0;
			*(	pTrRstIpart + n*nWidth + m	) =0;
		}
	double fCosTable;
	double fSinTable;
	int	  nPxValue;

	fCosTable=0 ;
	nPxValue =0;

	double fTmpRstR;
	double fTmpRstI;
	for(n=0; n<nHeight ; n++ )
		for(m=0 ; m<nWidth ; m++ )
		{
			fTmpRstR=0;
			fTmpRstI=0;
			for(y=0; y<nHeight ; y++ )
				for(x=0 ; x<nWidth ; x++ )
				{
					fCosTable= 
						cos(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fSinTable= 
						sin(	-2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					nPxValue = *(pImageData+ y*nSaveWidth + x )			;

					fTmpRstR+=fCosTable* nPxValue						;
					fTmpRstI+=fSinTable* nPxValue						;
				}
			*( pTrRstRpart + nWidth * n + m ) = fTmpRstR;
			*( pTrRstIpart + nWidth * n + m ) = fTmpRstI;
		}
}

/*************************************************************************
 *
 * \函数名称：
 *   IDFT_2D()
 *
 * \输入参数:
 *   CDib * pDib				- 指向CDib类的指针，含有图像数据
 *   double * pTrRstRpart		- 指向傅立叶系数实部的指针
 *   double * pTrRstIpart		- 指向傅立叶系数虚部的指针
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   二维傅立叶反变换。
 *
 *************************************************************************
 */
VOID WINAPI IDFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart)
{
	double PI = 3.1415926;
	//遍历图象的纵坐标
	int y;

	//遍历图象的横坐标
	int x;

	//频域的横坐标
	int m;

	//频域的纵坐标
	int n; 

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	int nSaveWidth = sizeImageSave.cx;

	//图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;


	// 正弦和余弦值
	double fCosTable;
	double fSinTable;
	fCosTable=0 ;
	fSinTable=0 ;

	// 临时变量
	double fTmpPxValue;
	double fRpartValue;
	double fIpartValue;
	fTmpPxValue=0;
	fRpartValue=0;
	fIpartValue=0;

	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			fTmpPxValue=0;
			for(n=0; n<nHeight ; n++ )
				for(m=0 ; m<nWidth ; m++ )
				{
					// 生成正弦和余弦值
					fCosTable= 
						cos(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fSinTable= 
						sin(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;

					// 傅立叶变化的实部与虚部
					fRpartValue=*(pTrRstRpart+ n*nHeight + m ) ;
					fIpartValue=*(pTrRstIpart+ n*nHeight + m ) ;
					
					// 加权相加
					fTmpPxValue+=fCosTable* fRpartValue-fSinTable*fIpartValue;
				}
			
			// 傅立叶系数变换对要求除以(nHeight*nWidth)
			fTmpPxValue=fTmpPxValue/(nHeight*nWidth);

			*( pImageData + nSaveWidth * y + x) = (unsigned char) fTmpPxValue ;
		}
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBWalsh()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行二维快速沃尔什-哈达玛变换。
 *
 ***********************************************************************
 */

BOOL WINAPI DIBWalsh(CDib * pDib)
{
	// 指向源图像的指针
	unsigned char *lpSrc;		
	
	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 循环变量
	LONG	i;
	LONG	j;
	
	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;
	
	// 中间变量
	double	dTemp;	

	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;

	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
		
	// 保证离散余弦变换的宽度和高度为2的整数次方
	while(lW * 2 <= lWidth)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}
	
	// 分配内存
	double *dpf = new double[lW * lH];
	double *dpF = new double[lW * lH];
	
	// 时域赋值
	for(i = 0; i < lH; i++)
	{
		// 列
		for(j = 0; j < lW; j++)
		{
			// 指向DIBi行j列象素的指针
			lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 将象素值赋值给时域数组
			dpf[j + i * lW] = *(lpSrc);
		}
	}
	
	for(i = 0; i < lH; i++)
		// 对y方向进行沃尔什-哈达玛变换
		WALSH(dpf + lW * i, dpF + lW * i, wp);
		
	// 保存计算结果
	for(i = 0; i < lH; i++)
	{
		for(j = 0; j < lW; j++)
		{
			dpf[j * lH + i] = dpF[j + lW * i];
		}
	}
	
	for(j = 0; j < lW; j++)
		// 对x方向进行沃尔什-哈达玛变换
		WALSH(dpf + j * lH, dpF + j * lH, hp);
	
	// 行
	for(i = 0; i < lH; i++)
	{
		// 列
		for(j = 0; j < lW; j++)
		{
			// 计算频谱
			dTemp = fabs(dpF[j * lH + i] * 1000);
			
			if (dTemp > 255)
			{
				// 超过255直接设置为255
				dTemp = 255;
			}
			
			// 指向DIBi行j列象素的指针
			lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	//释放内存
	delete dpf;
	delete dpF;

	// 返回
	return TRUE;
}



/*************************************************************************
 *
 * 函数名称：
 *   DIBDct()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行二维离散余弦变换。
 *
 ***********************************************************************
 */
BOOL WINAPI DIBDct(CDib *pDib)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 循环变量
	LONG	i;
	LONG	j;
	
	// 离散余弦变换的宽度和高度，必须为2的整数次方
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;
	
	// 中间变量
	double	dTemp;

	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;

   //图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;	
		
	// 保证离散余弦变换的宽度和高度为2的整数次方
	while(lW * 2 <= lWidth)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}
	
	// 分配内存
	double *dpf = new double[lW * lH];
	double *dpF = new double[lW * lH];
	
	// 时域赋值
	for(i = 0; i < lH; i++)
	{
		for(j = 0; j < lW; j++)
		{
			// 指针指向位图i行j列的象素
			lpSrc = lpDIBBits + lLineBytes * ( i) + j;
//			lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// 将象素值赋给时域数组
			dpf[j + i * lW] = *(lpSrc);
		}
	}
	
	for(i = 0; i < lH; i++)
		// y方向进行离散余弦变换
		DCT(&dpf[lW * i], &dpF[lW * i], wp);

	// 保存计算结果
	for(i = 0; i < lH; i++)
		for(j = 0; j < lW; j++)
			dpf[j * lH + i] = dpF[j + lW * i];
		
	
	for(j = 0; j < lW; j++)
		// x方向进行离散余弦变换
		DCT(&dpf[j * lH], &dpF[j * lH], hp);
	
	// 频谱的计算
	for(i = 0; i < lH; i++)
	{
		for(j = 0; j < lW; j++)
		{
			dTemp = fabs(dpF[j*lH+i]);
			
			// 是否超过255
			if (dTemp > 255)
				// 如果超过，设置为255
				dTemp = 255;
		
			// 指针指向位图i行j列的象素
			lpSrc = lpDIBBits + lLineBytes * ( i) + j;
//			lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	// 释放内存
	delete dpf;
	delete dpF;

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBOHOTELLING()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用霍特林变换来对图像进行旋转。
 *
 ***********************************************************************
 */

BOOL WINAPI DIBHOTELLING(CDib *pDib)
{
	
	// 指向源图像的指针
	unsigned char*	 lpSrc;

	// 循环变量
	LONG	i;
	LONG	j;
	
	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;

	// 经过变换后图象最大可能范围
	LONG    lMaxRange;
	
	//  物体坐标的均值
    LONG    AverEx;
	LONG    AverEy;
	
	//  物体总的象素数
	LONG    ToaCount;

	// 坐标值的协方差矩阵
	double  Matr4C[2][2];

	// 存放协方差矩阵的特征向量
	double  QMatrix[2][2];
	
	//  三对角阵的主对角和次对角线元素
	double  MainCross[2];
	double  HypoCross[2];
	
	// 中间变量
	double	dTemp;
	LONG    lTempI;
	LONG    lTempJ;
		
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;

   //图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;	
			
	// 估计图象经过旋转后可能最大的宽度和高度
	if(lWidth>lHeight)
		lMaxRange = lWidth;
	else
		lMaxRange =lHeight;
	
	// 赋初值
	AverEx=0.0;
	AverEy=0.0;
	ToaCount = 0;
	Matr4C[0][0] = Matr4C[0][1] = Matr4C[1][0] = Matr4C[1][1] = 0.0;
	
	// 分配内存
	double *F = new double[lWidth*lHeight];
	
	// 行
	for(i = 0; i < lHeight; i++)
	{
		// 列
		for(j = 0; j < lWidth; j++)
		{
			// 给旋转后坐标轴的每个点赋零值（灰度值255对应显示白色）
			F[i*lWidth + j] = 255;

			// 指向位图i行j列象素的指针
			lpSrc = lpDIBBits + lLineBytes*i + j;
						
			// 值小于255（非背景色白色）的象素认为物体的一部分
			// 并将其坐标值x和y看作二维随机矢量
			if((*lpSrc) < 255)
			{				
				// 属于物体象素的Y坐标和X坐标累计值
				AverEx=AverEx+i;
				AverEy=AverEy+j;

				// 物体总的象素数加一
				ToaCount++;
                   
                // 随机矢量协方差矩阵的累计值
				Matr4C[0][0] = Matr4C[0][0] + i*i;
                Matr4C[0][1] = Matr4C[0][1] + i*j;
				Matr4C[1][0] = Matr4C[1][0] + j*i;
				Matr4C[1][1] = Matr4C[1][1] + j*j;
			}
		}
	}
	
	
	// 计算随机矢量的均值
	AverEx = AverEx/ToaCount;
	AverEy = AverEy/ToaCount;

	//  计算随机矢量的协方差矩阵
    Matr4C[0][0] = Matr4C[0][0]/ToaCount - AverEx*AverEx;
	Matr4C[0][1] = Matr4C[0][1]/ToaCount - AverEx*AverEy;
	Matr4C[1][0] = Matr4C[1][0]/ToaCount - AverEx*AverEy;
	Matr4C[1][1] = Matr4C[1][1]/ToaCount - AverEy*AverEy;

    // 规定迭代的计算精度
	double Eps = 0.000001;
	
	// 将协方差矩阵化作三对角对称阵
    THREECROSS(*Matr4C, 2, *QMatrix, MainCross, HypoCross);
	
	// 求协方差矩阵的特征值和特征矢向量
	BSTQ(2, MainCross,HypoCross, *Matr4C, Eps, 50);
    
	// 将特征列向量转化称特征列向量
    dTemp = Matr4C[0][1];
	Matr4C[0][1] = Matr4C[1][0];
	Matr4C[1][0] = dTemp;

	// 对特征列向量进行归一化
	for(i=0;i<=1;i++)
	{
		dTemp = pow(Matr4C[i][0],2) + pow(Matr4C[i][1],2);
		dTemp = sqrt(dTemp);
		Matr4C[i][0] = Matr4C[i][0]/dTemp;
		Matr4C[i][1] = Matr4C[i][1]/dTemp;
	}
	
	// 查找经霍特林变换后的坐标点在原坐标系中的坐标    
    for(i = -lMaxRange+1; i < lMaxRange; i++)
	{
		for(j = -lMaxRange+1; j < lMaxRange; j++)
		{
			//  将新坐标值映射到旧的坐标系
			int Cx = (int)(i*Matr4C[0][0]-j*Matr4C[0][1])+AverEx;
			int Cy = (int)(-i*Matr4C[1][0]+j*Matr4C[1][1])+AverEy;
			
			//  映射值是否属于源图象
			if( Cx>=0 && Cx<lHeight && Cy>=0 && Cy<lWidth )
			{
				lpSrc = lpDIBBits + lLineBytes*Cx + Cy;

				// 映射值是否属于原来的物体
				if(*(lpSrc)<255)
				{
					//  将新坐标系原点平移到中心，以便显示
					lTempI=(LONG)(lHeight/2)+j;
					lTempJ=(LONG)(lWidth/2)+i;

					// 看如果能够进行显示，赋值给数组，进行存储
					if( lTempI>=0 && lTempI<lHeight && lTempJ>=0 && lTempJ<lWidth )
						F[lTempJ+ (lTempI) * lWidth]=*(lpSrc);
				}

			}		
		}
	}
	
	// 行
	for(i = 0; i < lMaxRange; i++)
	{
		// 列
		for(j = 0; j < lMaxRange; j++)
		{
			// 霍特林变换后的象素值
		    dTemp = F[i * lMaxRange + j] ;
			
			// 指向位图i行j列象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	// 释放内存
	delete F;

	// 返回
	return TRUE;
}
