//======================================================================
// 内容： 设备无关位图类-源文件
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

#include "StdAfx.h"
#include "Dib.h"

//=======================================================
// 函数功能： 构造函数
// 输入参数： 无
// 返回值：   无
//=======================================================
CDib::CDib(void)
{
    m_lpBmpFileHeader = NULL;
    m_lpDib = NULL;   
    m_lpBmpInfo = NULL;
    m_lpBmpInfoHeader = NULL;
    m_lpRgbQuad = NULL;
    m_lpData = NULL;
    m_lpGradeData = NULL;
    m_lpColorData = NULL;
    m_hPalette = NULL;
    m_bHasRgbQuad = FALSE;
    m_bValid = FALSE;
}

//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================
CDib::~CDib(void)
{
    // 清理空间
    Empty();
}

//=======================================================
// 函数功能： 从文件加载位图
// 输入参数： lpszPath-待加载位图文件路径
// 返回值：   位图加载结果：TRUE-成功；FALSE-失败
//=======================================================
BOOL CDib::LoadFromFile(LPCTSTR lpszPath)
{
    // 记录位图文件名
    strcpy(m_fileName, lpszPath);

    // 以读模式打开位图文件
    CFile dibFile;
    if(!dibFile.Open(m_fileName, CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // 清理空间
    Empty();

    // 读取位图文件头
    m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
    int nCount = dibFile.Read((void *)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
    if(nCount != sizeof(BITMAPFILEHEADER)) 
    {
        return FALSE;
    } 

    // 判断此文件是不是位图文件（“0x4d42”代表“BM”）
    if(m_lpBmpFileHeader->bfType == 0x4d42)
    {
        // 是位图文件

        // 计算除位图文件头的空间大小，并分配空间
        DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
        m_lpDib = new BYTE[dwDibSize];

        // 读取除位图文件头的所有数据
        dibFile.Read(m_lpDib, dwDibSize);

        // 关闭位图文件
        dibFile.Close();

        // 设置位图信息指针
        m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;

        // 设置位图信息头指针
        m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;

        // 设置每像素占的位数
        m_uBitCount = m_lpBmpInfoHeader->biBitCount;

        // 计算每行像素所占位数
        m_uLineByte = (GetWidth() * m_uBitCount / 8 + 3) / 4 * 4;

        // 设置位图颜色表指针
        m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);

        // 计算位图颜色
        m_uNumOfColor = CalcNumOfColor();

        // 如果位图没有设置位图使用的颜色数，设置它
        if(m_lpBmpInfoHeader->biClrUsed == 0)
        {
            m_lpBmpInfoHeader->biClrUsed = m_uNumOfColor;
        }

        // 计算颜色表长度
        m_uRgbQuadLength = m_uNumOfColor * sizeof(RGBQUAD);

        // 设置位图数据指针
        m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + m_uRgbQuadLength;

        // 判断是否有颜色表
        if(m_lpRgbQuad == (LPRGBQUAD)m_lpData)
        {
            m_lpRgbQuad = NULL;    // 将位图颜色表指针置空
            m_bHasRgbQuad = FALSE; // 无颜色表
        }
        else
        {
            m_bHasRgbQuad = TRUE;  // 有颜色表
            MakePalette();         // 根据颜色表生成调色板
        }        

        // 设置位图大小
        m_lpBmpInfoHeader->biSizeImage = GetSize();

        // 位图有效
        m_bValid = TRUE;

        return TRUE;
    }
    else
    {
        // 不是位图文件
        m_bValid = FALSE;

        return FALSE;
    }     

}

//=======================================================
// 函数功能： 将位图保存到文件
// 输入参数： lpszPath-位图文件保存路径
// 返回值：   位图保存结果：TRUE-成功；FALSE-失败
//=======================================================
BOOL CDib::SaveToFile(LPCTSTR lpszPath)
{
    // 记录位图文件名
    strcpy(m_fileName, lpszPath);

    // 以写模式打开文件
	CFile dibFile;
	if(!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
    {
        return FALSE;
    }

    // 填写文件头结构
    BITMAPFILEHEADER bmpFileHeader;
    bmpFileHeader.bfType = 0x4d42; // "0x4d42" 代表 "BM"
    bmpFileHeader.bfSize = 0;
    bmpFileHeader.bfReserved1 = bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
                              + m_uRgbQuadLength;	

    // 将文件头结构写进文件
    dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));

    // 将文件信息头结构写进文件
    dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

    // 如果有颜色表的话，将颜色表写进文件
    if(m_uRgbQuadLength != 0)
    {
        dibFile.Write(m_lpRgbQuad, m_uRgbQuadLength);
    }                                                        

    // 将位图数据写进文件
    UINT uDataSize = (GetWidth() * m_uBitCount / 8 + 3) / 4 * 4 * GetHeight();
    dibFile.Write(m_lpData, uDataSize);

    // 关闭文件
    dibFile.Close();
		
    return TRUE;
}

//=======================================================
// 函数功能： 获取位图文件名
// 输入参数： 无
// 返回值：   LPCTSTR 位图文件名
//=======================================================
LPCTSTR CDib::GetFileName()
{
    return m_fileName;
}

//=======================================================
// 函数功能： 获取位图大小
// 输入参数： 无
// 返回值：   DWORD 位图大小
//=======================================================
DWORD CDib::GetSize()
{
    if(m_lpBmpInfoHeader->biSizeImage != 0)
    {
        return m_lpBmpInfoHeader->biSizeImage;
    }
    else
    {
        DWORD dwWidth = (DWORD)GetWidth();
        DWORD dwHeight = (DWORD)GetHeight();        
        return dwWidth * dwHeight;
    }
}


//=======================================================
// 函数功能： 获取位图宽度
// 输入参数： 无
// 返回值：   UINT 位图宽度
//=======================================================
UINT CDib::GetWidth()
{
    return (UINT)m_lpBmpInfoHeader->biWidth;
}

//=======================================================
// 函数功能： 获取位图高度
// 输入参数： 无
// 返回值：   UINT 位图高度
//=======================================================
UINT CDib::GetHeight()
{
    return (UINT)m_lpBmpInfoHeader->biHeight;
}

//=======================================================
// 函数功能： 获取位图的宽度和高度
// 输入参数： 无
// 返回值：   位图的宽度和高度
//=======================================================
CSize CDib::GetDimension()
{
    return CSize(GetWidth(), GetHeight());
}

//=======================================================
// 函数功能： 计算位图颜色数
// 输入参数： 无
// 返回值：   UINT 位图颜色数
//=======================================================
UINT CDib::CalcNumOfColor()
{
    UINT uNumOfColor;     

    if ((m_lpBmpInfoHeader->biClrUsed == 0) 
        && (m_lpBmpInfoHeader->biBitCount < 9))
	{
		switch (m_lpBmpInfoHeader->biBitCount)
		{
		    case 1: uNumOfColor = 2; break;
		    case 4: uNumOfColor = 16; break;
		    case 8: uNumOfColor = 256;
		}
	}
    else
    {
        uNumOfColor = (UINT) m_lpBmpInfoHeader->biClrUsed;
    }  		

    return uNumOfColor;

}

//=======================================================
// 函数功能： 获取像素所占位数
// 输入参数： 无
// 返回值：   像素所占位数
//=======================================================
UINT CDib::GetBitCount()
{
    return m_uBitCount;
}

//=======================================================
// 函数功能： 获取位图颜色数
// 输入参数： 无
// 返回值：   位图颜色数
//=======================================================
UINT CDib::GetNumOfColor()
{
    return m_uNumOfColor;

}

//=======================================================
// 函数功能： 获取位图颜色表
// 输入参数： 无
// 返回值：   LPRGBQUAD 位图颜色表指针
//=======================================================
LPRGBQUAD CDib::GetRgbQuad()
{
    return m_lpRgbQuad;
}

//=======================================================
// 函数功能： 获取位图数据
// 输入参数： 无
// 返回值：   LPBYTE 位图数据指针
//=======================================================
LPBYTE CDib::GetData()
{
    return m_lpData;
}

//=======================================================
// 函数功能： 获取灰度位图数据
// 输入参数： 无
// 返回值：   LPBYTE 灰度位图数据指针
//=======================================================
LPBYTE CDib::GetGradeData()
{
    // 含有颜色表时已是灰度位图
    if(GetRgbQuad())
    {
        m_lpGradeData = m_lpData;
    }
    return m_lpGradeData;
}

//=======================================================
// 函数功能： 获取彩色位图数据
// 输入参数： 无
// 返回值：   LPBYTE 彩色位图数据指针
//=======================================================
LPBYTE CDib::GetColorData()
{
    return m_lpColorData;
}

//=======================================================
// 函数功能： 根据颜色表生成调色板
// 输入参数： 无
// 返回值：   生成结果：TRUE-成功；FALSE-失败
//=======================================================
BOOL CDib::MakePalette()
{
    // 如果颜色表长度为0，则不生成逻辑调色板
	if(m_uRgbQuadLength == 0) 
    {
        return FALSE;
    }

	//删除旧的调色板对象
	if(m_hPalette != NULL) DeleteObject(m_hPalette);

	// 申请缓冲区，生成逻辑调色板
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[2 * sizeof(WORD) 
                                + m_uRgbQuadLength * sizeof(PALETTEENTRY)];
	lpLogPalette->palVersion = 0x300;
	lpLogPalette->palNumEntries = m_uRgbQuadLength;
	LPRGBQUAD lpRgbQuad = (LPRGBQUAD) m_lpRgbQuad;
	for(int i = 0; i < m_uRgbQuadLength; i++) {
		lpLogPalette->palPalEntry[i].peRed = lpRgbQuad->rgbRed;
		lpLogPalette->palPalEntry[i].peGreen = lpRgbQuad->rgbGreen;
		lpLogPalette->palPalEntry[i].peBlue = lpRgbQuad->rgbBlue;
		lpLogPalette->palPalEntry[i].peFlags = 0;
		lpRgbQuad++;
	}

	// 创建逻辑调色板
	m_hPalette = CreatePalette(lpLogPalette);

	// 释放缓冲区
	delete lpLogPalette;

    return TRUE;
}

//=======================================================
// 函数功能： 显示位图
// 输入参数：
//            pDC-设备环境指针
//            origin-显示矩形区域的左上角
//            size-显示矩形区域的尺寸
// 返回值：
//            显示结果：TRUE-成功；FALSE-失败
//=======================================================
BOOL CDib::Draw(CDC *pDC, CPoint origin, CSize size)
{
    // 旧的调色板句柄
	HPALETTE hOldPalette = NULL;

	// 如果位图指针为空，则返回FALSE
	if(m_lpDib == NULL) 
    {
        return FALSE;
    }

	// 如果位图有调色板，则选进设备环境中
	if(m_hPalette != NULL) 
    {
		hOldPalette = SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
	}

	// 设置位图伸缩模式
	pDC->SetStretchBltMode(COLORONCOLOR);

	// 将位图在pDC所指向的设备上进行显示
	StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, GetWidth(), GetHeight(), m_lpData, m_lpBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	// 恢复旧的调色板
	if(hOldPalette != NULL)
    {
        SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
    }

    return TRUE;
}

//=======================================================
// 函数功能： 获取每行像素所占字节数
// 输入参数： 无
// 返回值：   每行像素所占字节数
//=======================================================
UINT CDib::GetLineByte()
{
    return m_uLineByte;
}

//=======================================================
// 函数功能： 彩色位图转灰度位图
// 输入参数： 无
// 返回值：   转换结果：TRUE-成功；FALSE-失败
//=======================================================
BOOL CDib::RgbToGrade()
{
    // 颜色表存在时已是灰度图，无须转换，否则需要转换
    if(GetRgbQuad())
    {
        m_lpGradeData = m_lpData;
    }
    else
    {
        BYTE r, g, b;
        
        UINT uHeight = GetHeight();
        UINT uWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // 如果已分配空间，释放原有空间
        if((m_lpGradeData != NULL) && (m_lpGradeData != m_lpData))
        {
            delete [] m_lpGradeData;
        }
        // 重新分配空间
        m_lpGradeData = (LPBYTE)new BYTE[GetSize()];
        
        // 进行转换
        for(int i = 0; i < uHeight; i++)
        {
            for(int j = 0; j < uWidth; j++)
            {
                b = m_lpData[i * m_uLineByte + 3 * j];
                g = m_lpData[i * m_uLineByte + 3 * j + 1];
                r = m_lpData[i * m_uLineByte + 3 * j + 2];
                m_lpGradeData[i * uWidth + j] = (BYTE)(0.3 * r + 0.59 * g + 0.11 * b); 
            }
        }

        // 指向转换换后的位图数据
        m_lpData = m_lpGradeData;

        // 更新位图头信息
        // 每像素所占位数
        m_lpBmpInfoHeader->biBitCount = 8;
        m_uBitCount = 8;
        // 每行像素所占字节数
        m_uLineByte = (GetWidth() * m_uBitCount / 8 + 3) / 4 * 4;

    }

    return TRUE;   
}   

//=======================================================
// 函数功能： 灰度位图转彩色位图
// 输入参数： 无
// 返回值：   转换结果：TRUE-成功；FALSE-失败
//=======================================================
BOOL CDib::GradeToRgb()
{
    // 颜色表存在时是灰度图，需转换，否则不需要转换
    if(GetRgbQuad())
    {
        UINT uHeight = GetHeight();
        UINT uWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // 如果已分配空间，释放原有空间
        if((m_lpColorData != NULL) && (m_lpColorData != m_lpData))
        {
            delete [] m_lpColorData;
        }
        // 重新分配空间
        m_lpColorData = (LPBYTE)new BYTE[GetSize()*3];

        // 进行转换
        for(int i = 0; i < uHeight; i++)
        {
            for(int j = 0; j < uWidth; j++)
            {
                m_lpColorData[(uHeight - i - 1) * m_uLineByte + 3 * j] = 
                    m_lpData[(uHeight - i - 1) * uWidth + i];
                m_lpColorData[(uHeight - i - 1) * m_uLineByte + 3 * j + 1] = 
                    m_lpData[(uHeight - i - 1) * uWidth + i];
                m_lpColorData[(uHeight - i - 1) * m_uLineByte + 3 * j + 2] = 
                    m_lpData[(uHeight - i - 1) * uWidth + i];

            }
        }

        // 指向转换换后的位图数据
        m_lpData = m_lpColorData;

        // 更新位图头信息
        // 每像素所占位数
        m_lpBmpInfoHeader->biBitCount = 24;
        m_uBitCount = 24;
        // 每行像素所占字节数
        m_uLineByte = (GetWidth() * m_uBitCount / 8 + 3) / 4 * 4;

    }        

    return TRUE;   
}   
 
//=======================================================
// 函数功能： 判断是否含有颜色表
// 输入参数： 无
// 返回值：   判断结果：TRUE-含有颜色表；FALSE-不含颜色表
//=======================================================
BOOL CDib::HasRgbQuad()
{
    return m_bHasRgbQuad;
}

//=======================================================
// 函数功能： 判断是否是灰度图
// 输入参数： 无
// 返回值：   判断结果：TRUE-是灰度图；FALSE-是彩色图
//=======================================================
BOOL CDib::IsGrade()
{
    return (m_uBitCount < 9);
}

//=======================================================
// 函数功能： 判断位图是否有效
// 输入参数： 无
// 返回值：   判断结果：TRUE-位图有效；FALSE-位图无效
//=======================================================
BOOL CDib::IsValid()
{
    return m_bValid;
}

//=======================================================
// 函数功能： 清理空间
// 输入参数： 无
// 返回值：   无
//=======================================================
void CDib::Empty()
{
    // 释放位图文件头指针空间
    if(m_lpBmpFileHeader != NULL)
    {
        delete [] m_lpBmpFileHeader;
    }

    // 释放灰度位图数据空间
    if((m_lpGradeData != NULL) && (m_lpGradeData != m_lpData))
    {
        delete [] m_lpGradeData;
    }

    // 释放彩色位图数据空间
    if((m_lpColorData != NULL) && (m_lpColorData != m_lpData))
    {
        delete [] m_lpColorData;
    }

    // 释放位图指针空间
    if(m_lpDib != NULL)
    {
        delete [] m_lpDib;
        m_lpBmpInfo = NULL;
        m_lpBmpInfoHeader = NULL;
        m_lpRgbQuad = NULL;
        m_lpData = NULL;           
    }       

    // 释放调色板
    if(m_hPalette != NULL)
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }    

    // 设置不含颜色表
    m_bHasRgbQuad = FALSE;
    
    // 设置位图无效
    m_bValid = FALSE;

}  
