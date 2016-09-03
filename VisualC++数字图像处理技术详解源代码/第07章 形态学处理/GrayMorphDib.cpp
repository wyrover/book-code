//======================================================================
// 文件： GrayMorphDib.cpp
// 内容： 灰度形态学处理基本运算函数-原文件
// 功能： （1）腐蚀运算；
//        （2）膨胀运算；
//        （3）开与闭运算；
//        （4）灰度形态学梯度；
//        （5）Top-Hat变换；
// 
//======================================================================

#include "StdAfx.h"
#include "GrayMorphDib.h"

//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CGrayMorphDib::CGrayMorphDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CGrayMorphDib::~CGrayMorphDib(void)
{
}


//=======================================================
// 函数功能： 灰度图像腐蚀运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================

void CGrayMorphDib::Erosion(int Structure[3][3])
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpTempDIBBits;	// 指向缓存DIB图像的指针
	
	long i,j,n,m;			//循环变量
	unsigned char p,q;	    //像素值,中间变量

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	lpTempDIBBits=new BYTE[lWidth * lHeight];

	//初始化
	memset(lpTempDIBBits, (BYTE)0, lWidth * lHeight);

	//使用自定义的结构元素进行腐蚀
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
			// 的两列像素和最上边和最下边的两行像素

			// 指向原图像第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpTempDIBBits + lWidth * j + i;

			p=q=255;    //因为要找最小值，故将中间变量定义为255
			
			//原图像中各点与对应结构元素中各点相减，
			//找到相减之后差的最小值作为点（i，j）的灰度值
			//注意在DIB图像中内容是上下倒置的
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					p=(unsigned char) (*(lpSrc + (1-m)*lWidth + (n-1))-Structure[m][n]);

					if (p<q )
					{	
						q = p;
					}
				}
			}
			* lpDst= q;
		}

	}

	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpTempDIBBits, lWidth * lHeight);
	//释放内存
	delete lpTempDIBBits;

}

//=======================================================
// 函数功能： 灰度图像膨胀运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================

void CGrayMorphDib::Dilation(int Structure[3][3])
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpTempDIBBits;	// 指向缓存DIB图像的指针
	
	long i,j,n,m;			//循环变量
	unsigned char p,q;	    //像素值,中间变量

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	lpTempDIBBits=new BYTE[lWidth * lHeight];

	//初始化
	memset(lpTempDIBBits, (BYTE)0, lWidth * lHeight);

	//使用自定义的结构元素进行膨胀
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
			// 的两列像素和最上边和最下边的两行像素

			// 指向原图像第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpTempDIBBits + lWidth * j + i;

			p=q=0;    //因为要找最大值，故将中间变量定义为0
			
			//原图像中各点与对应结构元素中各点相加，
			//找到相加之和的最大值作为点（i，j）的灰度值
			//注意在DIB图像中内容是上下倒置的
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					p=(unsigned char) (*(lpSrc + (1-m)*lWidth + (n-1))+Structure[m][n]);

					if (p>q )
					{	
						q = p;
					}
				}
			}
			* lpDst= q;
		}

	}

	// 复制膨胀后的图像
	memcpy(lpDIBBits, lpTempDIBBits, lWidth * lHeight);
	//释放内存
	delete lpTempDIBBits;

}

//=======================================================
// 函数功能： 灰度图像开运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================

void CGrayMorphDib::Opening(int Structure[3][3])
{
	//先腐蚀后膨胀
	Erosion(Structure);
	Dilation(Structure);

}

//=======================================================
// 函数功能： 灰度图像闭运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================

void CGrayMorphDib::Closing(int Structure[3][3])
{
	//先膨胀后腐蚀	
	Dilation(Structure);
	Erosion(Structure);

}

//=======================================================
// 函数功能： 灰度形态学梯度
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================

void CGrayMorphDib::Grads(int Structure[3][3])
{
	LPBYTE	lpSave_of_SrcDIBBits;	// 指向用于保存原图像缓存的指针
	LPBYTE	lpRst_of_DilDIBBits;	// 指向膨胀结果缓存的指针
	long i,j;//循环变量
	
		
	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	//分配内存以保存图像
	lpSave_of_SrcDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_DilDIBBits=new BYTE[lWidth * lHeight];	

	memcpy(lpSave_of_SrcDIBBits,lpDIBBits, lWidth * lHeight);//保存原图像

	//膨胀运算
	Dilation(Structure);

	memcpy(lpRst_of_DilDIBBits,lpDIBBits, lWidth * lHeight);//保存膨胀结果

	memcpy(lpDIBBits, lpSave_of_SrcDIBBits, lWidth * lHeight);//恢复原图像

	Erosion(Structure);//腐蚀运算

	//膨胀结果与腐蚀结果的差即为形态学梯度
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			*(lpDIBBits+lWidth*j+i)=(*(lpRst_of_DilDIBBits+lWidth*j+i))-(*(lpDIBBits+lWidth*j+i));
		}
	}

	//释放内存
	delete	lpSave_of_SrcDIBBits;
	delete  lpRst_of_DilDIBBits;

}

//=======================================================
// 函数功能： 灰度图像Top-Hat变换
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================

void CGrayMorphDib::Top_Hat(int Structure[3][3])
{
	LPBYTE	lpSave_of_SrcDIBBits;	// 指向用于保存原图像缓存的指针
	long i,j;//循环变量
		
		
	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	//分配内存以保存图像
	lpSave_of_SrcDIBBits=new BYTE[lWidth * lHeight];
	
	memcpy(lpSave_of_SrcDIBBits,lpDIBBits, lWidth * lHeight);//保存原图像

	//开运算
	Opening(Structure);


	//原图像与开运算结果的差即为Top-Hat变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			*(lpDIBBits+lWidth*j+i)=(*(lpSave_of_SrcDIBBits+lWidth*j+i))-(*(lpDIBBits+lWidth*j+i));
		}
	}

	//释放内存
	delete	lpSave_of_SrcDIBBits;

}