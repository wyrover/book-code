// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__AC952C3A_9B6B_4319_8D6E_E7F509348A88__INCLUDED_)
#define AFX_DIB_H__AC952C3A_9B6B_4319_8D6E_E7F509348A88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PalVersion   0x300                                 // 调色板版本

class CDib : public CObject  
{
public:
	CDib();
	virtual ~CDib();
	//operations
public:
	// 用于操作DIB的函数声明
	BOOL   DrawDib(HDC, LPRECT,HGLOBAL, LPRECT,CPalette*);//显示位图
	BOOL   ConstructPalette(HGLOBAL,CPalette* );         //构造逻辑调色板
	LPSTR  GetBits(LPSTR);                               //取得位图数据的入口地址
	DWORD  GetWidth(LPSTR);                              //取得位图的宽度
	DWORD  GetHeight(LPSTR);                             //取得位图的高度
	WORD   GetPalSize(LPSTR);                            //取得调色板的大小
	WORD   GetColorNum(LPSTR);                            //取得位图包含的颜色数目
	WORD   GetBitCount(LPSTR);                            //取得位图的颜色深度
	HGLOBAL CopyObject(HGLOBAL);                         //用于复制位图对象

	BOOL   SaveFile(HGLOBAL , CFile&);                    //存储位图为文件
	HGLOBAL   LoadFile(CFile&);                          //从文件中加载位图


	// 在对图像进行处理时，针对位图的字节宽度必须是4的倍数的这一要求，
	//	我们设计了函数GetRequireWidth，来处理这种比较特殊的情况
	int     GetReqByteWidth(int );                     //转换后的字节数GetRequireByteWidth
	long    GetRectWidth(LPCRECT );                    //取得区域的宽度
	long    GetRectHeight(LPCRECT);                    //取得区域的高度
public:
	void ClearMemory();
	void InitMembers();
public:
	LPBITMAPINFO        lpbminfo;	// 指向BITMAPINFO结构的指针
	LPBITMAPINFOHEADER  lpbmihrd;	//指向BITMAPINFOHEADER结构的指针
	BITMAPFILEHEADER	bmfHeader;  //BITMAPFILEHEADER结构
	LPSTR				lpdib;      //指向DIB的指针
	LPSTR				lpDIBBits;  // DIB像素指针
	DWORD				dwDIBSize;  //DIB大小

	HGLOBAL				m_hDib;//DIB对象的句柄

	RGBQUAD*			lpRgbQuag;//指向颜色表的指针
};

#endif // !defined(AFX_DIB_H__AC952C3A_9B6B_4319_8D6E_E7F509348A88__INCLUDED_)