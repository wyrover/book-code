/*************************************************************************
 * 文件DIBPrcs.cpp用来存放有关DIB的操作函数
 * 文件头存放在GlobalApi.h中
 *************************************************************************
 */

#include "GlobalApi.h"
#include "stdafx.h"
#include "cdib.h"



/*************************************************************************
 *
 * \函数名称：
 *   GetSystemPalette()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   HPALETTE				- 系统调色板句柄
 *
 * \说明:
 *   该函数获得当前正在使用的系统调色板的句柄
 *
 *************************************************************************
 */
HPALETTE GetSystemPalette() 
{ 
	// 设备上下文
	HDC hDC;                

	// 声明调色板句柄、指针等临时变量
	static HPALETTE hPal = NULL;   
	HANDLE hLogPal;         
	LPLOGPALETTE lpLogPal;  

	// 当前系统调色板的颜色数
	int nColors;            
 
    	// 获得当前系统设备上下文
	hDC = GetDC(NULL); 
	if (!hDC) 
		return NULL; 
 
	// 获得当前系统调色板的颜色数目
	nColors = ( 1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));
    
	// 给调色板分配内存
	hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * 
		sizeof(PALETTEENTRY)); 
	if (!hLogPal) 
		return NULL; 
 
	// 得到调色板内存指针
	lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal); 
 
	// 设置调色板信息
	lpLogPal->palVersion = 0x300; 
	lpLogPal->palNumEntries = nColors; 
 
	// 将系统的调色板拷贝到当前的逻辑调色板中
	GetSystemPaletteEntries(hDC, 0, nColors, 
            (LPPALETTEENTRY)(lpLogPal->palPalEntry)); 
 
	// 创建Windows调色板
	hPal = CreatePalette(lpLogPal); 
 
	// 释放已分配内存并删除临时对象
	GlobalUnlock(hLogPal); 
	GlobalFree(hLogPal); 
	ReleaseDC(NULL, hDC); 
 
	// 返回
	return hPal; 
} 
 

/*************************************************************************
 *
 * \函数名称：
 *   CopyPalette
 *
 * \输入参数:
 *   HPALETTE	hPalSrc			- 需要拷贝的源调色板句柄
 *
 * \返回值:
 *   HPALETTE				- 如果操作成功，则返回拷贝的调色板句柄
 *
 * \说明:
 *   该函数将创建一个新的调色板，并从指定的调色板拷贝调色板内容
 *
 *************************************************************************
 */
HPALETTE CopyPalette(HPALETTE hPalSrc)
{
	// 调色板指针，临时变量
	PLOGPALETTE     plogPal;

	// 声明一个调色板句柄和一个临时句柄
	HPALETTE        hPalette;
	HANDLE		hTemp;

	// 调色板表项数
	int             iNumEntries=0;
    
	// 获取调色板中的表项数
	iNumEntries = GetPaletteEntries(hPalSrc, 0, iNumEntries, NULL);
	if (iNumEntries == 0) 
		return (HPALETTE) NULL;

	// 分配调色板内存，得到句柄
	hTemp = GlobalAlloc(GHND, sizeof(DWORD) + sizeof(PALETTEENTRY)*iNumEntries);
	if (! hTemp)
		return (HPALETTE) NULL;

	// 得到调色板的指针
	plogPal = (PLOGPALETTE)GlobalLock(hTemp);
	if (! plogPal) 
		return (HPALETTE) NULL;

	// 设置调色板的信息
	plogPal->palVersion = 0x300;
	plogPal->palNumEntries = (WORD) iNumEntries;
    
	// 获取逻辑调色板中指定范围的调色板表项
	GetPaletteEntries(hPalSrc, 0, iNumEntries, plogPal->palPalEntry);

	// 创建一个Windows调色板
	hPalette = CreatePalette(plogPal);

	// 释放以分配的内存
	GlobalUnlock( hTemp );
	GlobalFree  ( hTemp );

	return hPalette;
}
 



