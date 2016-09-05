// cdib.cpp

#include "stdafx.h"
#include "math.h"
#include "process.h"

#include "cdib.h"
#include "GlobalApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 声明串行化过程
IMPLEMENT_SERIAL(CDib, CObject, 0);


/*************************************************************************
 *
 * \函数名称：
 *   CDib()
 *
 * \输入参数:
 * 无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   构造函数
 *
 ************************************************************************
 */
CDib::CDib()
{
	m_hFile      = NULL;
	m_hBitmap    = NULL;
	m_hPalette   = NULL;
	m_nBmihAlloc = m_nImageAlloc = noAlloc;
	Empty();
}


/*************************************************************************
 *
 * \函数名称：
 *   CDib()
 *
 * \输入参数:
 * CSize	size			- 位图尺寸
 * int		nBitCount		- 象素位数
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   构造函数
 *   根据给定的位图尺寸和象素位数构造CDib对象，并对信息头和调色板分配内存
 *   但并没有给位图数据分配内存
 *
 ************************************************************************
 */
CDib::CDib(CSize size, int nBitCount)
{
	m_hFile      = NULL;
	m_hBitmap    = NULL;
	m_hPalette   = NULL;
	m_nBmihAlloc = m_nImageAlloc = noAlloc;
	Empty();

	// 根据象素位数计算调色板尺寸
	ComputePaletteSize(nBitCount);

	// 分配DIB信息头和调色板的内存
	m_lpBMIH = (LPBITMAPINFOHEADER) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries];

	// 设置信息头内存分配状态
	m_nBmihAlloc = crtAlloc;

	// 设置信息头中的信息
	m_lpBMIH->biSize	= sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth	= size.cx;
	m_lpBMIH->biHeight	= size.cy;
	m_lpBMIH->biPlanes	= 1;
	m_lpBMIH->biBitCount	= nBitCount;
	m_lpBMIH->biCompression = BI_RGB;
	m_lpBMIH->biSizeImage	= 0;
	m_lpBMIH->biXPelsPerMeter = 0;
	m_lpBMIH->biYPelsPerMeter = 0;
	m_lpBMIH->biClrUsed	= m_nColorTableEntries;
	m_lpBMIH->biClrImportant= m_nColorTableEntries;

	// 计算图象数据内存的大小，并设置此DIB的调色板的指针
	ComputeMetrics();

	// 将此DIB的调色板初始化为0
	memset(m_lpvColorTable, 0, sizeof(RGBQUAD) * m_nColorTableEntries);

	// 暂时不分配图象数据内存
	m_lpImage = NULL; 
}


/*************************************************************************
 *
 * \函数名称：
 *   ~CDib()
 *
 * \输入参数:
 * 无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   析构函数，并释放所有分配的DIB内存
 *
 ************************************************************************
 */
CDib::~CDib()
{
	Empty();
}


/*************************************************************************
 *
 * \函数名称：
 *   GetDimensions()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   CSize				- DIB的宽度和高度
 *
 * \说明:
 *   返回以象素表示的DIB的宽度和高度
 *
 ************************************************************************
 */
CSize CDib::GetDimensions()
{	
	if(m_lpBMIH == NULL) return CSize(0, 0);
	return CSize((int) m_lpBMIH->biWidth, (int) m_lpBMIH->biHeight);
}


/*************************************************************************
 *
 * \函数名称：
 *   AttachMapFile()
 *
 * \输入参数:
 *   const	char* strPathname		- 映射文件的路径名
 *   BOOL	bShare				- 如果文件以共享形式打开，设置为TRUE
 *						- 默认值为FALSE
 *
 * \返回值:
 *   BOOL					- 如果成功，则为TRUE 
 *
 * \说明:
 *   以读模式打开内存映射文件，并将其与CDib对象进行关联。因为在文件使用之前并没有读入内存，
 *   所以它立即返回。不过，当访问这个DIB的时候，可能会有一些延迟，因为文件是分页的。
 *   DetachMapFile函数可以释放现有的已分配的内存，并关闭以前关联的任何内存映射文件。
 *   用内存中的DIB与已有的CDib对象关联。此内存可能是程序的资源，或者是可能是剪贴板
 *   或者OLE数据对象内存。内存可能已经由CRT堆栈用new运算符分配了，或者可能已经由
 *   Windows堆栈用GlobalAlloc分配了。
 *   如果打开相同的文件两次，则Windows以另一个文件来对待 
 *
 ************************************************************************
 */
