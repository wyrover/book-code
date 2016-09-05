//======================================================================
// 文件： BinaryMorphDib.cpp
// 内容： 二值形态学处理基本运算函数-源文件
// 功能： （1）腐蚀运算；
//        （2）膨胀运算；
//        （3）开与闭运算；
//        （4）击中与击不中运算；
//        （5）骨架提取算法；
// 
//======================================================================

#include "StdAfx.h"
#include "BinaryMorphDib.h"


//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CBinaryMorphDib::CBinaryMorphDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CBinaryMorphDib::~CBinaryMorphDib(void)
{
	
}


//=======================================================
// 函数功能： 二值图像腐蚀运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CBinaryMorphDib::Erosion(int Structure[3][3])
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j,n,m;			//循环变量
	unsigned char pixel;	//像素值

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//使用结构元素进行腐蚀
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
			// 的两列像素和最上边和最下边的两行像素

			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像倒数第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && *lpSrc != 0)
			{
				return FALSE;
			}

			//目标图像中的当前点先赋成白色
			*lpDst = (unsigned char)255;

			//如果原图像中对应结构元素中为白色的那些点中有一个不是白色，
			//则将目标图像中的当前点赋成黑色
			//注意在DIB图像中内容是上下倒置的
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					if( Structure[m][n] == 0)
				    continue;

					pixel = *(lpSrc + (1-m)*lWidth + (n-1));
					if (pixel == 0 )
					{	
						*lpDst = (unsigned char)0;
						break;
					}
				}
			}			
		}
	}
	
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}


//=======================================================
// 函数功能： 二值图像膨胀运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CBinaryMorphDib::Dilation(int Structure[3][3])
{
	
	LPBYTE	lpSrc;			// 指向原图像的指针	
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j,n,m;			//循环变量
	unsigned char pixel;	//像素值

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	
	lpNewDIBBits = (LPBYTE )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	//使用结构元素进行膨胀
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
			// 的两列像素和最上边和最下边的两行像素

			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			// 指向目标图像倒数第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && *lpSrc != 0)
			{
				return FALSE;
			}

			//目标图像中的当前点先赋成黑色
			*lpDst = (unsigned char)0;

			//原图像中对应结构元素中为白色的那些点中只要有一个是白色，
			//则将目标图像中的当前点赋成白色
			//注意在DIB图像中内容是上下倒置的
			for (m = 0;m < 3;m++ )
			{
				for (n = 0;n < 3;n++)
				{
					if( Structure[m][n] == 0)
				    continue;
					pixel = *(lpSrc + (1-m)*lWidth + (n-1));
					if (pixel == 255 )
					{	
						*lpDst = (unsigned char)255;
						break;
					}
				}
			}				
		}
	}
	
	// 复制膨胀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}


//=======================================================
// 函数功能： 二值图像开运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CBinaryMorphDib::Opening(int Structure[3][3])
{
	//先进行腐蚀运算,不成功返回FALSE
	if(!Erosion(Structure))
	{
		return FALSE;
	}
	//再进行膨胀运算,不成功返回FALSE
    if(!Dilation(Structure))
	{
		return FALSE;
	}

	return TRUE;

}


//=======================================================
// 函数功能： 二值图像闭运算
// 输入参数： int Structure[3][3]-用户定义的3×3结构元素
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CBinaryMorphDib::Closing(int Structure[3][3])
{
	//先进行膨胀运算,不成功返回FALSE
    if(!Dilation(Structure))
	{
		return FALSE;
	}
    //再进行腐蚀运算,不成功返回FALSE
	if(!Erosion(Structure))
	{
		return FALSE;
	}
	
	return TRUE;
   
}


