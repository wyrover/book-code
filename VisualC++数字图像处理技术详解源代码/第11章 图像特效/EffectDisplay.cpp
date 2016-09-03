#include "StdAfx.h"
#include "mainfrm.h"
#include "EffectDisplay.h"
#include <cmath>

// 静态成员变量初始化
int	EffectDisplay::s_nOffsetX	= 0;
int	EffectDisplay::s_nOffsetY	= 0;
int	EffectDisplay::s_nPicWidth	= 0;
int	EffectDisplay::s_nPicHeight = 0;
int	EffectDisplay::s_nCDCWidth	= 0;
int	EffectDisplay::s_nCDCHeight = 0;

//--------------------------------------------
//	作用:				延时函数
//	参数:		
//		dwMilliseconds	延时时间,单位毫秒
//--------------------------------------------
void EffectDisplay::DelayTime(DWORD dwMilliseconds)
{
	LARGE_INTEGER litmp; 
	LONGLONG lTimeBeign;
	LONGLONG lTimeEnd;
	double dMinus;
	double dFreq;
	double dTim;

	QueryPerformanceFrequency(&litmp);	// 获得机器内部定时器的时钟频率
	dFreq = (double)litmp.QuadPart;		// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	lTimeBeign = litmp.QuadPart;		// 获得时间起始值
	do
	{
		QueryPerformanceCounter(&litmp);
		lTimeEnd = litmp.QuadPart;		//获得中止值
		dMinus = (double)(lTimeEnd - lTimeBeign);
		dTim = dMinus / dFreq;			// 获得对应的时间值，单位为秒
	} while (dTim < dwMilliseconds * 0.001);
}

//--------------------------------------------
//	作用:		向下扫描特效
//--------------------------------------------
void EffectDisplay::ScanDownDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);
	
	for (int i = 0; i <= nHeight; i += 1)
	{
		pDC->BitBlt(s_nOffsetX, i + s_nOffsetY, nWidth, 1, 
					pMemDC, s_nOffsetX, i + s_nOffsetY, SRCCOPY);
		DelayTime(1);
	}
}

//--------------------------------------------
//	作用:		垂直双重扫描特效
//--------------------------------------------
void EffectDisplay::VSScanDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	for (int i = 0; i <= nHeight / 2; i += 1)
	{
		// 上面部分
		pDC->BitBlt(s_nOffsetX, i + s_nOffsetY, nWidth, 1, 
					pMemDC, s_nOffsetX, i + s_nOffsetY, SRCCOPY);
		// 下面部分
		pDC->BitBlt(s_nOffsetX, nHeight - i + s_nOffsetY, nWidth, 1, 
					pMemDC, s_nOffsetX, nHeight - i + s_nOffsetY, SRCCOPY);
		DelayTime(2);
	}
}

//--------------------------------------------
//	作用:		向右移动特效
//--------------------------------------------
void EffectDisplay::MoveRightDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	for (int i = 0; i <= s_nCDCWidth - s_nOffsetX; i += 2)
	{
		pDC->BitBlt(0, s_nOffsetY, i, nHeight, 
					pMemDC, s_nCDCWidth - s_nOffsetX - i, s_nOffsetY, SRCCOPY);
		DelayTime(1);
	}

	// 修正图像位置
	pDC->BitBlt(0, s_nOffsetY, s_nCDCWidth - s_nOffsetX, nHeight, 
				pMemDC, 0, s_nOffsetY, SRCCOPY);
}

//--------------------------------------------
//	作用:		水平双重移动特效
//--------------------------------------------
void EffectDisplay::HSMoveDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	for (int i = 0; i <= s_nCDCWidth / 2; i += 2)
	{
		// 左面部分
		pDC->BitBlt(0, s_nOffsetY, i, nHeight, 
					pMemDC, nWidth / 2 - i + s_nOffsetX, s_nOffsetY, SRCCOPY);
		// 右面部分
		pDC->BitBlt(s_nCDCWidth - i, s_nOffsetY, i, nHeight, 
					pMemDC, nWidth / 2  + s_nOffsetX, s_nOffsetY, SRCCOPY);
		DelayTime(1);
	}

	// 修正图像位置
	pDC->BitBlt(0, s_nOffsetY, s_nCDCWidth - s_nOffsetX, nHeight, 
				pMemDC, 0, s_nOffsetY, SRCCOPY);
}

//--------------------------------------------
//	作用:		垂直百叶窗特效
//--------------------------------------------
void EffectDisplay::VBlindDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	// 百叶窗宽度
	int nScanLine = 8;
	for (int i = 0; i < nScanLine; i++)
	{
		for (int j = 0; j < nWidth; j += nScanLine)
		{
			pDC->BitBlt(j + i + s_nOffsetX, s_nOffsetY, 1, nHeight, 
						pMemDC, j + i + s_nOffsetX, s_nOffsetY, SRCCOPY);
		}

		DelayTime(50);
	}
}