BOOL CDib::AttachMapFile(const char* strPathname, BOOL bShare) // for reading
{
	// 获取文件句柄，并设置打开模式为共享
	HANDLE hFile = ::CreateFile(strPathname, GENERIC_WRITE | GENERIC_READ,
		bShare ? FILE_SHARE_READ : 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(hFile != INVALID_HANDLE_VALUE);

	// 获取文件的尺寸
	DWORD dwFileSize = ::GetFileSize(hFile, NULL);

	// 创建文件映射对象，并设置文件映射的模式为读写
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	DWORD dwErr = ::GetLastError();
	if(hMap == NULL) {
		AfxMessageBox("Empty bitmap file");
		return FALSE;
	}

	// 映射整个文件，注意FILE_MAP_WRITE为读写模式
	LPVOID lpvFile = ::MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0); // map whole file
	ASSERT(lpvFile != NULL);

	// 确认为bmp格式文件
	if(((LPBITMAPFILEHEADER) lpvFile)->bfType != 0x4d42) {
		AfxMessageBox("Invalid bitmap file");
		DetachMapFile();
		return FALSE;
	}

	// 将内存中的DIB与已有的CDib对象关联
	AttachMemory((LPBYTE) lpvFile + sizeof(BITMAPFILEHEADER));

	// 将这些有用的句柄设置为类数据成员
	m_lpvFile = lpvFile;
	m_hFile   = hFile;
	m_hMap    = hMap;

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   CopyToMapFile()
 *
 * \输入参数:
 *   const	char* strPathname		- 映射文件的路径名
 *
 * \返回值:
 *   BOOL					- 如果成功，则为TRUE 
 *
 * \说明:
 *   该函数可以创建一个新的内存映射文件，并将现有的CDib数据复制到该文件的内存
 *   释放以前的所有内存。并关闭现有的所有内存映射文件。实际上，直到新文件
 *   关闭的时候，才将这个数据写到磁盘，但是，当CDib对象被重复使用或被破坏
 *   时，也会发生写磁盘操作
 *
 ************************************************************************
 */
BOOL CDib::CopyToMapFile(const char* strPathname)
{
	BITMAPFILEHEADER bmfh;

	// 设置文件头信息
	bmfh.bfType	= 0x4d42;  
	bmfh.bfSize	= m_dwSizeImage + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries + sizeof(BITMAPFILEHEADER);
	bmfh.bfReserved1= bmfh.bfReserved2 = 0;
	bmfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries;	
	
	// 创建接收数据的文件
	HANDLE hFile	= ::CreateFile(strPathname, GENERIC_WRITE | GENERIC_READ, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(hFile != INVALID_HANDLE_VALUE);

	// 计算文件的大小尺寸
	int nSize	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
				sizeof(RGBQUAD) * m_nColorTableEntries +  m_dwSizeImage;

	// 创建内存映射文件对象
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, nSize, NULL);
	DWORD dwErr = ::GetLastError();
	ASSERT(hMap != NULL);

	// 映射整个文件
	LPVOID lpvFile = ::MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0); 
	ASSERT(lpvFile != NULL);

	// 临时文件指针
	LPBYTE lpbCurrent = (LPBYTE) lpvFile;

	// 拷贝文件头信息到内存映射文件中
	memcpy(lpbCurrent, &bmfh, sizeof(BITMAPFILEHEADER)); 

	// 计算信息头在文件中的地址，并拷贝信息头信息
	lpbCurrent += sizeof(BITMAPFILEHEADER);
	LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) lpbCurrent;
	memcpy(lpbCurrent, m_lpBMIH,
		sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries); 

	// 计算调色板在文件中的地址，并拷贝调色板
	lpbCurrent += sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
	memcpy(lpbCurrent, m_lpImage, m_dwSizeImage); 

	// 暂时存放图象数据尺寸变量
	DWORD dwSizeImage = m_dwSizeImage;

	// 释放一起分配的所有内存
	Empty();

	// 设置图象数据尺寸并设置内存分配状态
	m_dwSizeImage = dwSizeImage;
	m_nBmihAlloc  = m_nImageAlloc = noAlloc;

	// 信息头指针重新指向文件中的位置
	m_lpBMIH = lpBMIH;

	// 图象数据指针重新指向文件中的数据地址
	m_lpImage = lpbCurrent;

	// 设置文件句柄
	m_hFile = hFile;

	// 设置映射对象句柄
	m_hMap = hMap;

	// 设置映射文件指针
	m_lpvFile = lpvFile;

	// 重新计算得到调色板尺寸
	ComputePaletteSize(m_lpBMIH->biBitCount);

	// 重新计算图象数据块大小，并设置调色板指针
	ComputeMetrics();

	// 如果调色板存在的话，读取并创建一个Windows调色板
	MakePalette();

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   AttachMemory()
 *
 * \输入参数:
 *   LPVOID	lpvMem			- 要关联的内存地址
 *   BOOL	bMustDelete		- 如果CDib类负责删除这个内存，标记为TRUE
 *					- 默认值为FALSE
 *   HGLOBAL	hGlobal			- 如果内存是通过Win32 GlobalAlloc得到的，
 *					- 则CDib对象必须保存该句柄，这样，以后
 *					- 可以释放句柄。这里假设bMustDelete设置为TRUE
 *
 * \返回值:
 *   BOOL					- 如果成功，则为TRUE 
 *
 * \说明:
 *   用内存中的DIB与已有的CDib对象关联。此内存可能是程序的资源，或者是可能是剪贴板
 *   或者OLE数据对象内存。内存可能已经由CRT堆栈用new运算符分配了，或者可能已经由
 *   Windows堆栈用GlobalAlloc分配了。
 *
 ************************************************************************
 */
