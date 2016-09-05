#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>
//using namespace std;

// FOURBYTES就是用来计算离4最近的整倍数
#define FOURBYTES(bits)    (((bits) + 31) / 32 * 4)

/**************************************************************************
 *  文件名：ImageCoding.cpp
 *
 *  正交变换API函数库：
 *
 *  WRITE2IMG()				- 256色图象保存为256色IMG文件
 *  LOADIMG()				- 读取指定的256色IMG文件 
 *  DIBBITPLANE()			- 图像位平面分解
 *
 ************************************************************************
*/

// PCX文件头结构
typedef struct{
		 BYTE bManufacturer;
		 BYTE bVersion;
		 BYTE bEncoding;
		 BYTE bBpp;
		 WORD wLeft;
		 WORD wTop;
		 WORD wRight;
		 WORD wBottom;
		 WORD wXResolution;
		 WORD wYResolution;
		 BYTE bPalette[48];
		 BYTE bReserved;
		 BYTE bPlanes;
		 WORD wLineBytes;
		 WORD wPaletteType;
		 WORD wSrcWidth;
		 WORD wSrcDepth;
		 BYTE bFiller[54];
} PCXHEADER;

typedef struct{
		 BYTE bBpp;
		 WORD wLeft;
		 WORD wTop;
		 WORD wRight;
		 WORD wBottom;
		 WORD wXResolution;
		 WORD wYResolution;
} IMGHEADER;

/*************************************************************************
 *
 * 函数名称：
 *   WRITE2IMG()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib对象的指针
 *   CFile& file        - 要保存的文件
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数将指定的图象保存为IMG文件。
 *
 *************************************************************************/
BOOL WINAPI WRITE2IMG(CDib * pDib, CFile& file)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 循环变量
	LONG	i;
	LONG	j;
		
	// 参与预测的象素和当前编码的象素
	BYTE	bCharA;
	BYTE	bCharB;
	BYTE	bCharC;
	BYTE	bCharD;
    
	// 预测值
	int		nTemp;
	
	// 预测后的残差
	int		nDpcm;
			
	// 指向编码后图像数据的指针
	BYTE *	lpDst;		
		
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
			
	/**********************************************************************
	*写入IMG文件头信息
	***********************************************************************
	*/
	IMGHEADER Header4IMG;
	
	// 给文件头赋值
		
	// 像素位数（256色为8位）
	Header4IMG.bBpp = 8;
	
	// 图像相对于屏幕的左上角X坐标（以像素为单位）
	Header4IMG.wLeft = 0;
	
	// 图像相对于屏幕的左上角Y坐标（以像素为单位）
	Header4IMG.wTop = 0;
	
	// 图像相对于屏幕的右下角X坐标（以像素为单位）
	Header4IMG.wRight = lWidth - 1;
	
	// 图像相对于屏幕的右下角Y坐标（以像素为单位）
	Header4IMG.wBottom = lHeight - 1;
	
	// 图像的水平分辨率
	Header4IMG.wXResolution = lWidth;
	
	// 图像的垂直分辨率
	Header4IMG.wYResolution = lHeight;

	// 写入文件头
	file.Write((LPSTR)&Header4IMG, sizeof(IMGHEADER));	
	
	// 编码第0行
	i = 0;
	for ( j = 0; j < lWidth; j++)
	{
		// 指向图象0行j列象素的指针
		lpSrc = (BYTE *)lpDIBBits + lLineBytes * (lHeight - 1 - i) +j ;
		
		// 给bCharD赋值
		bCharD = *lpSrc;

		// 如果是第0行0列，直接将象素值写入
		if(j == 0)
			nDpcm = (int)bCharD;
		// 利用 Dpcm ＝D － A 计算残差 
		else
		{
			bCharA = *(lpSrc - 1);
			nDpcm  = (int)bCharD - (int)bCharA;
		}

		// 将残差写入文件
		file.Write(&nDpcm , sizeof(int));
	}
	
	// 编码第1行到lHeight-1行
	for ( i=1;i<lHeight; i++)
	{
		for ( j = 0; j < lWidth; j++)
		{
			// 指向当前编码元素的指针
			lpSrc = (BYTE *)lpDIBBits + j + lLineBytes * (lHeight - 1 - i);
		
			// 赋值
			bCharD = *lpSrc;
			bCharB = *(lpSrc + lLineBytes);

			// 如果是第一列，利用 残差＝D －B 进行预测
			if(j == 0)
				nDpcm = (int)bCharD - (int)bCharB;
			else
			{
				// 利用(B-C)/2+A计算预测值
				bCharA = *(lpSrc - 1);
				bCharC = *(lpSrc + lLineBytes - 1);
				nTemp  =  (int)((bCharB-bCharC) / 2 + bCharA);
				
				// 如果预测值小于0，直接赋零
				if(nTemp < 0)
					nTemp = 0;
				// 如果预测值大于255，直接赋值255
				else if(nTemp > 255)
					nTemp = 255;
				else
					nTemp = nTemp;

				// 得到残差
				nDpcm = (int)bCharD - nTemp;
			}

			// 将残差写入文件
			file.Write(&nDpcm , sizeof(int));
		}
	}
	
	// 开辟一片缓冲区以保存调色板
	lpDst = new BYTE[769];
	
	// 调色板起始字节
	* lpDst = 0x0C;	

	// 得到图象的调色板
	LPRGBQUAD lpbmc = (LPRGBQUAD)pDib->m_lpvColorTable;
	
	// 读取当前图象的调色板
	for (i = 0; i < 256; i ++)
	{
		
			// 读取DIB调色板红色分量
			lpDst[i * 3 + 1] = lpbmc[i].rgbRed;
			
			// 读取DIB调色板绿色分量
			lpDst[i * 3 + 2] = lpbmc[i].rgbGreen;
			
			// 读取DIB调色板蓝色分量
			lpDst[i * 3 + 3] = lpbmc[i].rgbBlue;
	}
	
	// 写入调色板信息
	file.Write((LPSTR)lpDst, 769);

	// 返回值
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   LOADIMG()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *   CFile& file        - 要读取的文件
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE
 *
 * 说明:
 *   该函数将读取指定的IMG文件
 *
 *************************************************************************/
