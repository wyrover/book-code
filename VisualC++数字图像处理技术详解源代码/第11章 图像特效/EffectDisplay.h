#pragma once

// 特效显示枚举
enum EffectDislayType
{
	EDT_SCANDOWN = 0,		// 向下扫描
	EDT_VSCAN,				// 垂直双重扫描
	EDT_MOVERIGHT,			// 向右移动
	EDT_HSMOVE,				// 水平双重移动
	EDT_VBLIND,				// 垂直百叶窗
	EDT_HBLIND,				// 水平百叶窗
	EDT_VRASTER,			// 垂直栅条
	EDT_HRASTER,			// 水平栅条
	EDT_MOSAIC,				// 马赛克
	EDT_RAINDROP,			// 雨滴
	
	EDT_NONE,				// 无特效
	EDT_RAND				// 随机特效
};

class EffectDisplay
{
public:
	static int	s_nOffsetX;		// 图像在内存CDC中的x偏移量
	static int	s_nOffsetY;		// 图像在内存CDC中的y偏移量
	static int	s_nPicWidth;	// 图像的宽度
	static int	s_nPicHeight;	// 图像的高度
	static int	s_nCDCWidth;	// 内存CDC的宽度
	static int	s_nCDCHeight;	// 内存CDC的高度

	static void DelayTime(DWORD dwMilliseconds);

	static void ScanDownDisplay(CDC* pDC, CDC* pMemDC);		// 向下扫描
	static void VSScanDisplay(CDC* pDC, CDC* pMemDC);		// 垂直双重扫描
	static void MoveRightDisplay(CDC* pDC, CDC* pMemDC);	// 向右移动
	static void HSMoveDisplay(CDC* pDC, CDC* pMemDC);		// 水平双重移动
	static void VBlindDisplay(CDC* pDC, CDC* pMemDC);		// 垂直百叶窗
	static void HBlindDisplay(CDC* pDC, CDC* pMemDC);		// 水平百叶窗
	static void	VRasterDisplay(CDC* pDC, CDC* pMemDC);		// 垂直栅条
	static void	HRasterDisplay(CDC* pDC, CDC* pMemDC);		// 水平栅条
	static void	MosaicDisplay(CDC* pDC, CDC* pMemDC);		// 马赛克
	static void	RaindropDisplay(CDC* pDC, CDC* pMemDC);		// 雨滴
};