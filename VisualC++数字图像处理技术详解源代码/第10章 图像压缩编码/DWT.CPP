// 文件DWT.cpp存放的是有关小波变换的函数

#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include "GlobalApi.h"


/*************************************************************************
 *
 * \函数名称：
 *   DWT_1D()
 *
 * \输入参数:
 *   double * pDbSrc		- 指向源数据的指针
 *   int nMaxLevel		- 最大可分解的层数
 *   int nDWTSteps		- 需要分界的层数
 *   int nInv			- 是否为DWT，1表示为IDWT，0表示DWT
 *   int nStep			- 当前的计算层数
 *   int nSupp			- 小波基的紧支集的长度
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数用对存放在pDBSrc中的数据进行一维DWT或者IDWT。其中，nInv为表示进行
 *   DWT或者IDWT的标志。nStep为当前已经分界的层数。计算后数据仍存放在pDbSrc中
 *
 *************************************************************************
 */
BOOL DWT_1D(double* pDbSrc, int nMaxLevel,
			int nDWTSteps, int nInv, int nStep, int nSupp )
{
	// 计算最小可分界的层数
	int MinLevel = nMaxLevel-nDWTSteps;

	// 判断是否为DWT
	if (!nInv)
	{	// DWT
		int n = nMaxLevel;

		while (n>MinLevel)
			// 调用DWTStep_1D进行第n层的DWT
			if (!DWTStep_1D(pDbSrc, n--, nInv, nStep, nSupp)) 
				return FALSE;
	}

	// nInv为1则进行IDWT
	else
	{	// IDWT
		int n = MinLevel;

		while (n<nMaxLevel)
			// 调用DWTStep_1D进行第n层的IDWT
			if (!DWTStep_1D(pDbSrc, n++, nInv, nStep, nSupp)) 
				return FALSE;
	}
	return TRUE;
}

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
	double s = sqrt(2);

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
	
	// 将结果存入源图象中
	for (int i=0; i<CurN; i++)
		pDbSrc[i*nStep] = ptemp[i];

	// 释放临时内存，并返回
	delete[] ptemp;
	return TRUE;
}

/*************************************************************************
 *
 * \函数名称：
 *   DWT_2D()
 *
 * \输入参数:
 *   double * pDbSrc		- 指向源数据的指针
 *   int nMaxWLevel		- X方向最大可分解的层数
 *   int nMaxHLevel		- Y方向最大可分解的层数
 *   int nDWTSteps		- 需要分界的层数
 *   int nInv			- 是否为DWT，1表示为IDWT，0表示DWT
 *   int nStep			- 当前的计算层数
 *   int nSupp			- 小波基的紧支集的长度
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数用对存放在pDBSrc中的二维数据进行二维DWT或者IDWT。其中，nDWTSteps表示
 *   需要分解的层数，nInv为表示进行DWT或者IDWT的标志。nStep为当前已经分界的层数
 *   计算后数据仍存放在pDbSrc中
 *
 *************************************************************************
 */