//--------------------------------------------
//	作用:		水平百叶窗特效
//--------------------------------------------
void EffectDisplay::HBlindDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	// 百叶窗宽度
	int nScanLine = 8;
	for (int i = 0; i < nScanLine; i++)
	{
		for (int j = 0; j < nHeight; j += nScanLine)
		{
			pDC->BitBlt(s_nOffsetX, j + i + s_nOffsetY, nWidth, 1, 
						pMemDC, s_nOffsetX, j + i + s_nOffsetY, SRCCOPY);
		}

		DelayTime(50);
	}
}

//--------------------------------------------
//	作用:		垂直栅条特效
//--------------------------------------------
void EffectDisplay::VRasterDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	int nScanLine = 16;

	for (int i = 0; i <= nHeight + s_nOffsetY; i++)
	{
		for (int j = 0; j <= nWidth; j += 2 * nScanLine)
		{
			pDC->BitBlt(j + s_nOffsetX, 0, nScanLine, i, 
						pMemDC, j + s_nOffsetX, nHeight + s_nOffsetY - i, SRCCOPY);

			int k = j + nScanLine;
			pDC->BitBlt(k + s_nOffsetX, nHeight + s_nOffsetY - i, nScanLine, i + s_nOffsetY, 
						pMemDC, k + s_nOffsetX, 0, SRCCOPY);
		}

		DelayTime(1);
	}
}

//--------------------------------------------
//	作用:		水平栅条特效
//--------------------------------------------
void EffectDisplay::HRasterDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	int nScanLine = 16;

	for (int i = 0; i <= nWidth + s_nOffsetX; i++)
	{
		for (int j = 0; j <= nHeight; j += 2 * nScanLine)
		{
			pDC->BitBlt(0, j + s_nOffsetY, i, nScanLine, 
						pMemDC, nWidth + s_nOffsetX - i, j + s_nOffsetY, SRCCOPY);

			int k = j + nScanLine;
			pDC->BitBlt(nWidth + s_nOffsetX - i, k + s_nOffsetY, i + s_nOffsetX, nScanLine, 
						pMemDC, 0, k + s_nOffsetY, SRCCOPY);
		}

		DelayTime(1);
	}
}

//--------------------------------------------
//	作用:		马赛克特效
//--------------------------------------------
void EffectDisplay::MosaicDisplay(CDC* pDC, CDC* pMemDC)
{
	int nTileSize = 24;	// 马赛克小方块大小
	int nRw = 0;
	int nRh = 0;
	
	if (s_nPicWidth % nTileSize != 0)
		nRw = 1;
	if (s_nPicHeight % nTileSize != 0)
		nRh = 1;

	// 计算小方块的个数
	int nTileCount = (s_nPicWidth / nTileSize + nRw) * (s_nPicHeight / nTileSize + nRh);

	CPtrArray	points;		// 保存所有小方块的左上坐标
	long		lx = 0;
	long		ly = 0;

	for (int k = 0; k < nTileCount; k++)
	{
		CPoint* point = new CPoint;
		point->x = lx;
		point->y = ly;
		lx = lx + nTileSize;
		if (lx >= s_nPicWidth)
		{
			lx = 0;
			ly = ly + nTileSize;
		}

		points.Add(point);
	}

	int nDelayTime = 2;
	if (s_nPicHeight * s_nPicWidth > 600 * 500)
		nDelayTime = 1;

	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);

	//初始化一个以微秒为单位的时间种子
	srand((int)seed.QuadPart);

	for (int i = nTileCount - 1; i >= 0; i--)
	{
		int n = rand() % (i + 1);
		CPoint* point = (CPoint*)points[n];

		lx = point->x;
		ly = point->y;

		pDC->BitBlt(lx + s_nOffsetX, ly + s_nOffsetY, nTileSize, nTileSize, 
					pMemDC, lx + s_nOffsetX, ly + s_nOffsetY, SRCCOPY);

		SAFE_DELETE (point);
		points.RemoveAt(n);

		DelayTime(nDelayTime);
	}
}

//--------------------------------------------
//	作用:		雨滴特效
//--------------------------------------------
void EffectDisplay::RaindropDisplay(CDC* pDC, CDC* pMemDC)
{
	// 扫描高度
	int nHeight = min(s_nPicHeight, s_nCDCHeight);

	// 扫描宽度
	int nWidth = min(s_nPicWidth, s_nCDCWidth);

	for (int i = 0; i <= nHeight + s_nOffsetY; i++)
	{
		for (int j = 0; j <= nHeight + s_nOffsetY - i; j++)
		{
			pDC->BitBlt(s_nOffsetX, j, nWidth, 1, 
						pMemDC, s_nOffsetX, nHeight + s_nOffsetY - i, SRCCOPY);
		}

		DelayTime(1);
	}
}