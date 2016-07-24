#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "FuShiYuPengZhangDib.h"
#include "MainFrm.h"


 
 
FuShiYuPengZhangDib::FuShiYuPengZhangDib()
{
}
FuShiYuPengZhangDib::~FuShiYuPengZhangDib()
{
}


 

///***************************************************************/           
/*函数名称：ChuiZhiFuShi()                                        
/*函数类型：void                                      
/*功能：对图像进行垂直腐蚀。            
/***************************************************************/ 
void FuShiYuPengZhangDib::ChuiZhiFuShi()
{
	LPBYTE p_data;	// 指向DIB象素指针
	LPBYTE	lpSrc;// 指向源图像的指针
	LPBYTE	lpDst;// 指向缓存图像的指针
	LPBYTE	temp;// 指向缓存DIB图像的指针
	int i;
	int j;
	int n;//循环变量
	p_data = GetData();// 找到DIB图像象素起始位置
	LONG wide = GetWidth();// DIB的宽度
	LONG height = GetHeight();// DIB的高度
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{ 	
		temp = new BYTE[wide*height];// 暂时分配内存，以保存新图像
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);// 初始化新分配的内存，设定初始值为255
		//使用垂直方向的结构元素进行腐蚀
		for(j = 1; j < height - 1; j++)
		{
			for(i = 0; i < wide; i ++)
			{
				//由于使用3×1的结构元素，为防止越界，所以不处理最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + wide * j + i);
				lpDst = (unsigned char *)(temp + wide * j + i);	// 指向目标图像倒数第j行，第i个象素的指针
				*lpDst = 0;//目标图像中的当前点先赋成黑色
				// 如果源图像中(0,-1)、(0,0)、(0,1)三个点之一有白点，
				// 则将目标图像中的(0,0)点赋成白色
				for (n = 0; n < 3; n++)
				{
					if (*(lpSrc + (n - 1) * wide) > 128)
					{
						*lpDst = 255;					 
						break;
					}
				}				
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, temp, wide * height);
		// 释放内存
		delete temp; 
	}
	else //24位真彩色
	{
		LONG DibWidth;    //原图长、宽	     
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		//使用垂直方向的结构元素进行腐蚀
		for(j = 1; j < height - 1; j++)
		{
			for(i = 0; i < DibWidth; i+=3)
			{
				//由于使用3×1的结构元素，为防止越界，所以不处理最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + DibWidth * j + i);
				lpDst = (unsigned char *)(p_temp + DibWidth * j + i);	// 指向目标图像倒数第j行，第i个象素的指针
				
				*lpDst = 0;//目标图像中的当前点先赋成黑色
				*(lpDst+1)=0;
				*(lpDst+2)=0;
				// 如果源图像中(0,-1)、(0,0)、(0,1)三个点之一有白点，
				// 则将目标图像中的(0,0)点赋成白色
				for(int m=0;m<3;m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (*(lpSrc + (n - 1) * DibWidth) > 128)
						{
							*lpDst = 255;
							break;
						}
						else
							*lpDst =*lpSrc ;
					}
					lpSrc++;
					lpDst++;
				}			
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, p_temp, DibWidth * height); 
		// 释放内存
		delete []p_temp;
	}	
}