/*************************************************************************
 *
 * \函数名称：
 *   ReadDIB
 *
 * \输入参数:
 *   CFile*			pFile		- 需要打开的DIB文件
 *   LPBITMAPINFOHEADER*	pLpBMIH		- DIB信息头指针的指针
 *   LPBYTE*			pLpImage	- DIB位图数据块指针的指针
 *
 * \返回值:
 *   BOOL				- 如果操作成功，则返回TRUE
 *
 * \说明:
 *   该函数将指定文件中的DIB文件载入，其中信息头和调色板放在*pLpBMIH中
 *   图象数据存放到*pLpImage中。
 *
 *************************************************************************
 */
 BOOL ReadDIB(CFile* pFile, LPBITMAPINFOHEADER* pLpBMIH, LPBYTE* pLpImage)
 {
	 // 临时存放信息的变量
 	int nCount, nSize;
	BITMAPFILEHEADER bmfh;

	// 信息头指针
	//LPBITMAPINFOHEADER lpBMIH;

	// DIB图象数据指针
	//LPBYTE lpImage;

	// 进行读操作
	try 
	{
		// 读取文件头
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
			return false;
			//throw new CException;
		}

		// 如果文件类型部位"BM"，则返回并进行相应错误处理
		if(bmfh.bfType != 0x4d42) {
			return false;
			//throw new CException;
		}

		// 计算信息头加上调色板的大小，并分配相应的内存
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		*pLpBMIH = (LPBITMAPINFOHEADER) new char[nSize];

		// 读取信息头和调色板
		nCount = pFile->Read(*pLpBMIH, nSize); 

		// 计算图象数据大小并设置调色板指针
		if((*pLpBMIH)->biSize != sizeof(BITMAPINFOHEADER)) {
			TRACE("Not a valid Windows bitmap -- probably an OS/2 bitmap\n");
			return false;
			//throw new CException;
		}

		// 如果图象数据内存大小为0，则重新计算
		if((*pLpBMIH)->biSizeImage == 0) {
			DWORD dwBytes = ((DWORD) (*pLpBMIH)->biWidth * (*pLpBMIH)->biBitCount) / 32;
			if(((DWORD) (*pLpBMIH)->biWidth * (*pLpBMIH)->biBitCount) % 32) {
				dwBytes++;
			}
			dwBytes *= 4;
			(*pLpBMIH)->biSizeImage = dwBytes * (*pLpBMIH)->biHeight;	
		}
		
		// 分配图象数据内存，并从文件中读取图象数据
		*pLpImage = (LPBYTE) new char[(*pLpBMIH)->biSizeImage];
		nCount = pFile->Read((*pLpImage), (*pLpBMIH)->biSizeImage); 
	}

	// 错误处理
	catch(CException* pe) 
	{
		AfxMessageBox("Read error");
		pe->Delete();
		return FALSE;
	}

	// 将指针赋值

	// 返回
	return TRUE;


 }


 /*************************************************************************
 *
 * \函数名称：
 *   MakeDIBPalette()
 *
 * \输入参数:
 *   LPVOID		lpvColorTable	- 颜色表指针
 *   LPBITMAPINFOHEADER lpBMIH		- DIB信息头指针
 *
 * \返回值:
 *   HPALETTE				- 如果成功，则调色板句柄
 *
 * \说明:
 *   该函数将读取颜色表，并创建一个Windows调色板，并返回此调色板的句柄
 *
 ************************************************************************
 */
HPALETTE MakeDIBPalette(LPVOID lpvColorTable,  LPBITMAPINFOHEADER lpBMIH)
{
	// 调色板句柄
	HPALETTE hPalette = NULL;				

	// 颜色表颜色数
	int nColorTableEntries;
	
	// 设置DIB中的调色板指针
	// lpvColorTable = (LPBYTE) lpBMIH + sizeof(BITMAPINFOHEADER);

	// 计算调色板的表项数
	if(lpBMIH->biClrUsed == 0) {
		switch(lpBMIH->biBitCount) {
			case 1:
				nColorTableEntries = 2;
				break;
			case 4:
				nColorTableEntries = 16;
				break;
			case 8:
				nColorTableEntries = 256;
				break;
			case 16:
			case 24:
			case 32:
				nColorTableEntries = 0;
				break;
			default:
				break;
		}
	}

	// 否则调色板的表项数就是用到的颜色数目
	else {
		nColorTableEntries = lpBMIH->biClrUsed;
	}

	ASSERT((nColorTableEntries >= 0) && (nColorTableEntries <= 256)); 

	// 如果不存在调色板，则返回FALSE
	if(nColorTableEntries == 0) return FALSE;

	// 给逻辑调色板分配内存
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		nColorTableEntries * sizeof(PALETTEENTRY)];

	// 设置逻辑调色板的信息
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = nColorTableEntries;

	// 拷贝DIB中的颜色表到逻辑调色板
	LPRGBQUAD pDibQuad = (LPRGBQUAD) lpvColorTable;
	for(int i = 0; i < nColorTableEntries; i++) {
		pLogPal->palPalEntry[i].peRed = pDibQuad->rgbRed;
		pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pDibQuad->rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
		pDibQuad++;
	}

	// 创建逻辑调色板
	hPalette = ::CreatePalette(pLogPal);

	// 删除临时变量
	delete pLogPal;

	// 返回调色板句柄
	return hPalette;
}	

 /*************************************************************************
 *
 * \函数名称：
 *   PaintDIB()
 *
 * \输入参数:
 *   CDC*	pDC			- 指向将要接收DIB图象的设备上下文指针 
 *   LPBITMAPINFOHEADER lpBMIH		- DIB信息头指针
 *   LPBYTE		lpImage;	- DIB位图数据块地址 
 *   CPoint	origin			- 显示DIB的逻辑坐标
 *   CSize	size			- 显示矩形的宽度和高度
 *   HPALETTE	hPalette		- 指向DIB的调色板句柄
 *
 * \返回值:
 *   BOOL				- 如果成功，则返回TRUE
 *
 * \说明:
 *   该函数将DIB图象进行显示
 *
 ************************************************************************
 */
