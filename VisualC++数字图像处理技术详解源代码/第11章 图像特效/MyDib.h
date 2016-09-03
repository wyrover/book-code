#pragma once

class MyDib : public CObject
{
	/*成员变量*/
private:
	//DIB文件
	BITMAPINFO*	DibInfo;
	//像素数组
	void*		DibBits;	

	/*成员函数*/
public:

	//构造函数
	MyDib ( LPCTSTR  dibName);//从文件中读入DIB，参数dibName代表文件全路径名
	MyDib ( CBitmap& ddb, CPalette* palette );//从DDB中获得DIB
	MyDib ( int Width, int Height, int BitsPerPixel );//创建新的DIB

	//析构函数
	~MyDib ( );

	//返回像素数组
	void*		GetBits();
	//返回每个像素的位数
	int			GetBitsPerPixel ( );
	//获得图像的高
	long		GetWidth();
	//获得图像的宽
	long		GetHeight();
	//获得图像每行扫描线所需的字节数
	long		BytesPerLine();
	//获得调色板
	CPalette*	GetPalette();
	//复制调色板
	void		CopyPalette(CPalette&);
	//获得调色板的大小
	int	GetPaletteCount();
	//获得像素数组的大小
	long		GetBodySize();
	//在指定DC上缩放输出
	int			StretchToDC ( CDC& dc, CRect& src, CRect& dst, DWORD rop = SRCCOPY );
	//在指定DC上输出，不改变大小
	int			SetToDC ( CDC& dc, CRect& src, CPoint& dst );
	//保存位图，参数dibName代表文件全路径名
	void		SaveDIB ( LPCTSTR dibName );
};