///***************************************************************/           
/*函数名称：Shuipingfushi()                                        
/*函数类型：void                                      
/*功能：对图像进行水平腐蚀。            
/***************************************************************/ 
void FuShiYuPengZhangDib::Shuipingfushi()
{
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针	 
	LPBYTE	temp;
	//循环变量
	int i;
	int j;
	int n;
	// 找到DIB图像象素起始位置
	p_data = GetData();
	// DIB的宽度
	LONG wide = GetWidth();
	// DIB的高度
	LONG height = GetHeight();
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{    
		// 暂时分配内存，以保存新图像
		temp = new BYTE[wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		// 使用水平方向的结构元素进行腐蚀
		for (j = 0; j < height; j++)
		{
			for (i = 1; i < wide - 1; i ++)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				lpSrc = (unsigned char *)(p_data + wide *j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(temp + wide * j + i);
				// 目标图像中的当前点先赋成黑色
				*lpDst = 0;
				// 如果源图像中(-1,0)、(0,0)、(1,0)三个点之一有白点，
				// 则将目标图像中的(0,0)点赋成白色
				for (n = 0; n < 3; n++)
				{
					if (*(lpSrc + (n - 1) ) > 128)
					{
						*lpDst = 255;
						break;
					}
				}				
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, temp, wide * height); 
		// 释放内存
		delete temp; 
	}
	else //24位真彩色
	{
		LONG DibWidth;    //原图长、宽	     
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		// 使用水平方向的结构元素进行腐蚀
		for (j = 0; j < height; j++)
		{
			for (i = 3; i < DibWidth - 3; i +=3)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				lpSrc = (unsigned char *)(p_data + DibWidth *j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(p_temp + DibWidth * j + i);
				// 目标图像中的当前点先赋成黑色
				*lpDst = 0;
				*(lpDst+1)=0;
				*(lpDst+2)=0;
				// 如果源图像中(-1,0)、(0,0)、(1,0)三个点之一有白点，
				// 则将目标图像中的(0,0)点赋成白色
				for(int m=0;m<3;m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (*(lpSrc + (n - 1)*3 ) > 128)
						{
							*lpDst = 255;
							break;
						}
						else 
							*lpDst =*lpSrc;
					}
					lpSrc++;
					lpDst++;
				}
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, p_temp, DibWidth * height); 
		// 释放内存
		delete []p_temp; 
	}	
}



///***************************************************************/           
/*函数名称：Quanfangxiangfushi()                                        
/*函数类型：void                                      
/*功能：对图像进行全方向腐蚀。            
/***************************************************************/
void FuShiYuPengZhangDib::Quanfangxiangfushi()
{
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
    // 指向缓存DIB图像的指针
	LPBYTE	temp;
	//循环变量
	int i;
	int j;
	int  m;
	int n;
	// 找到DIB图像象素起始位置
	p_data = GetData();
	// DIB的宽度
	LONG wide= GetWidth();
	// DIB的高度
	LONG height =GetHeight();
    if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{  	
		// 暂时分配内存，以保存新图像
		temp =new BYTE[wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		// 3×3的结构元素
		int B[9] = {1, 0, 1,
			        0, 0, 0,
			        1, 0, 1};
		// 使用全方向的结构元素进行腐蚀
		for (j = 1; j < height - 1; j++)
		{
			for (i = 1; i < wide - 1; i++)
			{
				// 由于使用3×3的结构元素，为防止越界，所以不处理最左、右、上、下四边的像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + wide * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(temp + wide * j + i);
				// 目标图像中的当前点先赋成黑色
				*lpDst = 0; 
				// 如果源图像中3×3结构元素对应位置有白点	
				// 则将目标图像中的(0,0)点赋成白色
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (B[m + n] == 1)
							continue;
						
						if (*(lpSrc + (1 - m) * wide +(n - 1) ) > 128)
						{
							*lpDst = 255;
							break;
						}
					}
				}				
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, temp, wide * height);
		// 释放内存
		delete temp;
	}
	else //24位真彩色
	{
		LONG DibWidth;    //原图长、宽	     
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		// 3×3的结构元素
		int B[9] = {1, 0, 1,
		        	0, 0, 0,
		        	1, 0, 1};
		// 使用全方向的结构元素进行腐蚀
		for (j = 1; j < height - 1; j++)
		{
			for (i = 3; i < DibWidth - 3; i+=3)
			{
				// 由于使用3×3的结构元素，为防止越界，所以不处理最左、右、上、下四边的像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + DibWidth * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(p_temp + DibWidth * j + i);
				// 目标图像中的当前点先赋成黑色
				*lpDst = 0;
				*(lpDst+1)=0;
				*(lpDst+2)=0;
				// 如果源图像中3×3结构元素对应位置有白点	
				// 则将目标图像中的(0,0)点赋成白色
				for(int l=0;l<3;l++)
				{
					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							if (B[m + n] == 1)
								continue;
							if (*(lpSrc + (1 - m) * DibWidth +(n - 1)*3 ) > 128)
							{
								*lpDst = 255;
								break;
							}
							else 
								*lpDst = *lpSrc;
						}
					}
					*lpSrc++;
					*lpDst++;
				}
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, p_temp, DibWidth * height);
		// 释放内存
		delete []p_temp;
	}   
}



///***************************************************************/           
/*函数名称：Shuipingpengzhang()                                        
/*函数类型：void                                      
/*功能：对图像进行水平膨胀。            
/***************************************************************/
void FuShiYuPengZhangDib::Shuipingpengzhang()
{
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
 	LPBYTE	temp;
	//循环变量
	int i;
	int j;
	int n;
 	// 找到DIB图像象素起始位置
	p_data= GetData();
	// DIB的宽度
	LONG wide = GetWidth();
	// DIB的高度
	LONG height = GetHeight();
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{    
		// 暂时分配内存，以保存新图像
		temp = new BYTE[wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		// 使用水平方向的结构元素进行膨胀
		for (j = 0; j < height; j++)
		{
			for (i = 1; i < wide - 1; i++)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + wide * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(temp + wide * j + i);
				// 目标图像中的当前点先赋成白色
				*lpDst = 255;		 
				// 如果源图像中(-1,0)、(0,0)、(1,0)三个点之一有黑点，
				// 则将目标图像中的(0,0)点赋成黑色
				for (n = 0; n < 3; n++)
				{
					if (*(lpSrc + (n - 1) ) < 128)
					{
						*lpDst = 0;					 
						break;
					}
				}				
			}
		}
		// 复制膨胀后的图像
		memcpy(p_data, temp, wide * height); 
		// 释放内存
		delete temp;
	}
	else //24位真彩色
	{
		LONG DibWidth;    //原图长、宽	     
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		// 使用水平方向的结构元素进行膨胀
		for (j = 0; j < height-1; j++)
		{
			for (i = 3; i < DibWidth ; i+=3)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + DibWidth * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(p_temp + DibWidth * j + i);
				// 目标图像中的当前点先赋成白色
				*lpDst = *lpSrc;			 
				*(lpDst+1) = *(lpSrc+1);
				*(lpDst+2) = *(lpSrc+2);
				// 如果源图像中(-1,0)、(0,0)、(1,0)三个点之一有黑点，
				// 则将目标图像中的(0,0)点赋成黑色
				for(int m=0;m<3;m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (*(lpSrc + (n - 1)*3 ) < 127)
						{
							*lpDst = 0;
							break;
						}				
					}
					*lpSrc++;
					*lpDst++;
				}
			}
		}
		// 复制膨胀后的图像
		memcpy(p_data, p_temp, DibWidth * height); 
		// 释放内存
		delete []p_temp;
	}  
}


