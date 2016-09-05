// MyDib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dib.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
* Dib文件头标志（字符串"BM"）
*/
#define DIB_MARKER   ((WORD) ('M' << 8) | 'B') // 用于判断位图的标志宏 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDib::CDib()
{
	InitMembers();
}

CDib::~CDib()
{
	ClearMemory();
}

/*************************************************************************
* 函数名称：DrawDib()
* 参数说明:
*   HDC hDC，输出设备DC
*   LPRECT lpDCRect，绘制矩形区域
*   HGLOBAL hDIB ，DIB对象的句柄
*   LPRECT lpDIBRect ，DIB的输出区域
*   CPalette* pPal，调色板的指针
* 函数类型:BOOL
* 函数功能:该函数主要用来绘制DIB对象   
************************************************************************/

BOOL CDib::DrawDib(HDC     hDC,
				   LPRECT  lpDCRect,
				   HGLOBAL    hDIB,
				   LPRECT  lpDIBRect,
				   CPalette* pPal)
{
	BOOL     bSuccess=FALSE;      // 重画成功标志
	HPALETTE hOldPal=NULL;        // 以前的调色板	
	if (hDIB == NULL)// 判断是否是有效的DIB对象
	{		
		return FALSE;// 不是，则返回
	}	
	lpdib  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);// 锁定DIB	
	lpDIBBits = GetBits(lpdib);// 找到DIB图像像素起始位置	
	if (pPal != NULL)// 获取DIB调色板，并选取到设备环境中它
	{
		HPALETTE hPal = (HPALETTE) pPal->m_hObject;	
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}	
	::SetStretchBltMode(hDC, COLORONCOLOR);// 设置显示模式		
	bSuccess = ::StretchDIBits(hDC,								// 设备环境句柄
		lpDCRect->left,                 //目标X坐标
		lpDCRect->top,                  // 目标Y坐标
		GetRectWidth(lpDCRect),            // 目标宽度
		GetRectHeight(lpDCRect),           // 目标高度
		lpDIBRect->left,                // 源X坐标
		lpDIBRect->top,                 // 源Y坐标
		GetRectWidth(lpDIBRect),           // 源宽度
		GetRectHeight(lpDIBRect),          // 源高度
		lpDIBBits,                      // 指向dib像素的指针
		(LPBITMAPINFO)lpdib,         // 指向位图信息结构的指针
		DIB_RGB_COLORS,                 // 使用的颜色数目
		SRCCOPY);                       // 光栅操作类型	
	::GlobalUnlock(hDIB);	// 解除锁定
	if (hOldPal != NULL)// 恢复系统调色板
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}	
	return bSuccess;
}

/*************************************************************************
* 函数名称：ConstructPalette(HGLOBAL hDIB, CPalette* pPal)
* 函数参数:
*   HGLOBAL hDIB，DIB对象的句柄
*   CPalette* pPal，调色板的指针
* 函数类型:BOOL
* 函数说明:该函数按照DIB创建一个逻辑调色板  
************************************************************************/

BOOL CDib::ConstructPalette(HGLOBAL hDIB, CPalette* pPal)
{	

	HANDLE hLogPal;// 逻辑调色板的句柄	
	int iLoop;// 循环变量	
	BOOL bSuccess = FALSE;// 创建结果	
	if (hDIB == NULL)//判断是否是有效的DIB对象
	{		
		return FALSE;// 返回FALSE
	}	
	lpdib = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);// 锁定DIB
	lpbminfo= (LPBITMAPINFO)lpdib;			
	long wNumColors =GetColorNum(lpdib);// 获取DIB中颜色表中的颜色数目	
	if (wNumColors != 0)
	{		
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)// 分配为逻辑调色板内存
			+ sizeof(PALETTEENTRY)
			* wNumColors);				
		if (hLogPal == 0)// 如果失败则退出
		{			
			::GlobalUnlock((HGLOBAL) hDIB);// 解除锁定			
			return FALSE;
		}		
		LPLOGPALETTE lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);		

		lpPal->palVersion = PalVersion;// 设置调色板版本号				
		lpPal->palNumEntries = (WORD)wNumColors;// 设置颜色数目		
		for (iLoop=0; iLoop<(int)wNumColors;iLoop++)// 读取调色板
		{			
			lpPal->palPalEntry[iLoop].peRed =lpbminfo->bmiColors[iLoop].rgbRed;// 读取三原色分量						
			lpPal->palPalEntry[iLoop].peGreen =lpbminfo->bmiColors[iLoop].rgbGreen;							
			lpPal->palPalEntry[iLoop].peBlue =lpbminfo->bmiColors[iLoop].rgbBlue;								
			lpPal->palPalEntry[iLoop].peFlags =0;// 保留位			
		}		
		bSuccess=pPal->CreatePalette(lpPal);// 按照逻辑调色板创建调色板，并返回指针			
		::GlobalUnlock((HGLOBAL) hLogPal);// 解除锁定				
		::GlobalFree((HGLOBAL) hLogPal);// 释放逻辑调色板
	}		
	::GlobalUnlock((HGLOBAL) hDIB);// 解除锁定		
	return bSuccess;// 返回结果
}

