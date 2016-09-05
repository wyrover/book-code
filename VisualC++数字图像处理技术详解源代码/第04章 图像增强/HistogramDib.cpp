//======================================================================
// 文件： HistogramDib.cpp
// 内容： 直方图增强函数-源文件
// 功能： （1）直方图统计函数；
//        （2）直方图均衡化函数；
//        （3）直方图匹配（规定化）函数；
// 
//======================================================================

#include "StdAfx.h"
#include "HistogramDib.h"


//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================

CHistogramDib::CHistogramDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CHistogramDib::~CHistogramDib(void)
{
	
}


//=======================================================
// 函数功能： 直方图统计函数
// 输入参数： float *probability -灰度分布概率密度
// 返回值：   无
//=======================================================

void CHistogramDib::Histogram_Statistic( float *probability)
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	long i,j;			//循环变量	
	int gray[256]; // 灰度计数
	BYTE pixel;	//像素值

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 灰度计数变量初始化
	memset(gray,0,sizeof(gray));

	//逐个扫面图像中的像素点,进行灰度计数统计
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)*lpSrc;

			// 灰度统计计数
			gray[pixel]++;

		}
	}
	
	// 计算灰度概率密度
	for(i=0;i<256;i++)
    {
		probability[i] = gray[i] / (lHeight * lWidth *1.0f);
    }
	
}

//=======================================================
// 函数功能： 直方图均衡化函数
// 输入参数： 无
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CHistogramDib::Histogram_Equalization( )
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	BYTE pixel;	//像素值

	//原图像灰度分布概率密度变量
	float fPro[256];
	//中间变量
	float temp[256];
	int nRst[256];

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

	// 初始化中间变量temp
	memset(temp, 0, sizeof(temp));

	// 获取原图像灰度分布的概率密度
	Histogram_Statistic(fPro);

	//进行直方图均衡化处理
	for(i = 0; i < 256; i++)
	{
		if(i == 0)
		{
			temp[0] = fPro[0];	 
		}
		else
		{
			temp[i] = temp[i-1] + fPro[i];	 
		}
		nRst[i] = (int)(255.0f * temp[i] + 0.5f);
	}	


	//将直方图均衡化后的结果写到目标图像中
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

			*lpDst = (BYTE)(nRst[pixel]);

		}
	}
	
	// 复制均衡化处理后的图像到原图像中
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;

}

//=======================================================
// 函数功能： 直方图规定化（匹配）函数
// 输入参数： BYTE bGray -规定直方图的灰度级
//            int *npMap -规定直方图映射关系
//            float *fpPro -规定灰度分布概率
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CHistogramDib::Histogram_Match( BYTE bGray, int *npMap, float *fpPro)
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	BYTE pixel;	//像素值

	//原图像灰度分布概率密度变量
	float fPro[256];
	//中间变量
	float temp[256];
	//灰度映射表变量
	int nMap[256];

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

	// 获取原图像灰度分布的概率密度
	Histogram_Statistic(fPro);

	// 计算原图像累计直方图
	for (i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			temp[0] = fPro[0];			 
		}
		else
		{
			temp[i] = temp[i-1] + fPro[i];			 
		}
			fPro[i] = temp[i];
	}

	// 计算规定变换后的累计直方图
	for (i = 0; i < bGray; i++)
	{
		if (i == 0)
		{
			temp[0] = fpPro[0];
		}
		else
		{
			temp[i] = temp[i-1] + fpPro[i];
		}
		fpPro[i] = temp[i];
	}

	// 确定映射关系
	for (i = 0; i < 256; i++)
	{
		// 最接近的规定直方图灰度级
		int m = 0;
		// 最小差值
		float min_value = 1.0f;
		// 枚举规定直方图各灰度
		for (j = 0; j < bGray; j++)
		{
			// 当前差值
			float now_value = 0.0f;
			//  差值计算
			if (fPro[i] - fpPro[j] >= 0.0f)
				now_value = fPro[i] - fpPro[j];
			else
				now_value = fpPro[j] - fPro[i];
			// 寻找最接近的规定直方图灰度级
			if (now_value < min_value)
			{
				// 最接近的灰度级
				m = j;
				// 最小差值
				min_value = now_value;
			}
		}

		// 建立灰度映射表
		nMap[i] = npMap[m];
	}

	// 对各像素进行直方图规定化映射处理
	for (j = 0; j < lHeight; j ++)
	{
		for (i = 0; i < lWidth; i ++)
		{
			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像倒数第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)*lpSrc;
            //对目标图像进行映射处理
			*lpDst = (BYTE)(nMap[pixel]);

		}
	}
	
	// 复制直方图规定化处理后的图像到原图像中
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;

}