BOOL PaintDIB(CDC* pDC, LPBITMAPINFOHEADER lpBMIH, LPBYTE lpImage, CPoint origin, CSize size, HPALETTE hPalette)
{
	if(lpBMIH == NULL) return FALSE;

	// 如果调色板不为空，则将调色板选入设备上下文
	if(hPalette != NULL) {
		::SelectPalette(pDC->GetSafeHdc(), hPalette, TRUE);
	}

	// 设置显示模式
	pDC->SetStretchBltMode(COLORONCOLOR);

	// 在设备的origin位置上画出大小为size的图象
	::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y,size.cx,size.cy,
		0, 0, lpBMIH->biWidth, lpBMIH->biHeight,
		lpImage, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS, SRCCOPY);

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   ConvertDDBToDIB()
 *
 * \输入参数:
 *   HBITMAP	hBitmap			- 指向源数据的BITMAP句柄
 *   CDib*	pDibDst			- 指向转换目标的CDib对象指针
 *
 * \返回值:
 *   BOOL				- 如果操作成功，则返回TRUE
 *
 * \说明:
 *   该函数将源BITMAP类pDibSrc中的数据拷贝到pDibDst中，并对相应的数据成员赋值
 *
 *************************************************************************
 */

BOOL ConvertDDBToDIB(HBITMAP hBitmap, CDib* pDibDest, HPALETTE hPal)
{
	// 声明一个BITMAP结构
	BITMAP bm;

	// 设备上下文
	HDC hDC;
	
	// 象素位数
	WORD biBitCount;
	
	// 调色板表项数
	int nColorTableEntries;

	// 如果hBitmap句柄无效，则返回
	if(!hBitmap){
		return FALSE;
	}

	// 填充图象数据到bm中，其中最后一个参数表示接收这个指定的对象的指针
	if(!GetObject(hBitmap,sizeof(BITMAP),(LPBYTE)&bm)){
		return FALSE;
	}

	// 计算象素位数
	biBitCount=bm.bmPlanes*bm.bmBitsPixel;
	if(biBitCount<=1)
		biBitCount=1;
	else if(biBitCount<=4)
		biBitCount=4;
	else if(biBitCount<=8)
		biBitCount=8;
	else 
		biBitCount=24;

	// 计算调色板的尺寸
	// 如果biClrUsed为零，则用到的颜色数为2的biBitCount次方
	switch(biBitCount) {
		case 1:
			nColorTableEntries = 2;
			break;
		case 4:
			nColorTableEntries = 16;
			break;
		case 8:
			nColorTableEntries = 256;
			break;
		case 16:
		case 24:
		case 32:
			nColorTableEntries = 0;
			break;
		default:
			ASSERT(FALSE);
	}	
	ASSERT((nColorTableEntries >= 0) && (nColorTableEntries <= 256)); 

	// 分配DIB信息头和调色板的内存
	LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) new char
		[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries];

	lpBMIH->biSize = sizeof(BITMAPINFOHEADER); 
	lpBMIH->biWidth = bm.bmWidth; 
	lpBMIH->biHeight = bm.bmHeight; 
	lpBMIH->biPlanes = 1; 
	lpBMIH->biBitCount = biBitCount; 
	lpBMIH->biCompression = BI_RGB; 
	lpBMIH->biSizeImage = 0; 
	lpBMIH->biXPelsPerMeter = 0; 
	lpBMIH->biYPelsPerMeter = 0; 
	lpBMIH->biClrUsed = nColorTableEntries;
	lpBMIH->biClrImportant = nColorTableEntries;	

	// 获得设备上下文句柄
	hDC=GetDC(NULL);

	  // select and realize our palette 
 
	// 如果没有指定调色板，则从系统中获得当前的系统调色板
	if(hPal==NULL){
		hPal = GetSystemPalette();
	}
	hPal = SelectPalette(hDC, hPal, FALSE); 
	RealizePalette(hDC); 
 

	// 调用GetDIBits填充信息头，并获得图象数据的尺寸。注意这里图象数据指针为NULL
	GetDIBits( hDC, hBitmap, 0, (UINT)lpBMIH->biHeight, NULL, (LPBITMAPINFO)lpBMIH, DIB_RGB_COLORS);

	// 如果没有正确的获得图象数据尺寸，则重新计算
	if( lpBMIH->biSizeImage == 0 ){
		lpBMIH->biSizeImage=(((bm.bmWidth*biBitCount) + 31) / 32 * 4)*bm.bmHeight;
	}

	// 分配存放图象数据的内存
	LPBYTE lpImage = (LPBYTE) new char[lpBMIH->biSizeImage];

	// 调用GetDIBits加载图象数据，注意这里给出了图象数据指针

	// 如果加载图象数据不成功，则释放已经分配的内存，并返回FALSE
	if( GetDIBits( hDC, hBitmap, 0, (UINT)lpBMIH->biHeight, (LPBYTE)lpImage,
		(LPBITMAPINFO)lpBMIH, DIB_RGB_COLORS) == 0 ){
		//clean up and return NULL
		delete []lpImage;
		delete []lpBMIH;

		SelectPalette( hDC, hPal, TRUE );
		RealizePalette( hDC );
		ReleaseDC( NULL, hDC );
		
		return FALSE;
	}

	// 将指针保存在CDib对象的数据成员中
	pDibDest->m_lpBMIH  = lpBMIH;
	pDibDest->m_lpImage = lpImage;
	pDibDest->m_nBmihAlloc = pDibDest->m_nImageAlloc = pDibDest->crtAlloc;


	// 删除临时变量
	SelectPalette(hDC, hPal, TRUE); 
	RealizePalette(hDC); 
	ReleaseDC(NULL, hDC); 
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   CopyDIB()
 *
 * \输入参数:
 *   CDib*	pDibSrc			- 指向源数据的CDib对象指针
 *   CDib*	pDibDst			- 指向拷贝目标的CDib对象指针
 *
 * \返回值:
 *   BOOL				- 如果操作成功，则返回TRUE
 *
 * \说明:
 *   该函数将源CDib类pDibSrc中的数据拷贝到pDibDst中，并对相应的数据成员赋值
 *
 *************************************************************************
 */