BOOL CDib::AttachMemory(LPVOID lpvMem, BOOL bMustDelete, HGLOBAL hGlobal)
{
	// 首先释放已经分配的内存
	Empty();

	m_hGlobal = hGlobal;
	
	// bMustDelete为TRUE表示此CDib类分配的内存，负责删除
	// 否则的设置信息头分配状态为noAlloc
	if(bMustDelete == FALSE) {
		m_nBmihAlloc = noAlloc;
	}
	else {
		m_nBmihAlloc = ((hGlobal == NULL) ? crtAlloc : heapAlloc);
	}
	try {
		// 设置信息头指针
		m_lpBMIH = (LPBITMAPINFOHEADER) lpvMem;

		// 重新计算得到图象数据块的大小，并设置调色板的指针
		ComputeMetrics();

		// 计算调色板的尺寸
		ComputePaletteSize(m_lpBMIH->biBitCount);

		// 设置图象数据指针
		m_lpImage = (LPBYTE) m_lpvColorTable + sizeof(RGBQUAD) * m_nColorTableEntries;

		// 如果调色板存在的话，读取它，并创建一个Windows调色板，
		// 并将调色板的句柄存放在数据成员中
		MakePalette();
	}

	// 错误处理
	catch(CException* pe) {
		AfxMessageBox("AttachMemory error");
		pe->Delete();
		return FALSE;
	}

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   UsePalette()
 *
 * \输入参数:
 *   CDC*	pDC			- 要实现调色板的设备上下文指针
 *   BOOL	bBackground		- 如果标记为FALSE（默认值），并且应用
 *					- 程序正在前台运行，则Windows将把该调
 *					- 色板作为前台调色板来实现（向系统调色
 *					- 板中复制尽可能多的颜色）。如果标记为
 *					- TURE，则Windows将把该调色板作为后台
 *					- 调色板来实现（尽可能相系统调色板映射
 *					- 逻辑调色板）
 *
 * \返回值:
 *   UINT				- 如果成功，则返回映射到系统调色板的逻
 *					- 辑调色板的表项数，否则返回GDI_ERROR 
 *
 * \说明:
 *   该函数将CDib对象的逻辑调色板选入设备上下文，然后实现该调色板。Draw成员函
 *   数在绘制DIB之前调用UsePalette。
 *
 ************************************************************************
 */
UINT CDib::UsePalette(CDC* pDC, BOOL bBackground /* = FALSE */)
{
	// 判断是否存在调色板
	if(m_hPalette == NULL) return 0;

	// 得到设备上下文句柄
	HDC hdc = pDC->GetSafeHdc();

	// 选择调色板到设备上下文
	::SelectPalette(hdc, m_hPalette, bBackground);

	// 实现该调色板
	return ::RealizePalette(hdc);
}


/*************************************************************************
 *
 * \函数名称：
 *   Draw()
 *
 * \输入参数:
 *   CDC*	pDC			- 指向将要接收DIB图象的设备上下文指针
 *   CPoint	origin			- 显示DIB的逻辑坐标
 *   CSize	size			- 显示矩形的宽度和高度
 *
 * \返回值:
 *   BOOL				- 如果成功，则为TRUE，
 *
 * \说明:
 *   通过调用Win32 SDK的StretchDIBits函数将CDib对象输出到显示器（或者打印机）。
 *   为了适合指定的矩形，位图可以进行必要的拉伸
 *
 ************************************************************************
 */
BOOL CDib::Draw(CDC* pDC, CPoint origin, CSize size)
{
	// 如果信息头为空，表示尚未有数据，返回FALSE
	if(m_lpBMIH == NULL) return FALSE;

	// 如果调色板不为空，则将调色板选入设备上下文
	if(m_hPalette != NULL) {
		::SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
	}

	// 设置显示模式
	pDC->SetStretchBltMode(COLORONCOLOR);

	// 在设备的origin位置上画出大小为size的图象
	::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y,size.cx,size.cy,
		0, 0, m_lpBMIH->biWidth, m_lpBMIH->biHeight,
		m_lpImage, (LPBITMAPINFO) m_lpBMIH, DIB_RGB_COLORS, SRCCOPY);

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   CreateSection()
 *
 * \输入参数:
 *   CDC*	pDC			- 设备上下文指针
 *
 * \返回值:
 *   HBITMAP				- 到GDI位图的句柄。如果不成功，则为NULL。
 *					- 该句柄也是作为公共数据成员存储的
 *
 * \说明:
 *   通过调用Win32 SDK的CreateDIBSection函数创建一个DIB段。图象内存将不被初始化
 *
 ************************************************************************
 */
HBITMAP CDib::CreateSection(CDC* pDC /* = NULL */)
{
	// 如果信息头为空，不作任何处理
	if(m_lpBMIH == NULL) return NULL;

	// 如果图象数据不存在，不作任何处理
	if(m_lpImage != NULL) return NULL; 

	// 创建一个DIB段
	m_hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO) m_lpBMIH,
		DIB_RGB_COLORS,	(LPVOID*) &m_lpImage, NULL, 0);	
	ASSERT(m_lpImage != NULL);

	// 返回HBIMAP句柄
	return m_hBitmap;
}


/*************************************************************************
 *
 * \函数名称：
 *   MakePalette()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   BOOL				- 如果成功，则为TRUE
 *
 * \说明:
 *   如果颜色表存在的话，该函数将读取它，并创建一个Windows调色板。
 *   HPALETTE存储在一个数据成员中。
 *
 ************************************************************************
 */
BOOL CDib::MakePalette()
{
	// 如果不存在调色板，则返回FALSE
	if(m_nColorTableEntries == 0) return FALSE;

	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);
	TRACE("CDib::MakePalette -- m_nColorTableEntries = %d\n", m_nColorTableEntries);
	
	// 给逻辑调色板分配内存
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		m_nColorTableEntries * sizeof(PALETTEENTRY)];

	// 设置逻辑调色板的信息
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = m_nColorTableEntries;

	// 拷贝DIB中的颜色表到逻辑调色板
	LPRGBQUAD pDibQuad = (LPRGBQUAD) m_lpvColorTable;
	for(int i = 0; i < m_nColorTableEntries; i++) {
		pLogPal->palPalEntry[i].peRed = pDibQuad->rgbRed;
		pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pDibQuad->rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
		pDibQuad++;
	}

	// 创建逻辑调色板
	m_hPalette = ::CreatePalette(pLogPal);

	// 删除临时变量并返回TRUE
	delete pLogPal;
	return TRUE;
}	


