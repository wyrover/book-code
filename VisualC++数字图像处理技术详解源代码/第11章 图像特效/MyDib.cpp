// MyDib.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "MyDib.h"


MyDib::MyDib ( LPCTSTR dibName )
: DibBits ( NULL ), DibInfo ( NULL )
{
	//创建CFile对象
	CFile fp ( dibName, CFile::modeRead | CFile::typeBinary );

	BITMAPFILEHEADER bmfileheader;
	BITMAPINFOHEADER bmheader;

	ULONGLONG headpos;
	int PaletteSize = 0;
	int ret, cbHeaderSize;
	//获取文件指针的位置
	headpos = fp.GetPosition();
	//读取BMP文件头
	ret = fp.Read ( &bmfileheader, sizeof(BITMAPFILEHEADER) );
	//如果文件类型标头不是“0x4d42”，表示该文件不是BMP类型文件
	//则提示错误并退出。注意“0x4d42”的字符意义就是“BM”
	if ( bmfileheader.bfType != 0x4d42) 
	{
		AfxMessageBox(L"Error!");
		return;
	}

	//读取BMP文件信息头	
	ret = fp.Read ( &bmheader, sizeof(BITMAPINFOHEADER) );
	//计算RGBQUAD的大小
	switch ( bmheader.biBitCount )
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
	}

	//为BITMAPINFO结构分配内存
	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize*sizeof ( RGBQUAD );
	DibInfo = (BITMAPINFO*) new char [ cbHeaderSize ];
	DibInfo->bmiHeader = bmheader;

	if ( PaletteSize )
	{	
		ret = fp.Read ( &(DibInfo->bmiColors[0]), PaletteSize*sizeof ( RGBQUAD ) );
		if ( ret != int( PaletteSize*sizeof ( RGBQUAD ) ) )
		{
			delete[] DibInfo;
			DibInfo = NULL;
			return;
		}
	}

	//为像素数组分配空间，大小由GetBodySize()决定
	DibBits = (void*) new char[GetBodySize()];
	//把文件指针移动到DIB图像数组
	fp.Seek ( headpos + bmfileheader.bfOffBits, CFile::begin );

	ret = fp.Read ( DibBits, GetBodySize() );
	if ( ret != int ( GetBodySize() ) )
	{
		delete[] DibInfo;
		delete[] DibBits;
		DibInfo = NULL;
		DibBits = NULL;
	}
	//关闭文件
	fp.Close();
}

MyDib::MyDib ( CBitmap& ddb, CPalette* palette )
: DibBits ( NULL ), DibInfo ( NULL )
{
	BITMAP ddbinfo;
	//获得DDB的相关信息
	ddb.GetBitmap ( &ddbinfo );	
	/*下面根据图像的高度宽度等信息，填写DIB所必需的BITMAPINFO*/
	int w = ddbinfo.bmWidth;
	int h = ddbinfo.bmHeight;
	int BitsPerPixel = ddbinfo.bmPlanes*ddbinfo.bmBitsPixel;

	int i;
	int PaletteSize = 0, cbHeaderSize;

	//根据每像素需要几个bit来设置RGBQUAD数组的大小
	switch ( BitsPerPixel )	
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
		//超过8位的DIB不需要调色板
	case 15:
	case 16:
		BitsPerPixel = 16;
		break;
	}

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize * sizeof ( RGBQUAD );
	if ( DibInfo ) delete[] DibInfo;
	//为BITMAPINFO的对象DibInfo分配一段内存空间
	DibInfo = (BITMAPINFO*) new char [ cbHeaderSize ]; 
	//填写BITMAPINFO
	DibInfo->bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
	DibInfo->bmiHeader.biWidth = w;
	DibInfo->bmiHeader.biHeight = h;
	DibInfo->bmiHeader.biPlanes = 1;
	DibInfo->bmiHeader.biBitCount = short(BitsPerPixel);
	DibInfo->bmiHeader.biCompression = BI_RGB;
	DibInfo->bmiHeader.biSizeImage = BytesPerLine() * DibInfo->bmiHeader.biHeight;
	DibInfo->bmiHeader.biXPelsPerMeter =
		DibInfo->bmiHeader.biYPelsPerMeter = 3780;
	DibInfo->bmiHeader.biClrUsed = PaletteSize;
	DibInfo->bmiHeader.biClrImportant = PaletteSize;

	if ( PaletteSize )
	{
		//把RGBQUAD数组中的每个元素全部置0
		for ( i = 0 ; i < PaletteSize ; i ++ )
		{
			DibInfo->bmiColors[i].rgbRed = 0;
			DibInfo->bmiColors[i].rgbGreen = 0;
			DibInfo->bmiColors[i].rgbBlue = 0;
			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	//为DIB像素数组分配空间，大小由GetBodySize()决定
	DibBits = (void*) new char[GetBodySize()];
	//判断是否由调色板，如果有则将palette中的entry
	//复制到BITMAPINFO的RGBQUAD数组中
	if ( palette ) CopyPalette ( *palette );

	// 将DDB中的图像信息复制到DIB中
	// GetDesktopWindow()返回桌面窗口的句柄
	CWnd DesktopWnd;	
	DesktopWnd.Attach ( ::GetDesktopWindow() );
	CWindowDC dtpDC ( &DesktopWnd );
	// 完成从DDB向DIB的转换工作
	::GetDIBits( dtpDC.m_hDC,		
		HBITMAP ( ddb ),	 
		0,		  
		ddbinfo.bmHeight,	
		LPVOID ( DibBits ),	
		DibInfo,
		DIB_RGB_COLORS  
		);
	DesktopWnd.Detach();
}

MyDib::~MyDib()
{
	delete[] DibInfo;
	delete[] DibBits;
}

void* MyDib::GetBits() 
{
	return DibBits; 
}

int MyDib::GetBitsPerPixel () 
{ 
	if( !DibInfo )return 0;
	return DibInfo->bmiHeader.biBitCount; 
}

long MyDib::GetWidth() 
{ 
	return DibInfo->bmiHeader.biWidth; 
}

long MyDib::GetHeight() 
{ 
	return DibInfo->bmiHeader.biHeight; 
}

long MyDib::GetBodySize() 
{ 
	return BytesPerLine() * DibInfo->bmiHeader.biHeight; 
}

int MyDib::GetPaletteCount()
{
	int PaletteSize = 0;
	if (!DibInfo) return 0;
	switch ( DibInfo->bmiHeader.biBitCount )
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
	}
	return PaletteSize;
}