BOOL CopyDIB(CDib* pDibSrc, CDib* pDibDst)
{
	// 将目的CDib对象清空
	pDibDst->Empty();
	
	// 计算信息头加上调色板的大小，并分配相应的内存
	int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * pDibSrc->m_nColorTableEntries;

	pDibDst->m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSizeHdr];
	pDibDst->m_nBmihAlloc = pDibDst->m_nImageAlloc = pDibDst->crtAlloc;

	try{
		
		// 拷贝信息头和调色板
		memcpy(pDibDst->m_lpBMIH,pDibSrc->m_lpBMIH,nSizeHdr);
		
		
		// 如果结构的长度不对，则进行错误处理
		if(pDibDst->m_lpBMIH->biSize != sizeof(BITMAPINFOHEADER)) {
			TRACE("Not a valid Windows bitmap -- probably an OS/2 bitmap\n");
			return false;
			//throw new CException;
		}

		// 保存图象数据内存大小到CDib对象的数据成员中
		pDibDst->m_dwSizeImage = pDibDst->m_lpBMIH->biSizeImage;

		// 如果图象数据内存大小为0，则重新计算
		if(pDibDst->m_dwSizeImage == 0) {
			DWORD dwBytes = ((DWORD) pDibDst->m_lpBMIH->biWidth * pDibDst->m_lpBMIH->biBitCount) / 32;
			if(((DWORD) pDibDst->m_lpBMIH->biWidth * pDibDst->m_lpBMIH->biBitCount) % 32) {
				dwBytes++;
			}
			dwBytes *= 4;
			pDibDst->m_dwSizeImage = dwBytes * pDibDst->m_lpBMIH->biHeight; 
		}

		// 设置DIB中的调色板指针
		pDibDst->m_lpvColorTable = (LPBYTE) pDibDst->m_lpBMIH + sizeof(BITMAPINFOHEADER);

		// 计算调色板的表项数
		pDibDst->ComputePaletteSize(pDibDst->m_lpBMIH->biBitCount);

		// 如果DIB中存在调色板，则创建一个Windows调色板
		pDibDst->MakePalette();

		// 分配图象数据内存，并拷贝图象数据
		pDibDst->m_lpImage = (LPBYTE) new char[pDibDst->m_dwSizeImage];
		memcpy(pDibDst->m_lpImage, pDibSrc->m_lpImage,pDibDst->m_dwSizeImage);
	}

	catch(CException* pe) 
	{
		AfxMessageBox("Copy DIB error");
		pDibDst->Empty();
		pe->Delete();
		return FALSE;
	}
	
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   CopyScreenToDIB
 *
 * \输入参数:
 *   LPRECT	lpRect			- 需要拷贝的屏幕区域
 *   CDib*	pDibDest		- 指向目标CDib对象的指针
 *
 * \返回值:
 *   BOOL				- 如果操作成功，则返回TRUE
 *
 * \说明:
 *   该函数将指定矩形位置内的屏幕内容拷贝到DIB中源CDib类pDibSrc中的数据拷贝到pDibDst中
 *
 *************************************************************************
 */