/*************************************************************************
 *
 * \函数名称：
 *   SetSystemPalette()
 *
 * \输入参数:
 *   CDC*	pDC			- 设备上下文指针
 *
 * \返回值:
 *   BOOL				- 如果成功，则为TRUE，
 *
 * \说明:
 *   如果16bpp、24bpp或32bppDIB不具备调色板，则该函数可以为CDib对象创建一个逻辑调色板，
 *   它与由CreatehalftonePalette函数返回的调色板相匹配。如果程序在256色调色板显示器上
 *   运行，而你又没有调用SetSystemPalette，那么，你将不具有任何调色板，只有20中标准的
 *   Windows颜色出现在DIB中
 *
 ************************************************************************
 */
BOOL CDib::SetSystemPalette(CDC* pDC)
{
	// 如果DIB不具备调色板，则需要利用系统的调色板
	if(m_nColorTableEntries != 0) return FALSE;
	
	// 为设备上下文创建中间调色板，并将其与CPalette对象连接
	m_hPalette = ::CreateHalftonePalette(pDC->GetSafeHdc());

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   CreateBitmap()
 *
 * \输入参数:
 *   CDC*	pDC			- 设备上下文指针
 *
 * \返回值:
 *   HBITMAP				- 到GDI位图的句柄；如果不成功，则为NULL
 *					- 该句柄不是作为公共数据成员存储的
 *
 * \说明:
 *   从已有的DIB中创建DDB位图。不要将这个函数与CreateSection
 *   弄混了，后者的作用是生成DIB并保存句柄
 *
 ************************************************************************
 */
HBITMAP CDib::CreateBitmap(CDC* pDC)
{
	// 如果不存在图象数据，则返回NULL
	if (m_dwSizeImage == 0) return NULL;

	// 用指定的DIB来创建DDB，并用DIB信息初始化位图的图象位
	HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), m_lpBMIH,
		CBM_INIT, m_lpImage, (LPBITMAPINFO) m_lpBMIH, DIB_RGB_COLORS);
	ASSERT(hBitmap != NULL);

	// 返回DDB位图句柄
	return hBitmap;
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

BOOL CDib::ConvertFromDDB(HBITMAP hBitmap, HPALETTE hPal)
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

	
	// 释放已分配的内存
	Empty();
	
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
	m_nColorTableEntries = nColorTableEntries;

	// 分配DIB信息头和调色板的内存
	m_lpBMIH = (LPBITMAPINFOHEADER) new char
		[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries];
	m_nBmihAlloc = m_nImageAlloc = crtAlloc;

	m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER); 
	m_lpBMIH->biWidth = bm.bmWidth; 
	m_lpBMIH->biHeight = bm.bmHeight; 
	m_lpBMIH->biPlanes = 1; 
	m_lpBMIH->biBitCount = biBitCount; 
	m_lpBMIH->biCompression = BI_RGB; 
	m_lpBMIH->biSizeImage = 0; 
	m_lpBMIH->biXPelsPerMeter = 0; 
	m_lpBMIH->biYPelsPerMeter = 0; 
	m_lpBMIH->biClrUsed = nColorTableEntries;
	m_lpBMIH->biClrImportant = nColorTableEntries;	

	// 获得设备上下文句柄
	hDC=GetDC(NULL);
	   
	// 如果没有指定调色板，则从系统中获得当前的系统调色板
	if(hPal==NULL){
		hPal = GetSystemPalette();
	}
	hPal = SelectPalette(hDC, hPal, FALSE); 
	RealizePalette(hDC); 
 

	// 调用GetDIBits填充信息头，并获得图象数据的尺寸。注意这里图象数据指针为NULL
	GetDIBits( hDC, hBitmap, 0, (UINT)m_lpBMIH->biHeight, NULL, (LPBITMAPINFO)m_lpBMIH, DIB_RGB_COLORS);

	// 如果没有正确的获得图象数据尺寸，则重新计算
	if( m_lpBMIH->biSizeImage == 0 ){
		m_lpBMIH->biSizeImage=(((bm.bmWidth*biBitCount) + 31) / 32 * 4)*bm.bmHeight;
	}

	// 分配存放图象数据的内存
	m_lpImage = (LPBYTE) new char[m_lpBMIH->biSizeImage];

	// 调用GetDIBits加载图象数据，注意这里给出了图象数据指针

	// 如果加载图象数据不成功，则释放已经分配的内存，并返回FALSE
	if( GetDIBits( hDC, hBitmap, 0, (UINT)m_lpBMIH->biHeight, (LPBYTE)m_lpImage,
		(LPBITMAPINFO)m_lpBMIH, DIB_RGB_COLORS) == 0 ){
		//clean up and return NULL
		Empty();

		SelectPalette( hDC, hPal, TRUE );
		RealizePalette( hDC );
		ReleaseDC( NULL, hDC );
		
		return FALSE;
	}

	// 删除临时变量
	SelectPalette(hDC, hPal, TRUE); 
	RealizePalette(hDC); 
	ReleaseDC(NULL, hDC); 
	return TRUE;
}

