//======================================================================
// 内容： 设备无关位图类-头文件
// 功能： （1）位图的加载与保存；
//        （2）判断位图是否有效、获取位图的文件名；
//        （3）获取位图的长、宽、大小等信息；
//        （4）判断位图是否含有颜色表、获取位图颜色表、根据颜色表生成调
//             色板、使用颜色数、每个像素所占位数、每行像素所占位数；
//        （5）获取位图数据；
//        （6）显示位图；
//        （7）将彩色位图转换成灰度位图；
//        （8）将灰度位图转换成彩色位图；
// 作者： 李平科
// 联系： lipingke@126.com
// 日期： 2009-7-26
//======================================================================

#pragma once

#include "afx.h"

class CDib : public CObject
{
public:
    // 构造函数
    CDib(void);

    // 析构函数
    ~CDib(void);

    // 从文件加载位图
    BOOL LoadFromFile(LPCTSTR lpszPath);

    // 将位图保存到文件
    BOOL SaveToFile(LPCTSTR lpszPath);

    // 获取位图文件名
    LPCTSTR GetFileName();

    // 获取位图大小
    DWORD GetSize();

    // 获取位图宽度
    UINT GetWidth();

    // 获取位图高度
    UINT GetHeight();

    // 获取位图的宽度和高度
    CSize GetDimension();

    // 获取位图颜色数
    UINT GetNumOfColor();

    // 获取位图颜色表
    LPRGBQUAD GetRgbQuad();

    // 获取位图数据
    LPBYTE GetData();

    // 获取灰度位图数据
    LPBYTE GetGradeData();

    // 获取彩色位图数据
    LPBYTE GetColorData();

    // 根据颜色表生成调色板
    BOOL MakePalette();
      
    // 显示位图
    BOOL Draw(CDC *pDC, CPoint origin, CSize size);

    // 获取每行像素所占字节数
    UINT GetLineByte();

    // 彩色位图转灰度位图
    BOOL RgbToGrade();

    // 灰度位图转彩色位图
    BOOL GradeToRgb();

    // 判断是否含有颜色表
    BOOL HasRgbQuad();

    // 判断位图是否有效
    BOOL IsValid();  

    // 清理空间
    void Empty();

protected:
    // 计算位图颜色数
    UINT CalcNumOfColor();

private:
    // 位图文件名
    char m_fileName[_MAX_PATH];

    // 位图文件头指针
    LPBITMAPFILEHEADER m_lpBmpFileHeader;

    // 位图指针（包含除位图文件头的所有内容）
	LPBYTE m_lpDib; // 需要动态分配和释放

    // 位图信息指针
    LPBITMAPINFO m_lpBmpInfo;

	// 位图信息头指针
	LPBITMAPINFOHEADER m_lpBmpInfoHeader;  

    // 位图颜色表指针
	LPRGBQUAD m_lpRgbQuad; 

    // 位图数据指针
	LPBYTE m_lpData; 

    // 灰度位图数据指针（位图转换时用）
	LPBYTE m_lpGradeData; // 需要动态分配和释放

    // 彩色位图数据指针（位图转换时用）
	LPBYTE m_lpColorData; // 需要动态分配和释放 

    // 颜色表长度
	UINT m_uRgbQuadLength;

    // 位图颜色数
    UINT m_uNumOfColor;

	// 每像素占的位数
	UINT m_uBitCount;  

    // 每行像素所占字节数，为4的倍数
    UINT m_uLineByte;

    // 是否有颜色表
    BOOL m_bHasRgbQuad;
	
	// 调色板句柄
	HPALETTE m_hPalette;

    // 位图是否有效
    BOOL m_bValid;
};