///***************************************************************/           
/*函数名称：Chuizhipengzhang()                                        
/*函数类型：void                                      
/*功能：对图像进行垂直膨胀。            
/***************************************************************/
void FuShiYuPengZhangDib::Chuizhipengzhang()
{
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
 	LPBYTE	temp;
	//循环变量
	int i;
	int j;
	int n;
	// 找到DIB图像象素起始位置
	p_data = GetData();
	// DIB的宽度
	LONG wide = GetWidth();
	// DIB的高度
	LONG height = GetHeight();
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{    
	    // 暂时分配内存，以保存新图像
		temp =new BYTE[wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		//使用垂直方向的结构元素进行膨胀
		for(j = 1; j < height - 1; j++)
		{
			for(i = 0; i < wide; i ++)
			{
				//由于使用3×1的结构元素，为防止越界，所以不处理最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data+ wide * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(temp + wide * j + i);
				//目标图像中的当前点先赋成白色		
				*lpDst = 255;			 
				// 如果源图像中(0,-1)、(0,0)、(0,1)三个点之一有黑点，
				// 则将目标图像中的(0,0)点赋成黑色
				for (n = 0; n < 3; n++)
				{
					if (*(lpSrc + (n - 1) * wide) < 128)
					{
						*lpDst = 0;
						break;
					}
				}				
			}
		}
		// 复制膨胀后的图像
		memcpy(p_data, temp, wide *height);
		// 释放内存
		delete temp;
	}
	else //24位真彩色
	{
		LONG DibWidth;    //原图长、宽	    
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		//使用垂直方向的结构元素进行膨胀
		for(j = 1; j < height - 1; j++)
		{
			for(i = 0; i < DibWidth; i +=3)
			{
				//由于使用3×1的结构元素，为防止越界，所以不处理最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data+ DibWidth * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(p_temp + DibWidth * j + i);
				//目标图像中的当前点先赋成白色
				*lpDst = *lpSrc;			 
				*(lpDst+1) = *(lpSrc+1);
				*(lpDst+2) = *(lpSrc+2);
				// 如果源图像中(0,-1)、(0,0)、(0,1)三个点之一有黑点，
				// 则将目标图像中的(0,0)点赋成黑色
				for (int m=0;m<3;m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (*(lpSrc + (n - 1) * DibWidth) < 127)
						{
							*lpDst = 0;                  
							break;
						}				
					}
					*lpSrc++;
					*lpDst++;
				}
			}
		}
		// 复制膨胀后的图像
		memcpy(p_data, p_temp, DibWidth *height);
		// 释放内存
		delete []p_temp;
	}	
}