/*************************************************************************
 *
 * \函数名称：
 *   Compress()
 *
 * \输入参数:
 *   CDC*	pDC			- 设备上下文指针
 *   BOOL	bCompress		- TRUE对应于压缩的DIB，FALSE对应于不压缩的DIB
 *
 * \返回值:
 *   BOOL				- 如果成功，则返回TRUE
 *
 * \说明:
 *   该函数将DIB重新生成为压缩或者不压缩的DIB。在内部，它转换已有的DIB为DDB位图
 *   然后生成一个新的压缩或者不压缩的DIB。压缩仅为4bpp和8bpp的DIB所支持。不能
 *   压缩DIB段
 *
 ************************************************************************
 */
BOOL CDib::Compress(CDC* pDC, BOOL bCompress /* = TRUE */)
{
	// 判断是否为4bpp或者8bpp位图，否则，不进行压缩，返回FALSE
	if((m_lpBMIH->biBitCount != 4) && (m_lpBMIH->biBitCount != 8)) return FALSE;
	
	// 如果为DIB段，也不能支持压缩，返回FALSE
	if(m_hBitmap) return FALSE; 

	TRACE("Compress: original palette size = %d\n", m_nColorTableEntries); 
	
	// 获得设备上下文句柄
	HDC hdc = pDC->GetSafeHdc();

	// 将此DIB的调色板选入设备上下文，并保存以前的调色板句柄
	HPALETTE hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);

	HBITMAP hBitmap;  

	// 创建一个DDB位图，如果不成功，则返回FALSE
	if((hBitmap = CreateBitmap(pDC)) == NULL) return FALSE;

	// 计算信息头加上调色板的大小尺寸，并给它们分配内存
	int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
	LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];

	// 将信息头和调色板拷贝到内存中
	memcpy(lpBMIH, m_lpBMIH, nSize);  // new header

	// 如果需要进行压缩，设置相应的信息，并创建压缩格式的DIB
	if(bCompress) {
		switch (lpBMIH->biBitCount) {
		case 4:
			lpBMIH->biCompression = BI_RLE4;
			break;
		case 8:
			lpBMIH->biCompression = BI_RLE8;
			break;
		default:
			ASSERT(FALSE);
		}

		// 设置位图数据指针为NULL，调用GetDIBits来得到压缩格式的DIB的尺寸
		// 如果不能创建DIB，则进行相应的错误处理。
		if(!::GetDIBits(pDC->GetSafeHdc(), hBitmap, 0, (UINT) lpBMIH->biHeight,
					NULL, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS)) {
			AfxMessageBox("Unable to compress this DIB");			

			// 删除临时变量，并释放已分配内存
	 		::DeleteObject(hBitmap);
			delete [] lpBMIH;

			// 重新将以前的调色板选入，并返回FALSE
			::SelectPalette(hdc, hOldPalette, FALSE);
			return FALSE; 
		}

		// 如果位图数据为空，则进行相应的错误处理
		if (lpBMIH->biSizeImage == 0) {
			AfxMessageBox("Driver can't do compression");

			// 删除临时变量，并释放已分配内存
	 		::DeleteObject(hBitmap);
			delete [] lpBMIH;

			// 重新将以前的调色板选入，并返回FALSE
			::SelectPalette(hdc, hOldPalette, FALSE);
			return FALSE; 
		}

		// 将位图数据尺寸赋值给类的成员变量
		else {
			m_dwSizeImage = lpBMIH->biSizeImage;
		}
	}

	// 如果是解压缩，进行相应的处理
	else {

		// 设置压缩格式为不压缩
		lpBMIH->biCompression = BI_RGB; 

		// 根据位图的宽度和高度计算位图数据内存的大小		
		DWORD dwBytes = ((DWORD) lpBMIH->biWidth * lpBMIH->biBitCount) / 32;
		if(((DWORD) lpBMIH->biWidth * lpBMIH->biBitCount) % 32) {
			dwBytes++;
		}
		dwBytes *= 4;
		
		// 将得到位图数据的大小尺寸保存在类的成员变量中
		m_dwSizeImage = dwBytes * lpBMIH->biHeight; 

		// 将位图数据内存的大小赋值给临时的信息头中的相应的变量
		lpBMIH->biSizeImage = m_dwSizeImage;
	} 

	// 再次调用GetDIBits来生成DIB数据

	// 分配临时存放位图数据
	LPBYTE lpImage = (LPBYTE) new char[m_dwSizeImage];

	// 再次调用GetDIBits来生成DIB数据，注意此时位图数据指针不为空
	VERIFY(::GetDIBits(pDC->GetSafeHdc(), hBitmap, 0, (UINT) lpBMIH->biHeight,
    		lpImage, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS));
	TRACE("dib successfully created - height = %d\n", lpBMIH->biHeight);
	
	// 压缩转换完毕，进行相应的其他处理

	// 删除临时的DDB位图
	::DeleteObject(hBitmap);

	// 释放原来的DIB分配的内存
	Empty();

	// 重新设置图象信息头和图象数据内存分配状态
	m_nBmihAlloc = m_nImageAlloc = crtAlloc;

	// 重新定位信息头和图象数据指针
	m_lpBMIH = lpBMIH;
	m_lpImage = lpImage;

	// 计算图象数据尺寸，并设置DIB中调色板的指针
	ComputeMetrics();

	// 计算DIB中调色板的尺寸
	ComputePaletteSize(m_lpBMIH->biBitCount);

	// 如果DIB中调色板存在的话，读取并创建一个Windows调色板
	MakePalette();

	// 恢复以前的调色板
	::SelectPalette(hdc, hOldPalette, FALSE);
	TRACE("Compress: new palette size = %d\n", m_nColorTableEntries); 

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   Read()
 *
 * \输入参数:
 *   CFile*	pFile			- 指向CFile对象的指针
 *
 * \返回值:
 *   BOOL				- 如果成功，则返回TRUE
 *
 * \说明:
 *   该函数DIB从一个文件读入CDib对象。该文件必须成功打开。如果该文件是BMP文件
 *   读取工作从文件头开始。如果该文件是一个文档，读取工作则从当前文件指针处开始 
 *
 ************************************************************************
 */