BOOL CopyScreenToDIB(LPRECT lpRect, CDib* pDibDest)
{
	// 屏幕设备上下文和内存设备上下文句柄
	HDC         hScrDC, hMemDC;         

	// 声明BITMAP临时句柄和以前的BITMAP句柄
	HBITMAP     hBitmap, hOldBitmap;    

	// 调色板句柄
	HPALETTE    hPalette;       
    
	// 获取矩形区域的坐标
	int         nX, nY, nX2, nY2;

	// DIB图象的高度和宽度
	int         nWidth, nHeight; 

	// 屏幕分辨率
	int         xScrn, yScrn;    
 
        // 如果给定的矩形区域为空，则不进行进一步的处理
	if (IsRectEmpty(lpRect)) 
		return FALSE; 
 
    	// 得到一个屏幕设备上下文
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 

	// 创建与屏幕设备兼容的内存设备上下文
	hMemDC = CreateCompatibleDC(hScrDC); 
 
	// 得到矩形的区域坐标
	nX = lpRect->left; 
	nY = lpRect->top; 
	nX2 = lpRect->right; 
	nY2 = lpRect->bottom; 
 
	// 得到屏幕的分辨率，以便后面的判断处理
	xScrn = GetDeviceCaps(hScrDC, HORZRES); 
	yScrn = GetDeviceCaps(hScrDC, VERTRES); 
 
	// 判断矩形区域是否超出屏幕
 
	if (nX < 0) 
		nX = 0; 
	if (nY < 0) 
	        nY = 0; 
	if (nX2 > xScrn) 
	        nX2 = xScrn; 
	if (nY2 > yScrn) 
	        nY2 = yScrn; 
 
	// 计算DIB图象的高度和宽度
	nWidth = nX2 - nX; 
	nHeight = nY2 - nY; 
 
	// 创建一个与屏幕设备上下文兼容的DDB位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
 
	// 将DDB位图选入内存设备上下文
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
 
	// 将屏幕中指定区域的图象传输到内存设备上下文中
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY); 
 
	// 然后将以前的图象选入，并得到屏幕区域的DDB图象句柄
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap); 
 
	// 将临时的设备上下文删除
	DeleteDC(hScrDC); 
	DeleteDC(hMemDC); 
 
	// 得到当前系统调色板
	hPalette = GetSystemPalette(); 
 
	// 将DDB图象转换为DIB图象
	pDibDest->ConvertFromDDB(hBitmap,hPalette);
	
	// 删除临时对象
	DeleteObject(hPalette); 
	DeleteObject(hBitmap); 
	
	return TRUE;
}
