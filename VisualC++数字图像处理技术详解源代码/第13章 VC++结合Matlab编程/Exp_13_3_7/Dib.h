//======================================================================
// 文件： Dib.h
// 内容： 设备无关位图类-头文件
// 功能： （1）位图的加载与保存；
//        （2）位图信息的获取；
//        （3）位图数据的获取；
//        （3）位图的显示；
//        （4）位图的转换；
//        （5）位图相关判断；
// 作者： 李平科
// 联系： lipingke@126.com
// 日期： 2009-7-26
//======================================================================

#pragma once

#include "afx.h"

class CDib : public CObject
{
public:
    // 构造函数，初始化数据成员
    CDib(void);

    // 析构函数，释放内存空间
    ~CDib(void);

    // 从文件加载位图
    BOOL LoadFromFile(LPCTSTR lpszPath);

    // 将位图保存到文件
    BOOL SaveToFile(LPCTSTR lpszPath);

    // 获取位图文件名
    LPCTSTR GetFileName();

    // 获取位图宽度
    LONG GetWidth();

    // 获取位图高度
    LONG GetHeight();

    // 获取位图的宽度和高度
    CSize GetDimension();  
    
    // 获取位图大小
    DWORD GetSize();

    // 获取单个像素所占位数
    WORD GetBitCount();

    // 获取每行像素所占字节数
    UINT GetLineByte();

    // 获取位图颜色数
    DWORD GetNumOfColor();

    // 获取位图颜色表
    LPRGBQUAD GetRgbQuad();

    // 获取位图数据
    LPBYTE GetData();
      
    // 显示位图
    BOOL Draw(CDC *pDC, CPoint origin, CSize size);

    // 24位彩色位图转8位灰度位图
    BOOL RgbToGray();

    // 8位灰度位图转24位彩色位图
    BOOL GrayToRgb();

    // 数据替换
    BOOL ReplaceData(LPBYTE lpData,BOOL bFlag);

    // 判断是否含有颜色表
    BOOL HasRgbQuad();

    // 判断是否是灰度图
    BOOL IsGray();

    // 判断位图是否有效
    BOOL IsValid();  

protected:
    // 计算位图颜色表长度
    DWORD CalcRgbQuadLength();

    // 根据颜色表生成调色板
    BOOL MakePalette();

    // 清理空间
    void Empty(BOOL bFlag = TRUE);

private:
    // 位图文件名
    char m_fileName[_MAX_PATH];

    // 位图文件头指针    
    LPBITMAPFILEHEADER m_lpBmpFileHeader; // 需要动态分配和释放 

    // 位图指针（包含除位图文件头的所有内容）
	LPBYTE m_lpDib;                       // 需要动态分配和释放

    // 位图信息指针
    LPBITMAPINFO m_lpBmpInfo;

	// 位图信息头指针
	LPBITMAPINFOHEADER m_lpBmpInfoHeader;  

    // 位图颜色表指针
	LPRGBQUAD m_lpRgbQuad; 

    // 位图数据指针
	LPBYTE m_lpData; 

    // 调色板句柄
	HPALETTE m_hPalette;

    // 是否有颜色表
    BOOL m_bHasRgbQuad;

    // 位图是否有效
    BOOL m_bValid;
};