BOOL CDib::Read(CFile* pFile)
{
	// 释放已经分配的内存
	Empty();

	// 临时存放信息的变量
	int nCount, nSize;
	BITMAPFILEHEADER bmfh;

	// 进行读操作
	try 
	{
		// 读取文件头
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
			throw new CException;
		}

		// 如果文件类型部位"BM"，则返回并进行相应错误处理
		if(bmfh.bfType != 0x4d42) {
			throw new CException;
		}

		// 计算信息头加上调色板的大小，并分配相应的内存
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
		m_nBmihAlloc = m_nImageAlloc = crtAlloc;

		// 读取信息头和调色板
		nCount = pFile->Read(m_lpBMIH, nSize); 

		// 计算图象数据大小并设置调色板指针
		ComputeMetrics();

		// 计算调色板的表项数
		ComputePaletteSize(m_lpBMIH->biBitCount);

		// 如果DIB中存在调色板，则创建一个Windows调色板
		MakePalette();

		// 分配图象数据内存，并从文件中读取图象数据
		m_lpImage = (LPBYTE) new char[m_dwSizeImage];
		nCount = pFile->Read(m_lpImage, m_dwSizeImage); 
	}

	// 错误处理
	catch(CException* pe) 
	{
		AfxMessageBox("Read error");
		pe->Delete();
		return FALSE;
	}

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   ReadSection()
 *
 * \输入参数:
 *   CFile*	pFile			- 指向CFile对象的指针；对应的磁盘
 *					- 文件中包含DIB
 *   CDC*	pDC			- 设备上下文指针
 *
 * \返回值:
 *   BOOL				- 如果成功，则返回TRUE
 *
 * \说明:
 *   该函数从BMP文件中读取信息头，调用CreateDIBSection来分配图象内存，然后将
 *   图象从该文件读入刚才分配的内存。如果你想从磁盘读取一个DIB，然后通过调用
 *   GDI函数编辑它的话，可以使用该函数。你可以用Write或CopyToMapFile将DIB写
 *   回到磁盘
 *
 ************************************************************************
 */
BOOL CDib::ReadSection(CFile* pFile, CDC* pDC /* = NULL */)
{
	
	// 释放已经分配的内存
	Empty();

	// 临时变量
	int nCount, nSize;
	BITMAPFILEHEADER bmfh;

	// 从文件中读取数据
	try {
		// 读取文件头
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
			throw new CException;
		}

		// 如果文件类型部位"BM"，则返回并进行相应错误处理
		if(bmfh.bfType != 0x4d42) {
			throw new CException;
		}

		// 计算信息头加上调色板的大小，并分配相应的内存
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
		m_nBmihAlloc = crtAlloc;
		m_nImageAlloc = noAlloc;

		// 读取信息头和调色板
		nCount = pFile->Read(m_lpBMIH, nSize); 

		// 如果图象为压缩格式，则不进行后续处理
		if(m_lpBMIH->biCompression != BI_RGB) {
			throw new CException;
		}

		// 计算图象数据大小并设置调色板指针
		ComputeMetrics();

		// 计算调色板的表项数
		ComputePaletteSize(m_lpBMIH->biBitCount);

		// 如果DIB中存在调色板，则创建一个Windows调色板
		MakePalette();

		// 将CDib对象的逻辑调色板选入设备上下文		
		UsePalette(pDC);

		// 创建一个DIB段，并分配图象内存
		m_hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO) m_lpBMIH,
			DIB_RGB_COLORS,	(LPVOID*) &m_lpImage, NULL, 0);
		ASSERT(m_lpImage != NULL);

		// 从文件中读取图象数据
		nCount = pFile->Read(m_lpImage, m_dwSizeImage); 
	}

	// 错误处理
	catch(CException* pe) {
		AfxMessageBox("ReadSection error");
		pe->Delete();
		return FALSE;
	}
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   Write()
 *
 * \输入参数:
 *   CFile*	pFile			- 指向CFile对象的指针
 *
 * \返回值:
 *   BOOL				- 如果成功，则返回TRUE
 *
 * \说明:
 *   该函数把DIB从CDib对象写进文件中。该文件必须成功打开或者创建
 *
 ************************************************************************
 */