/*************************************************************************
* 函数名称：GetBits(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:LPSTR 
* 函数功能:计算DIB像素的起始位置，并返回指向它的指针
************************************************************************/

LPSTR CDib::GetBits(LPSTR lpdib)
{
	return (lpdib + ((LPBITMAPINFOHEADER)lpdib)->biSize+GetPalSize(lpdib));
	//	return (lpdib + *(LPDWORD)lpdib+GetPalSize(lpdib));
}

/*************************************************************************
* 函数名称：GetWidth(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:DWORD
* 函数功能:该函数返回DIB中图像的宽度
************************************************************************/

DWORD CDib::GetWidth(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biWidth;//返回DIB宽度
}


/*************************************************************************
* 函数名称：GetHeight(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib ，指向DIB对象的指针
* 函数类型:DWORD
* 函数功能:该函数返回DIB中图像的高度
************************************************************************/


DWORD CDib::GetHeight(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biHeight;//返回DIB高度

}
/*************************************************************************
* 函数名称：GetPalSize(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:WORD
* 函数功能:该函数返回DIB中调色板的大小
************************************************************************/

WORD CDib::GetPalSize(LPSTR lpdib)
{	
	return (WORD)(GetColorNum(lpdib) * sizeof(RGBQUAD));// 计算DIB中调色板的大小
}

/*************************************************************************
* 函数名称：GetColorNum(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:WORD
* 函数功能:该函数返回DIB中调色板的颜色的种数
************************************************************************/
WORD CDib::GetColorNum(LPSTR lpdib)
{				
	long dwClrUsed = ((LPBITMAPINFOHEADER)lpdib)->biClrUsed;	// 读取dwClrUsed值	
	if (dwClrUsed != 0)
	{			
		return (WORD)dwClrUsed;// 如果dwClrUsed不为0，直接返回该值
	}		
	WORD wBitCount = ((LPBITMAPINFOHEADER)lpdib)->biBitCount;// 读取biBitCount值		
	switch (wBitCount)// 按照像素的位数计算颜色数目
	{
	case 1:
		return 2;

	case 4:
		return 16;

	case 8:
		return 256;

	default:
		return 0;
	}
}

/*************************************************************************
* 函数名称：GetBitCount(LPSTR lpdib)
* 函数参数:
*   LPSTR lpdib，指向DIB对象的指针
* 函数类型:WORD 
* 函数功能:该函数返回DIBBitCount
************************************************************************/
WORD CDib::GetBitCount(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biBitCount;// 返回位宽
}

/*************************************************************************
* 函数名称：CopyObject (HGLOBAL hGlob)
* 函数参数:
*   HGLOBAL hGlob ，要复制的内存区域
* 函数类型:HGLOBAL
* 函数功能:该函数复制指定的内存区域
************************************************************************/

HGLOBAL CDib::CopyObject (HGLOBAL hGlob)
{
	if (hGlob== NULL)
		return NULL;	
	DWORD dwLen = ::GlobalSize((HGLOBAL)hGlob);// 获取指定内存区域大小		
	HGLOBAL hTemp = ::GlobalAlloc(GHND, dwLen);// 分配新内存空间		
	if (hTemp!= NULL)// 判断分配是否成功
	{	
		void* lpTemp = ::GlobalLock((HGLOBAL)hTemp);// 锁定
		void* lp     = ::GlobalLock((HGLOBAL) hGlob);				
		memcpy(lpTemp, lp, dwLen);// 复制		
		::GlobalUnlock(hTemp);// 解除锁定
		::GlobalUnlock(hGlob);
	}
	return hTemp;
}

/*************************************************************************
* 函数名称：SaveFile(HGLOBAL hDib, CFile& file)
* 函数参数:
*   HGLOBAL hDib，要保存的DIB
*   CFile& file，保存文件CFile
* 函数类型:BOOL
* 函数功能:将指定的DIB对象保存到指定的CFile中
*************************************************************************/

BOOL CDib::SaveFile(HGLOBAL hDib, CFile& file)
{
	if (hDib == NULL)
	{		
		return FALSE;// 如果DIB为空，返回FALSE
	}	
	lpbmihrd = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);// 读取BITMAPINFO结构，并锁定	
	if (lpbmihrd == NULL)
	{		
		return FALSE;// 为空，返回FALSE
	}		
	bmfHeader.bfType = DIB_MARKER;	// 填充文件头	
	dwDIBSize = *(LPDWORD)lpbmihrd + GetPalSize((LPSTR)lpbmihrd);// 文件头大小＋颜色表大小			
	DWORD dwBmBitsSize;// 像素的大小		
	dwBmBitsSize =GetReqByteWidth((lpbmihrd->biWidth)*((DWORD)lpbmihrd->biBitCount)) * lpbmihrd->biHeight;// 大小为Width * Height		
	dwDIBSize += dwBmBitsSize;// 计算后DIB每行字节数为4的倍数时的大小
	lpbmihrd->biSizeImage = dwBmBitsSize;// 更新biSizeImage	
	bmfHeader.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);// 文件大小		
	bmfHeader.bfReserved1 = 0;// 两个保留字
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpbmihrd->biSize// 计算偏移量bfOffBits
		+ GetPalSize((LPSTR)lpbmihrd);
	TRY
	{		
		file.Write(&bmfHeader, sizeof(BITMAPFILEHEADER));// 写文件头				
		file.Write(lpbmihrd, dwDIBSize);// 写DIB头和像素
	}
	CATCH (CFileException, e)
	{		
		::GlobalUnlock((HGLOBAL) hDib);// 解除锁定				
		THROW_LAST();// 抛出异常
	}
	END_CATCH		
		::GlobalUnlock((HGLOBAL) hDib);// 解除锁定	
	return TRUE;// 返回TRUE
}