//=======================================================
// 函数功能： 二值图像击中与击不中运算
// 输入参数：int hStructure[3][3]-用户定义的3×3结构元素h
//           int mStructure[3][3]-用户定义的3×3结构元素m  
// 返回值：   无
//=======================================================
void CBinaryMorphDib::Hit_Miss_Transform(int hStructure[3][3], int mStructure[3][3])
{
	long i,j;			//循环变量
	
	LPBYTE	lpSave_of_SrcDIBBits;	// 指向用于保存原图像缓存的指针
	LPBYTE	lpRst_of_hStrDIBBits;	// 指向结构元素h腐蚀图像缓存的指针
	LPBYTE	lpRst_of_mStrDIBBits;   // 指向结构元素m腐蚀图像缓存的指针
		
	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	//分配内存以保存图像
	lpSave_of_SrcDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_hStrDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_mStrDIBBits=new BYTE[lWidth * lHeight];

	memcpy(lpSave_of_SrcDIBBits,lpDIBBits, lWidth * lHeight);//保存原图像

	Erosion(hStructure);//用结构元素h腐蚀原图像

	memcpy(lpRst_of_hStrDIBBits, lpDIBBits, lWidth * lHeight);//保存结构元素h对原图像的腐蚀结果

	memcpy(lpDIBBits, lpSave_of_SrcDIBBits, lWidth * lHeight);//恢复原图像

	//对原图像求补
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			if(*(lpDIBBits+lWidth*j+i)==0)
			{
				*(lpDIBBits+lWidth*j+i)=255;
			}
			else
			{
				*(lpDIBBits+lWidth*j+i)=0;
			}
		}
	}

	Erosion(mStructure);//用结构元素m腐蚀原图像补集

	memcpy(lpRst_of_mStrDIBBits, lpDIBBits, lWidth * lHeight);//保存结构元素m对原图像补集的腐蚀结果

	//求两个腐蚀结果的交集即为击中与击不中变换结果
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			if((*(lpRst_of_hStrDIBBits+lWidth*j+i)==255)&&(*(lpRst_of_mStrDIBBits+lWidth*j+i)==255))
			{
				*(lpDIBBits+lWidth*j+i)=255;
			}
			else
			{
				*(lpDIBBits+lWidth*j+i)=0;
			}
		}
	}

	//释放内存
	delete	lpSave_of_SrcDIBBits;
	delete	lpRst_of_hStrDIBBits;
	delete	lpRst_of_mStrDIBBits;
	
}


//=======================================================
// 函数功能： 骨架提取算法
// 输入参数： int structure[3][3]-用户定义的3×3结构元素
// 返回值：   无
//=======================================================
void CBinaryMorphDib::SkeletonExtraction(int Structure[3][3])
{
	long i,j;			//循环变量
	unsigned char mark=1;//标志变量
	
	LPBYTE	lpRst_of_nEroDIBBits;	// 指向n次腐蚀结果缓存的指针
	LPBYTE	lpRst_of_nSXDIBBits;   // 指向n次运算后集合差缓存的指针
	LPBYTE	lpRst_of_nSkeDIBBits;   // 指向n次运算后得到的骨架结果的缓存指针

		
	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度

	//分配内存以保存图像
	lpRst_of_nEroDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_nSXDIBBits=new BYTE[lWidth * lHeight];
	lpRst_of_nSkeDIBBits=new BYTE[lWidth * lHeight];

	memset(lpRst_of_nEroDIBBits, (BYTE)0, lWidth * lHeight);
	memset(lpRst_of_nSXDIBBits, (BYTE)0, lWidth * lHeight);
	memset(lpRst_of_nSkeDIBBits, (BYTE)0, lWidth * lHeight);

	while(mark)
	{
		Erosion(Structure);//用结构元素腐蚀原图像
		memcpy(lpRst_of_nEroDIBBits, lpDIBBits, lWidth * lHeight);//保存第n次腐蚀结果
		
		mark=0;
		//设置标志mark，以判断是否结束while循环
		for(j = 0; j <lHeight; j++)
	    {
		    for(i = 0;i <lWidth; i++)
		    {
			    if((*(lpDIBBits+lWidth*j+i)==255))
			    {
					mark=1;
					i=lWidth;
					j=lHeight;					
			    }
			   
		    }
	    }
		
		Opening(Structure); //开运算

		//求n次腐蚀运算与开运算的差
		for(j = 0; j <lHeight; j++)
	    {
		    for(i = 0;i <lWidth; i++)
		    {
			    if((*(lpRst_of_nEroDIBBits+lWidth*j+i)==255)&&(*(lpDIBBits+lWidth*j+i)==0))
			    {
					*(lpRst_of_nSXDIBBits+lWidth*j+i)=255;					
			    }
							   
		    }
	    }

		//求第n次运算后的骨架
		for(j = 0; j <lHeight; j++)
	    {
		    for(i = 0;i <lWidth; i++)
		    {
			    if((*(lpRst_of_nSkeDIBBits+lWidth*j+i)==255)||(*(lpRst_of_nSXDIBBits+lWidth*j+i)==255))
			    {
					*(lpRst_of_nSkeDIBBits+lWidth*j+i)=255;					
			    }
				
		    }
	    }

		memcpy(lpDIBBits, lpRst_of_nEroDIBBits, lWidth * lHeight);//把n次腐蚀结果赋给原图像

	}

	memcpy(lpDIBBits, lpRst_of_nSkeDIBBits, lWidth * lHeight);//把最终求得的骨架结果复制到原图像

	//释放内存
	delete	lpRst_of_nEroDIBBits;
	delete	lpRst_of_nSXDIBBits;
	delete	lpRst_of_nSkeDIBBits;

}