///***************************************************************/           
/*函数名称：Quanfangxiangpengzhang()                                        
/*函数类型：void                                      
/*功能：对图像进行全方向膨胀。            
/***************************************************************/
void FuShiYuPengZhangDib::Quanfangxiangpengzhang()
{
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	//循环变量
	int i;
	int j;
	int m;
	int n;
	// 找到DIB图像象素起始位置
	p_data = GetData();
 	// DIB的宽度
	LONG wide =GetWidth();
	// DIB的高度
	LONG height = GetHeight();
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{    
		// 暂时分配内存，以保存新图像
		temp = new BYTE[wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		// 3×3的结构元素
		int B[9] = {1, 0, 1,
			        0, 0, 0,
			        1, 0, 1};
		// 使用全方向的结构元素进行腐蚀
		for (j = 1; j <  height - 1; j++)
		{
			for (i = 1; i < wide -1; i ++)
			{
				// 由于使用3×3的结构元素，为防止越界，所以不处理最左、右、上、下四边的像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + wide * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(temp + wide * j + i);
				// 目标图像中的当前点先赋成白色
				*lpDst = 255;
				// 如果源图像中3×3结构元素对应位置有黑点	
				// 则将目标图像中的(0,0)点赋成黑色
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (B[m + n] == 1)
							continue;
						if (*(lpSrc + (1 - m) * wide +(n - 1) ) < 128)
						{
							*lpDst = 0;						 
							break;
						}
					}
				}				
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, temp, wide * height);
		// 释放内存
		delete temp;
	}
	else //24位真彩色
	{
		LONG DibWidth;    //原图长、宽	     
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		// 3×3的结构元素
		int B[9] = {1, 0, 1,
					0, 0, 0,
					1, 0, 1};
		// 使用全方向的结构元素进行腐蚀
		for (j = 1; j <  height - 1; j++)
		{
			for (i = 3; i < DibWidth -3; i +=3)
			{
				// 由于使用3×3的结构元素，为防止越界，所以不处理最左、右、上、下四边的像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (unsigned char *)(p_data + DibWidth * j + i);
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (unsigned char *)(p_temp + DibWidth * j + i);
				// 目标图像中的当前点先赋成白色
				*lpDst = *lpSrc;			 
				*(lpDst+1) = *(lpSrc+1);
				*(lpDst+2) = *(lpSrc+2);
				// 如果源图像中3×3结构元素对应位置有黑点	
				// 则将目标图像中的(0,0)点赋成黑色
				for(int l=0;l<3;l++)
				{
					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							if (B[m + n] == 1)
								continue;
							if (*(lpSrc + (1 - m) * DibWidth +(n - 1)*3 ) < 128)
							{
								*lpDst = 0;	
								break;
							}
						}
					}
					*lpSrc++;
					*lpDst++;
				}
			}
		}
		// 复制腐蚀后的图像
		memcpy(p_data, p_temp, DibWidth * height);
		// 释放内存
		delete []p_temp;
	}  
}