/*************************************************************************
* 函数名称：LoadFile(CFile& file)
* 函数参数:
*   CFile& file，要读取得文件文件CFile
* 函数类型:HGLOBAL
* 函数功能:将指定的文件中的DIB对象读到指定的内存区域中
*************************************************************************/

HGLOBAL CDib::LoadFile(CFile& file)
{

	DWORD dwFileSize;

	dwFileSize= file.GetLength();// 获取文件大小	
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))// 读取DIB文件头
	{		
		return NULL;// 大小不一致，返回NULL。
	}
	if (bmfHeader.bfType != DIB_MARKER)// 判断是否是DIB对象
	{		
		return NULL;// 如果不是则返回NULL。
	}	
	m_hDib= (HGLOBAL) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwFileSize-sizeof(BITMAPFILEHEADER));// 分配DIB内存
	if (m_hDib== 0)
	{		
		return NULL;// 分配失败，返回NULL
	}
	/////////////////////////////////////////////////////////////////////////
	//给CDib类的成员变量赋值	
	lpdib = (LPSTR) ::GlobalLock((HGLOBAL) m_hDib);// 锁定
	lpbminfo=(BITMAPINFO*)lpdib;
	lpbmihrd=(BITMAPINFOHEADER*)lpdib;
	lpRgbQuag=(RGBQUAD*)(lpdib+lpbmihrd->biSize);
	int m_numberOfColors =GetColorNum((LPSTR)lpbmihrd);
	if (lpbmihrd->biClrUsed == 0)
		lpbmihrd->biClrUsed =m_numberOfColors;
	DWORD colorTableSize = m_numberOfColors *sizeof(RGBQUAD);
	lpDIBBits=lpdib+lpbmihrd->biSize+colorTableSize;
	/////////////////////////////////////////////////////////////////////////

	if (file.Read(lpdib, dwFileSize - sizeof(BITMAPFILEHEADER)) !=// 读像素
		dwFileSize - sizeof(BITMAPFILEHEADER) )//大小不一致
	{					
		::GlobalUnlock((HGLOBAL) m_hDib);	// 解除锁定			
		::GlobalFree((HGLOBAL) m_hDib);	// 释放内存
		return NULL;
	}		
	::GlobalUnlock((HGLOBAL) m_hDib);// 解除锁定		
	return m_hDib;// 返回DIB句柄
}

/*************************************************************************
* 函数名称：GetReqByteWidth(int bits)
* 函数参数:
*   int bits，位数
* 函数类型:int
* 函数功能:获取需要的行字节数，应为4的倍数
*************************************************************************/
int  CDib::GetReqByteWidth(int bits)
{
	int getBytes=(bits + 31) / 32 * 4;
	return  getBytes;
}

/*************************************************************************
* 函数名称：GetRectWidth(LPCRECT lpRect)
* 函数参数:
*   LPCRECT lpRect，指向矩形区域的指针
* 函数类型:long
* 函数功能:获取矩形区域的宽度
*************************************************************************/

long  CDib::GetRectWidth(LPCRECT lpRect)
{
	long nWidth=lpRect->right - lpRect->left;
	return nWidth;
}
/*************************************************************************
* 函数名称：GetRectHeight(LPCRECT lpRect)
* 函数参数:
*   LPCRECT lpRect，指向矩形区域的指针
* 函数类型:long
* 函数功能:获取矩形区域的高度
*************************************************************************/
long  CDib::GetRectHeight(LPCRECT lpRect)
{
	long nHeight=lpRect->bottom - lpRect->top;
	return nHeight;
}

/*************************************************************************
* 函数名称：InitMembers() 
* 函数类型: void
* 函数功能:初始化类的成员变量
*************************************************************************/
void CDib::InitMembers()
{
	m_hDib=NULL;
	lpbmihrd=NULL;	// 指向BITMAPINFO结构的指针
	lpdib=NULL;
	lpDIBBits=NULL;
	dwDIBSize=0;
	lpRgbQuag=NULL;
}

/*************************************************************************
* 函数名称：ClearMemory() 
* 函数类型: void
* 函数功能:复位类的成员变量
*************************************************************************/
void CDib::ClearMemory()
{
	if(m_hDib!=NULL)
		::GlobalFree(m_hDib);
	lpbmihrd=NULL;	// 指向BITMAPINFO结构的指针
	lpdib=NULL;
	dwDIBSize=0;
	lpRgbQuag=NULL;	
	dwDIBSize=0;
}