long MyDib::BytesPerLine() 
{ 
	return ((((DibInfo->bmiHeader.biWidth * GetBitsPerPixel())+31)/32)*4);
}

int MyDib::SetToDC ( CDC& dc, CRect& src, CPoint& dst )
{
	return ::SetDIBitsToDevice(
		dc.m_hDC,	
		dst.x,	
		dst.y,	 
		src.Width(),	 
		src.Height(),	 
		src.left,	 
		src.top,	 
		0,	 
		DibInfo->bmiHeader.biHeight,	 
		DibBits,	 
		DibInfo,	 
		DIB_RGB_COLORS	 
		);	
}

int MyDib::StretchToDC ( CDC& dc, CRect& src, CRect& dst, DWORD rop )
{
	return ::StretchDIBits (
		dc.m_hDC,	
		dst.left,	 
		dst.top,	 
		dst.Width(),	 
		dst.Height(),	 
		src.left,	 
		src.top,	 
		src.Width(),	 
		src.Height(),	 
		DibBits,	 
		DibInfo,	
		DIB_RGB_COLORS,	 
		rop		
		);	
}

void MyDib::SaveDIB ( LPCTSTR dibName )
{
	//若DIB为空则提示错误并返回
	if(!DibInfo)
	{
		AfxMessageBox(L"DIB信息不存在！");
		return;
	}

	//通过文件名创建CFile对象
	//若文件打开失败则提示错误并返回
	CFile file;
	if(!file.Open(dibName, CFile::modeCreate | CFile::modeReadWrite))
	{
		AfxMessageBox(L"建立文件失败！");
		return;
	}

	//Bitmap文件头
	BITMAPFILEHEADER bmfileheader;

	long size = BytesPerLine() * DibInfo->bmiHeader.biHeight;
	int PaletteSize = GetPaletteCount();

	//填写BMP文件的数据
	bmfileheader.bfType = 0x4d42;//类型表示"BM"
	bmfileheader.bfReserved1 = bmfileheader.bfReserved2 = 0;//保留字0
	bmfileheader.bfOffBits = 
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+PaletteSize*sizeof( RGBQUAD );
	bmfileheader.bfSize = bmfileheader.bfOffBits + size;

	//将文件头，位图信息和像素数组写入磁盘文件
	file.Write ( &bmfileheader, sizeof(BITMAPFILEHEADER) );
	file.Write ( DibInfo, sizeof(BITMAPINFOHEADER)+PaletteSize*sizeof( RGBQUAD ) );
	file.Write ( DibBits, size );
	//关闭文件
	file.Close();	
}

void MyDib::CopyPalette( CPalette& palette )
{
	int i;
	PALETTEENTRY *pe;

	if ( !DibInfo->bmiHeader.biBitCount ) return;
	if ( palette.GetEntryCount() != DibInfo->bmiHeader.biBitCount ) return;

	pe = new PALETTEENTRY[palette.GetEntryCount()];
	palette.GetPaletteEntries( 0, palette.GetEntryCount(), pe );

	for ( i = 0 ; i < palette.GetEntryCount() ; i ++ )
	{
		DibInfo->bmiColors[i].rgbRed = pe[i].peRed;
		DibInfo->bmiColors[i].rgbGreen = pe[i].peGreen;
		DibInfo->bmiColors[i].rgbBlue = pe[i].peBlue;
		DibInfo->bmiColors[i].rgbReserved = 0;
	}

	delete[] pe;
}