BOOL WINAPI LOADIMG(CDib * pDib, CFile& file)
{
	// 循环变量
	LONG	i;
	LONG	j;
		
	// 图象高度
	LONG	lHeight;
	
	// 图象宽度
	LONG	lWidth;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 中间变量
	BYTE	bChar;
	int     nTemp;
	
	// 指向源图像象素的指针
	int *	lpSrc;
	
	// 指向编码后图像数据的指针
	BYTE *	lpDst;
	
	// 临时指针
	int *	lpTemp;

	// 用来参与预测的三个象素和当前编码象素
	BYTE	bCharA;
	BYTE	bCharB;
	BYTE	bCharC;

	// IMG头文件
	IMGHEADER Header4IMG;
	
	/**********************************************************************
	*读出IMG文件头信息
	***********************************************************************
	*/
	
	// 尝试读取文件头
	if (file.Read((LPSTR)&Header4IMG, sizeof(IMGHEADER)) 
		!= sizeof(IMGHEADER))
	{
		// 大小不对，返回NULL。
		return NULL;
	}
	
	// 获取图像高度
	lHeight = Header4IMG.wBottom - Header4IMG.wTop + 1;
	
	// 获取图像宽度
	lWidth  = Header4IMG.wRight - Header4IMG.wLeft + 1;
	
	// 计算图像每行的字节数
	lLineBytes = FOURBYTES(lWidth * 8);
		
	// 获得显示图象的头文件
	LPBITMAPINFOHEADER lpBI=pDib->m_lpBMIH;	
		
	// 给图象头文件成员赋值
	lpBI->biSize = 40;
	lpBI->biWidth = lWidth;
	lpBI->biHeight = lHeight;
	lpBI->biPlanes = 1;
	lpBI->biBitCount = 8;
	lpBI->biCompression = BI_RGB;
	lpBI->biSizeImage = lHeight * lLineBytes;
	lpBI->biXPelsPerMeter = Header4IMG.wXResolution;
	lpBI->biYPelsPerMeter = Header4IMG.wYResolution;
	lpBI->biClrUsed = 0;
	lpBI->biClrImportant = 0;
	
	// 分配内存以读取编码后的象素
	lpSrc = new int[(file.GetLength() - sizeof(IMGHEADER)-769) ];
	
	lpTemp = lpSrc;
	
	// 读取编码后的象素
	if (file.Read(lpSrc, file.GetLength() - sizeof(IMGHEADER)-769) 
		!= file.GetLength() - sizeof(IMGHEADER)-769 )
	{
	
		return FALSE;
	}
	
	// 重新分配内存，以适应新的图象大小
	delete 	pDib->m_lpImage;
	pDib->m_lpImage = new unsigned char[lHeight * lLineBytes];

	// CDib类中像素位置
	lpDst = pDib->m_lpImage;
	
	// 解码第0行
	i = 0;
	for(j = 0; j < lWidth; j++)
	{
		if(j==0)
		{
			// 如果是0行0列，编码值就是真实值
			lpDst[j + lLineBytes * (lHeight - 1 - i)] = (BYTE)(*lpTemp);
			lpTemp ++;
		}
		else
		{
			// 利用 D＝A＋残差 得到原来的象素
			lpDst[j+ lLineBytes * (lHeight - 1 - i)]
				= (BYTE)(*lpTemp) + lpDst[j + lLineBytes * (lHeight - 1 - i) - 1];
			lpTemp++;
		}
	}

	// 解码第1行到第lHeight－1行
	for (i = 1; i < lHeight; i++)
    {
		for (j = 0; j < lWidth; j++)
		{
			// 得到象素B的值
			bCharB = lpDst[j + lLineBytes * (lHeight  - i)];
			
			// 解码第一列
			if(j==0)
			{
				// 利用 D＝B＋残差 得到原来的象素值
				lpDst[j+ lLineBytes * (lHeight - 1 - i)] = (BYTE)((*lpTemp) + bCharB);
				lpTemp++;
			}
			
			// 解码剩下的列
			else
			{
				// 利用 D=(B-C)/2 + A + 残差 得到原来的象素值
				bCharA=lpDst[j - 1 + lLineBytes * (lHeight - 1 - i)];
				bCharC=lpDst[j - 1 + lLineBytes * (lHeight - i)];
				
				// 解码时的预测
				nTemp=(int)((bCharB - bCharC) / 2  +bCharA);
				
				// 预测值小于0，直接赋0
				if(nTemp<0)
					nTemp = 0;
				// 预测值大于255，直接赋值255
				else if(nTemp>255)
					nTemp = 255;				 
				else
					nTemp = nTemp;

				// 预测值＋残差
				lpDst[j + lLineBytes * (lHeight - 1 - i)] 
					= (BYTE)(*lpTemp + (BYTE)nTemp);
				lpTemp++;
			}
		}
	}

	// 释放内存
	delete lpSrc;
	lpDst  = NULL;
	
	// 读调色板标志位
	file.Read(&bChar, 1);
	if (bChar != 0x0C)
	{		
		// 返回NULL。
		return FALSE;
	}
	
	// 分配内存以读取编码后的象素
	lpDst = new BYTE[768];
	
	// 图象中调色板的位置
	LPRGBQUAD lpbmc = (LPRGBQUAD)pDib->m_lpvColorTable;
	
	// 读取调色板
	if (file.Read(lpDst, 768) != 768)
	{		
		return FALSE;
	}
	
	// 给调色板赋值
	for (i = 0; i < 256; i++)
	{	
		lpbmc[i].rgbRed   = lpDst[i * 3 + 2];
		lpbmc[i].rgbGreen = lpDst[i * 3 + 1];
		lpbmc[i].rgbBlue  = lpDst[i * 3];
		lpbmc[i].rgbReserved = 0;
	}
	
	// 返回值
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBBITPLANE()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *   CFile& file        - 要读取的文件
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE
 *
 * 说明:
 *   该函数将制定的图象进行位平面分解
 *
 *************************************************************************/
BOOL WINAPI DIBBITPLANE(CDib *pDib,BYTE bBitNum)
{
	// 如果输入的数不符合要求，不进行分解
	if(bBitNum <1 || bBitNum >8)
		return FALSE;
	
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 循环变量
	LONG	i;
	LONG	j;

	// 中间变量
	BYTE bTemp;
	BYTE bA;

	// 二进制第i位对应的十进制值
	BYTE bCount;
				
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;	
	
	bCount = 1<<(bBitNum - 1);
	
	for (i = 0; i<lHeight; i++)
	{
		for ( j = 0; j < lWidth; j++)
		{
			// 指向位图i行j列的指针
			lpSrc = (BYTE *)lpDIBBits + j+lLineBytes * (lHeight - 1 - i);

			// 对位图按灰度码进行分解
			if(bBitNum == 8)
			{
				// 如果求的是位图8，直接取二值的第8位
				bTemp = (*(lpSrc) & bCount) / bCount;
				bTemp = bTemp * 255;
			}
			else
			{
				// 否则进行异或，求得灰度码
				bTemp = (*(lpSrc) & bCount) / bCount;
				bA    = bCount * 2;

				// 第i＋1位图的象素值
				bA = (*(lpSrc) & bA) / bA;

				// 异或
				bTemp = (bTemp ^ bA) * 255;
			}

			// 更新源图象
			*(lpSrc) = bTemp;	
		}
	}

	// 返回值
	return TRUE;
}