BOOL CDib::Write(CFile* pFile)
{
	BITMAPFILEHEADER bmfh;

	// 设置文件头中文件类型为"BM"
	bmfh.bfType = 0x4d42;  

	// 计算信息头和调色板的大小尺寸
	int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;

	// 设置文件头信息
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + m_dwSizeImage;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries;	
	
	// 进行写操作
	try {
		pFile->Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		pFile->Write((LPVOID) m_lpBMIH,  nSizeHdr);
		pFile->Write((LPVOID) m_lpImage, m_dwSizeImage);
	}

	// 错误处理
	catch(CException* pe) {
		pe->Delete();
		AfxMessageBox("write error");
		return FALSE;
	}

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   Serialize()
 *
 * \输入参数:
 *   CArchive&	ar			- 指向应用程序归档对象
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数进行串行化过程，将CDib数据进行读入或者写出
 *
 ************************************************************************
 */
void CDib::Serialize(CArchive& ar)
{
	DWORD dwPos;

	// 获得此归档文件的CFile对象指针
	dwPos = ar.GetFile()->GetPosition();
	TRACE("CDib::Serialize -- pos = %d\n", dwPos);

	// 从归档文件缓冲区中冲掉未写入数据
	ar.Flush();

	// 重新获得此归档文件的CFile对象指针
	dwPos = ar.GetFile()->GetPosition();
	TRACE("CDib::Serialize -- pos = %d\n", dwPos);

	// 确定归档文件是否被存储，是则进行存储	
	if(ar.IsStoring()) {
		Write(ar.GetFile());
	}

	// 否则进行加载
	else {
		Read(ar.GetFile());
	}
}


/*************************************************************************
 *
 * \函数名称：
 *   ComputePaletteSize()
 *
 * \输入参数:
 *   int	nBitCount		- 指向CFile对象的指针
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据位图象素位数计算调色板的尺寸
 *
 ************************************************************************
 */
void CDib::ComputePaletteSize(int nBitCount)
{
	// 如果biClrUsed为零，则用到的颜色数为2的biBitCount次方
	if((m_lpBMIH == NULL) || (m_lpBMIH->biClrUsed == 0)) {
		switch(nBitCount) {
			case 1:
				m_nColorTableEntries = 2;
				break;
			case 4:
				m_nColorTableEntries = 16;
				break;
			case 8:
				m_nColorTableEntries = 256;
				break;
			case 16:
			case 24:
			case 32:
				m_nColorTableEntries = 0;
				break;
			default:
				ASSERT(FALSE);
		}
	}

	// 否则调色板的表项数就是用到的颜色数目
	else {
		m_nColorTableEntries = m_lpBMIH->biClrUsed;
	}

	ASSERT((m_nColorTableEntries >= 0) && (m_nColorTableEntries <= 256)); 
}


/*************************************************************************
 *
 * \函数名称：
 *   ComputeMetrics()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数计算图象位图的尺寸，并对DIB中的调色板的指针进行赋值
 *
 ************************************************************************
 */
void CDib::ComputeMetrics()
{
	// 如果结构的长度不对，则进行错误处理
	if(m_lpBMIH->biSize != sizeof(BITMAPINFOHEADER)) {
		TRACE("Not a valid Windows bitmap -- probably an OS/2 bitmap\n");
		throw new CException;
	}

	// 保存图象数据内存大小到CDib对象的数据成员中
	m_dwSizeImage = m_lpBMIH->biSizeImage;

	// 如果图象数据内存大小为0，则重新计算
	if(m_dwSizeImage == 0) {
		DWORD dwBytes = ((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) / 32;
		if(((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) % 32) {
			dwBytes++;
		}
		dwBytes *= 4;
		m_dwSizeImage = dwBytes * m_lpBMIH->biHeight;	
	}

	// 设置DIB中的调色板指针
	m_lpvColorTable = (LPBYTE) m_lpBMIH + sizeof(BITMAPINFOHEADER);
}


/*************************************************************************
 *
 * \函数名称：
 *   Empty()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数清空DIB，释放已分配的内存，并且必要的时候关闭映射文件
 *
 ************************************************************************
 */
void CDib::Empty()
{
	// 关闭内存映射文件的连接
	DetachMapFile();

	// 根据内存分配的状态，调用相应的函数释放信息头
	if(m_nBmihAlloc == crtAlloc) {
		delete [] m_lpBMIH;
	}
	else if(m_nBmihAlloc == heapAlloc) {
		::GlobalUnlock(m_hGlobal);
		::GlobalFree(m_hGlobal);
	}

	// 释放图象数据内存
	if(m_nImageAlloc == crtAlloc) delete [] m_lpImage;

	// 释放调色板句柄
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);

	// 如果创建了BITMAP，则进行释放
	if(m_hBitmap != NULL) ::DeleteObject(m_hBitmap);

	// 重新设置内存分配状态
	m_nBmihAlloc = m_nImageAlloc = noAlloc;

	// 释放内存后，还需要将指针设置为NULL并将相应的数据设置为0
	m_hGlobal = NULL;
	m_lpBMIH = NULL;
	m_lpImage = NULL;
	m_lpvColorTable = NULL;
	m_nColorTableEntries = 0;
	m_dwSizeImage = 0;
	m_lpvFile = NULL;
	m_hMap = NULL;
	m_hFile = NULL;
	m_hBitmap = NULL;
	m_hPalette = NULL;
}


/*************************************************************************
 *
 * \函数名称：
 *   DetachMapFile()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   函数可以释放现有的已分配的内存，并关闭以前关联的任何内存映射文件。
 *
 ************************************************************************
 */
void CDib::DetachMapFile()
{
	// 如果没有进行内存映射，则不进行处理
	if(m_hFile == NULL) return;

	// 关闭内存映射
	::UnmapViewOfFile(m_lpvFile);

	// 关闭内存映射对象和文件
	::CloseHandle(m_hMap);
	::CloseHandle(m_hFile);
	m_hFile = NULL;
}


/*************************************************************************
 *
 * \函数名称：
 *   PaletteSize()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   DWORD				- 返回调色板的尺寸
 *
 * \说明:
 *   该函数计算机调色板所需的尺寸
 *
 ************************************************************************
 */
WORD CDib::PaletteSize()
{
	// 临时变量
	WORD	       NumColors;
	LPBITMAPINFOHEADER lpbi=m_lpBMIH;

	// 如果biClrUsed为零，且图象象素位数小于8，则计算调色板用到的表项数
	NumColors = 	((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                    ? (int)(1 << (int)(lpbi)->biBitCount)          \
                                    : (int)(lpbi)->biClrUsed);

	// 根据颜色表示的字节数计算调色板的尺寸
	if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
		return NumColors * sizeof(RGBTRIPLE);
	else 
		return NumColors * sizeof(RGBQUAD);

}


/*************************************************************************
 *
 * \函数名称：
 *   IsEmpty()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   BOOL				- 如果信息头和图象数据为空，则返回TRUE
 *
 * \说明:
 *   判断信息头和图象数据是否为空
 *
 ************************************************************************
 */
BOOL CDib::IsEmpty()
{

	if(	m_lpBMIH == NULL&&m_lpImage == NULL)
		return TRUE;
	else
		return FALSE;

}


/*************************************************************************
 *
 * \函数名称：
 *   GetDibSaveDim()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   CSize			- DIB实际存储的高度和宽度
 *
 * \说明:
 *   该函数函数用来得到dib的实际存储宽度（DWORD对齐）
 *
 ************************************************************************
 */

CSize CDib::GetDibSaveDim()
{
	CSize sizeSaveDim;
	sizeSaveDim.cx	= ( m_lpBMIH->biWidth * m_lpBMIH->biBitCount + 31)/32 * 4;
	sizeSaveDim.cy	= m_lpBMIH->biHeight; 
	return sizeSaveDim;

}


/*************************************************************************
 *
 * \函数名称：
 *   GetPixelOffset()
 *
 * \输入参数:
 *   int	x		- 象素在X轴的坐标
 *   int	y		- 象素在Y轴的坐标
 *
 * \返回值:
 *   int			- 返回象素在图象数据块中的真实地址
 *
 * \说明:
 *   该函数得到坐标为(x,y)的象素点的真实地址。由于DIB结构中对图象数据排列的
 *   方式为从下到上，从左到右的，所以需要进行转换。
 *
 ************************************************************************
 */
LONG CDib::GetPixelOffset(int  x, int y)
{
	CSize sizeSaveDim;
	sizeSaveDim = GetDibSaveDim();

	LONG lOffset = (LONG) (sizeSaveDim.cy - y - 1) * sizeSaveDim.cx +
		x  / (8 / m_lpBMIH->biBitCount);
	return lOffset;
}


/*************************************************************************
 *
 * \函数名称：
 *   GetPixel()
 *
 * \输入参数:
 *   int	x		- 象素在X轴的坐标
 *   int	y		- 象素在Y轴的坐标
 *
 * \返回值:
 *   RGBQUAD			- 返回DIB在该点真实的颜色
 *
 * \说明:
 *   该函数得到DIB图象在该点真是的颜色。
 *
 ************************************************************************
 */
RGBQUAD CDib::GetPixel(int x, int y)
{
	// 颜色结构
	RGBQUAD cColor;
	
	// 根据每象素比特数得到此点的象素值
	switch (m_lpBMIH->biBitCount)
	{
		case 1 :
			if (1<<(7-x%8) & *(LPBYTE)(m_lpImage+GetPixelOffset(x, y)))
			{			
				cColor.rgbBlue  = 255;
				cColor.rgbGreen = 255;
				cColor.rgbRed   = 255;
				cColor.rgbReserved =0;
			}
			else
			{			
				cColor.rgbBlue  = 0;
				cColor.rgbGreen = 0;
				cColor.rgbRed   = 0;	
				cColor.rgbReserved =0;
			}
			break;
		case 4 :	
			{
				int nIndex = (*(LPBYTE)(m_lpImage+GetPixelOffset(x, y)) & 
							   (x%2 ? 0x0f : 0xf0)) >> (x%2 ? 0 : 4);
				LPRGBQUAD pDibQuad = (LPRGBQUAD) (m_lpvColorTable) + nIndex;
				cColor.rgbBlue  = pDibQuad->rgbBlue;
				cColor.rgbGreen = pDibQuad->rgbGreen;
				cColor.rgbRed   = pDibQuad->rgbRed;
				cColor.rgbReserved =0;
			}
					break;
		case 8 :	
			{
				int nIndex = *(BYTE*)(m_lpImage+GetPixelOffset(x, y));
				LPRGBQUAD pDibQuad = (LPRGBQUAD) (m_lpvColorTable) + nIndex;
				cColor.rgbBlue  = pDibQuad->rgbBlue;
				cColor.rgbGreen = pDibQuad->rgbGreen;
				cColor.rgbRed   = pDibQuad->rgbRed;
				cColor.rgbReserved =0;
			}
					break;
		default:
				int nIndex = *(BYTE*)(m_lpImage+GetPixelOffset(x, y));					
				cColor.rgbRed   = m_lpImage[nIndex];
				cColor.rgbGreen = m_lpImage[nIndex + 1];
				cColor.rgbBlue  = m_lpImage[nIndex + 2];
				cColor.rgbReserved =0;	
				break;
	}

	// 返回颜色结构
	return cColor;
}