BOOL DWT_2D(double* pDbSrc, int nMaxWLevel, int nMaxHLevel,
			int nDWTSteps, int nInv, int nStep, int nSupp)
{
	// 计算X，Y方向上最小的分界层数
	int MinWLevel = nMaxWLevel - nDWTSteps;
	int MinHLevel = nMaxHLevel - nDWTSteps;

	// 判断是进行DWT，还是IDWT
	if (!nInv)
	{	// DWT
		int n = nMaxWLevel, m = nMaxHLevel;

		// 调用DWTStep_2D进行分解，分解的层数为nDWTSteps		
		while (n>MinWLevel)
			if (!DWTStep_2D(pDbSrc, n--, m--, nMaxWLevel, nMaxHLevel, nInv, nStep, nSupp)) 
				return FALSE;
	}

	// 否则进行IDWT
	else
	{	// IDWT
		int n = MinWLevel, m = MinHLevel;

		// 调用DWTStep_2D进行IDWT，进行恢复的层数为nDWTSteps	
		while (n<nMaxWLevel)
			if (!DWTStep_2D(pDbSrc, n++, m++, nMaxWLevel, nMaxHLevel, nInv, nStep, nSupp)) 
				return FALSE;
	}

	// 返回
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
	// 计算图象的长度和宽度（2次幂对齐）
	int W = 1<<nMaxWLevel, H = 1<<nMaxHLevel;

	// 计算当前分解的图象的长度和宽度
	int CurW = 1<<nCurWLevel, CurH = 1<<nCurHLevel;

	// 判断是进行DWT还是IDWT
	if (!nInv)
	{	// 对行进行一维DWT
		for (int i=0; i<CurH; i++)
			if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, nInv, nStep, nSupp)) return FALSE;
		// 对列进行一维DWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, nInv, W*nStep, nSupp)) return FALSE;
	}

	// 否则进行IDWT
	else
	{
		// 计算当前变换的图象的长度和宽度
		CurW <<= 1;
		CurH <<= 1;

		// 对列进行IDWT
		for (int i=0; i<CurW; i++)
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
 *   ImageDWT()
 *
 * \输入参数:
 *   CDib* pDibSrc		- 指向源数据的指针
 *   int nMaxWLevel		- X方向上最大可分解的层数
 *   int nMaxHLevel		- Y方向上最大可分解的层数
 *   int nDWTSteps		- 需要进行变换的层数
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
BOOL ImageDWT(LPBYTE lpImage, int nMaxWLevel, int nMaxHLevel,
			int nDWTSteps, int nInv, int nStep, int nSupp)
{
	// 判断变换的层数以及当前层数是否有效
	if (nDWTSteps>nMaxWLevel || nDWTSteps>nMaxHLevel || nStep<=0)
		return FALSE;

	// 获得X，Y方向上的最大象素数（2次幂对齐）
	int W = 1<<nMaxWLevel, H = 1<<nMaxHLevel;

	// 获得X，Y方向上变换时最小的象素数
	int minW = W>>nDWTSteps, minH = H>>nDWTSteps;

	int i, j, index;
	
	// 分配临时内存存放结果
	double*	pDbTemp = new double[W*H];
	if (!pDbTemp) return FALSE;

	// 判断是进行DWT还是IDWT，然后将数据存放到临时内存中，需要注意的是，需要进行采样
	if (!nInv)	// DWT
		for (index=0; index<W*H; index++) pDbTemp[index] = lpImage[index*nStep];
	else		// IDWT
	{
		index = 0;
		for (i=0; i<minH; i++)
		{
			for (j=0; j<minW; j++, index++)
				pDbTemp[index] = lpImage[index*nStep];
			for (; j<W; j++, index++)
				pDbTemp[index] = (char)lpImage[index*nStep];
		}
		for (; index<W*H; index++)
			pDbTemp[index] = (char)lpImage[index*nStep];
	}

	// 调用DWT_2D进行小波变换
	if(!DWT_2D(pDbTemp, nMaxWLevel, nMaxHLevel, nDWTSteps, nInv, nStep, nSupp))
	{
		delete []pDbTemp;
		return FALSE;
	}

	// 将数据存入原始的内存中，需要注意的是，存储时需要进行类型转换
	if (!nInv)	// DWT
	{
		index = 0;
		for (i=0; i<minH; i++)
		{
			for (j=0; j<minW; j++, index++)
				lpImage[index*nStep] = FloatToByte(pDbTemp[index]);
			for (; j<W; j++, index++)
				lpImage[index*nStep] = (BYTE)FloatToChar(pDbTemp[index]);
			//	lpImage[index*nStep] = (BYTE)FloatToByte(pDbTemp[index]);
		}
		for (; index<W*H; index++)
			lpImage[index*nStep] = (BYTE)FloatToChar(pDbTemp[index]);
			//lpImage[index*nStep] = (BYTE)FloatToByte(pDbTemp[index]);
	}
	else		// IDWT
		for (index=0; index<W*H; index++) 
			lpImage[index*nStep] = FloatToByte(pDbTemp[index]);

	// 释放内存
	delete []pDbTemp;

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