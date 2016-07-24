#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "TuXiangHeChengDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"

 
 
TuXiangHeChengDib::TuXiangHeChengDib()
{
	bool m_valid=true;
}
TuXiangHeChengDib::~TuXiangHeChengDib()
{
}



///***************************************************************/           
/*函数名称：Add(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行加运算。            
/***************************************************************/ 
void TuXiangHeChengDib::Add(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)
{
	LPBYTE lpSrc;  //原图像像素指针
    LPBYTE lpDst;   //目标像素指针
    LPBYTE lpSrcBK;  //背景像素指针	
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		int i,j;  //循环变量
		LPBYTE  temp=new BYTE[wide*height];    //新图像缓冲区
		//初始化新分配的内存为255
		memset(temp,255,wide*height);			
		//进行每个像素的加运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{
				//获得原像素指针
				lpSrc=p_data+wide*j+i;
				//获得目标像素指针
				lpDst=temp+wide*j+i;
				//获得背景像素指针
				lpSrcBK=p_dataBK+wide*j+i;
				//进行加运算
				if((*lpSrc+*lpSrcBK)>255)
					*lpDst=255;
				else
					*lpDst=(*lpSrc+*lpSrcBK);
			}
		}
		memcpy(p_data, temp,wide*height);   //复制处理后的图像
		delete  temp;
	}
	else //24位真彩色
	{
		LONG wide,height,DibWidth;    //原图长、宽
		p_data=this->GetData ();   //取得原图的数据区指针
		wide=this->GetWidth ();  //取得原图的数据区宽度
		height=this->GetHeight ();   //取得原图的数据区高度
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		//进行每个像素的加运算
		for(int j=1;j<height-1;j++)
		{
			for(int i=3;i<DibWidth-3;i++)
			{
				//获得原像素指针
				lpSrc=p_data+DibWidth*j+i;
				//获得目标像素指针
				lpDst=p_temp+DibWidth*j+i;
				//获得背景像素指针
				lpSrcBK=p_dataBK+DibWidth*j+i;
				//进行加运算
				if((*lpSrc+*lpSrcBK)>255)
					*lpDst=255;
				else
					*lpDst=(*lpSrc+*lpSrcBK);
			}
		}
		memcpy(p_data, p_temp,DibWidth*height);   //复制处理后的图像
		delete  []p_temp;
	}	
}



///***************************************************************/           
/*函数名称：Sub(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行减运算。            
/***************************************************************/ 
void TuXiangHeChengDib::Sub(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height)
{
	LPBYTE lpSrc;  //原图像像素指针
    LPBYTE lpDst;   //目标像素指针
    LPBYTE lpSrcBK;  //背景像素指针
    if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		int i,j;  //循环变量
		LPBYTE  temp=new BYTE[wide*height];    //新图像缓冲区
		//初始化新分配的内存为255
		memset(temp,255,wide*height);	
		//进行每个像素的减运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{
				//获得原像素指针
				lpSrc=p_data+wide*j+i;
				//获得目标像素指针
				lpDst=temp+wide*j+i;
				//获得背景像素指针
				lpSrcBK=p_dataBK+wide*j+i;
				//进行减运算
				if((*lpSrc-*lpSrcBK)<0)
					*lpDst=0;
				else
					*lpDst=(*lpSrc-*lpSrcBK);
			}
		}
		memcpy(p_data, temp,wide*height);   //复制处理后的图像
		delete  temp;
	}
	else//24位真彩色
	{
		int wide,height,DibWidth;    //原图长、宽
		p_data=this->GetData ();   //取得原图的数据区指针
		wide=this->GetWidth ();  //取得原图的数据区宽度
		height=this->GetHeight ();   //取得原图的数据区高度
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp=new BYTE[height*DibWidth];
		//进行每个像素的减运算
		for(int j=1;j<height-1;j++)
		{
			for(int i=1;i<DibWidth-1;i++)
			{
				//获得原像素指针
				lpSrc=p_data+DibWidth*j+i;
				//获得目标像素指针
				lpDst=p_temp+DibWidth*j+i;
				//获得背景像素指针
				lpSrcBK=p_dataBK+DibWidth*j+i;
				//进行减运算
				if((*lpSrc-*lpSrcBK)<0)
					*lpDst=0;
				else
					*lpDst=(*lpSrc-*lpSrcBK);
			}
		}
		memcpy(p_data, p_temp,DibWidth*height);   //复制处理后的图像
		delete  []p_temp;
	}
}



///***************************************************************/           
/*函数名称：Yuyunsuan(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行与运算。            
/***************************************************************/
void TuXiangHeChengDib::Yuyunsuan(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height)
{
    if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		int i,j;  //循环变量 	 
		LPBYTE  temp1=new BYTE[wide*height]; //新图像缓冲区
		LPBYTE  temp2=new BYTE[wide*height]; 
		memset( temp1,0,wide*height);
		memset( temp2,0,wide*height);
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_data[wide*j+i]>128)
					temp1[wide*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_dataBK[wide*j+i]>128)
					temp2[wide*j+i]=1; 
			}
		}
		//进行每个像素的与运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{   
				if( temp2[wide*j+i]==1&& temp1[wide*j+i]==1)
					temp1[wide*j+i]=255;
				else 
					temp1[wide*j+i]=0;
			}
		}
		memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else//24位真彩色
	{
		int i,j;  //循环变量
		LPBYTE  p_data;     //原图数据区指针
		int wide,height,DibWidth;    //原图长、宽
		p_data=this->GetData ();   //取得原图的数据区指针
		wide=this->GetWidth ();  //取得原图的数据区宽度
		height=this->GetHeight ();   //取得原图的数据区高度
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_data[DibWidth*j+i]>128)
					p_temp1[DibWidth*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_dataBK[DibWidth*j+i]>128)
					p_temp2[DibWidth*j+i]=1; 
			}
		}
		//进行每个像素的与运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<DibWidth-1;i++)
			{   
				if( p_temp2[DibWidth*j+i]==1&& p_temp1[DibWidth*j+i]==1)
					p_temp1[DibWidth*j+i]=255;
				else 
					p_temp1[DibWidth*j+i]=0;
			}
		}
		memcpy(p_data, p_temp1,DibWidth*height);   //复制处理后的图像
		delete  []p_temp1;
		delete  []p_temp2;
	}
}
///***************************************************************/           
/*函数名称：Huoyunsuan(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行或运算。            
/***************************************************************/ 
void TuXiangHeChengDib::Huoyunsuan(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height)
{
    if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		int i,j;  //循环变量		 
		LPBYTE  temp1=new BYTE[wide*height]; //新图像缓冲区
		LPBYTE  temp2=new BYTE[wide*height]; 
		memset(temp1,0,wide*height);
		memset(temp2,0,wide*height);
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_data[wide*j+i]>128)
					temp1[wide*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_dataBK[wide*j+i]>128)
					temp2[wide*j+i]=1; 
			}
		}
		//进行每个像素的与运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{   
				if( temp2[wide*j+i]==0&& temp1[wide*j+i]==0)
					temp1[wide*j+i]=0;
				else 
					temp1[wide*j+i]=255;
			}
		}
		memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else//24位真彩色
	{
		int i,j;  //循环变量
		LPBYTE  p_data;     //原图数据区指针
		int wide,height,DibWidth;    //原图长、宽
		p_data=this->GetData ();   //取得原图的数据区指针
		wide=this->GetWidth ();  //取得原图的数据区宽度
		height=this->GetHeight ();   //取得原图的数据区高度
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_data[DibWidth*j+i]>128)
					p_temp1[DibWidth*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_dataBK[DibWidth*j+i]>128)
					p_temp2[DibWidth*j+i]=1; 
			}
		}
		//进行每个像素的与运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<DibWidth-1;i++)
			{   
				if( p_temp2[DibWidth*j+i]==0&& p_temp1[DibWidth*j+i]==0)
					p_temp1[DibWidth*j+i]=0;
				else 
					p_temp1[DibWidth*j+i]=255;
			}
		}
        memcpy(p_data, p_temp1,DibWidth*height);   //复制处理后的图像
		delete  []p_temp1;
		delete  []p_temp2;
	}
}



///***************************************************************/           
/*函数名称：Feiyunsuan()                                        
/*函数类型：void                                   
/*功能：对图像进行非运算。            
/***************************************************************/ 
void TuXiangHeChengDib::Feiyunsuan()
{
	int i,j;  //循环变量
	BYTE* p_data;//图像指针
	int wide,height;  //原图的高和宽
	p_data=GetData();
    if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像	    
	   wide=GetWidth();
	else//24位真彩色
	   wide=GetDibWidthBytes();
	height=GetHeight();
	LPBYTE p_temp1=new BYTE[wide*height]; //新图像缓冲区         
	memset(p_temp1,0,wide*height); //初始化为0
	//对原图二值化
	for(j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{
				if(p_data[wide*j+i]>128)
				p_temp1[wide*j+i]=1; 
		}
	}
 
	//进行每个像素的与运算
    for(j=1;j<height-1;j++)
	{
		for(i=1;i<wide-1;i++)
		{   
				if(p_temp1[wide*j+i]==0 )
				 p_temp1[wide*j+i]=255;
				else 
				 p_temp1[wide*j+i]=0;
		}
	}
    memcpy(p_data,p_temp1,wide*height);   //复制处理后的图像
	delete p_temp1;
}



///***************************************************************/           
/*函数名称：Huofei(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行或非运算。            
/***************************************************************/ 
void TuXiangHeChengDib::Huofei(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height)
{
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		int i,j;  //循环变量		 
		LPBYTE  temp1=new BYTE[wide*height]; //新图像缓冲区
		LPBYTE  temp2=new BYTE[wide*height]; 
		memset( temp1,0,wide*height);
		memset( temp2,0,wide*height);
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_data[wide*j+i]>128)
					temp1[wide*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_dataBK[wide*j+i]>128)
					temp2[wide*j+i]=1; 
			}
		}
		//进行每个像素的与运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{   
				if( temp2[wide*j+i]==0&& temp1[wide*j+i]==0)
					temp1[wide*j+i]=255;
				else 
					temp1[wide*j+i]=0;
			}
		}
		memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else//24位真彩色
	{
		int i,j;  //循环变量
		LPBYTE  p_data;     //原图数据区指针
		int wide,height,DibWidth;    //原图长、宽
		p_data=this->GetData ();   //取得原图的数据区指针
		wide=this->GetWidth ();  //取得原图的数据区宽度
		height=this->GetHeight ();   //取得原图的数据区高度
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_data[DibWidth*j+i]>128)
					p_temp1[DibWidth*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_dataBK[DibWidth*j+i]>128)
					p_temp2[DibWidth*j+i]=1; 
			}
		}
		//进行每个像素的与运算
		for(j=1;j<height-1;j++)
		{
			for(i=3;i<DibWidth-3;i++)
			{   
				if( p_temp2[DibWidth*j+i]==0&& p_temp1[DibWidth*j+i]==0)
					p_temp1[DibWidth*j+i]=255;
				else 
					p_temp1[DibWidth*j+i]=0;
			}
		}
		memcpy(p_data, p_temp1,DibWidth*height);   //复制处理后的图像
		delete  []p_temp1;
		delete  []p_temp2;
	}
}



///***************************************************************/           
/*函数名称：Yufei(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行与非运算。            
/***************************************************************/
void TuXiangHeChengDib::Yufei(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height)
{
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		int i,j;  //循环变量		 
		PBYTE  temp1=new BYTE[wide*height]; //新图像缓冲区
		LPBYTE  temp2=new BYTE[wide*height]; 
		memset( temp1,0,wide*height);
		memset( temp2,0,wide*height);
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_data[wide*j+i]>128)
					temp1[wide*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_dataBK[wide*j+i]>128)
					temp2[wide*j+i]=1; 
			}
		}
		//进行每个像素的与非运算
		for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{   
				if( temp2[wide*j+i]==1&& temp1[wide*j+i]==1)
					temp1[wide*j+i]=0;
				else 
					temp1[wide*j+i]=255;
			}
		}
		memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else//24位真彩色
	{
		int i,j;  //循环变量
		LPBYTE  p_data;     //原图数据区指针
		int wide,height,DibWidth;    //原图长、宽
		p_data=this->GetData ();   //取得原图的数据区指针
		wide=this->GetWidth ();  //取得原图的数据区宽度
		height=this->GetHeight ();   //取得原图的数据区高度
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
		BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_data[DibWidth*j+i]>128)
					p_temp1[DibWidth*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_dataBK[DibWidth*j+i]>128)
					p_temp2[DibWidth*j+i]=1; 
			}
		}
		//进行每个像素的与非运算
		for(j=1;j<height-1;j++)
		{
			for(i=3;i<DibWidth-3;i++)
			{   
				if( p_temp2[wide*j+i]==1&& p_temp1[DibWidth*j+i]==1)
					p_temp1[DibWidth*j+i]=0;
				else 
					p_temp1[DibWidth*j+i]=255;
			}
		}
		memcpy(p_data, p_temp1,DibWidth*height);   //复制处理后的图像
		delete  []p_temp1;
		delete  []p_temp2;
	}
}


///***************************************************************/           
/*函数名称：Yihuo(LPBYTE p_data, LPBYTE p_dataBK,int wide,int height)                                        
/*函数类型：void 
/*参数说明：p_data       原图像首地址
/*          p_dataBK     背景图像首地址
/*          wide，height 原图像的高和宽                                   
/*功能：对图像进行异或运算。            
/***************************************************************/
void TuXiangHeChengDib::Yihuo(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height)
{
	if (m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
       int i,j;  //循环变量 
	   LPBYTE  temp1=new BYTE[wide*height]; //新图像缓冲区
       LPBYTE  temp2=new BYTE[wide*height]; 
	   memset( temp1,0,wide*height);
       memset( temp2,0,wide*height);
	   //对原图二值化
	   for(j=0;j<height;j++)
	   {
			for(i=0;i<wide;i++)
			{
				if(p_data[wide*j+i]>128)
				 temp1[wide*j+i]=1; 
			}
		}
		//对背景二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				if(p_dataBK[wide*j+i]>128)
				   temp2[wide*j+i]=1; 
			}
		}
		//进行每个像素的异或运算
        for(j=1;j<height-1;j++)
		{
			for(i=1;i<wide-1;i++)
			{   
				if( temp2[wide*j+i]+ temp1[wide*j+i]==1)
				  temp1[wide*j+i]=255;
				else 
					  temp1[wide*j+i]=0;
			}
		}
        memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else//24位真彩色
	{
        int i,j;  //循环变量
	    LPBYTE  p_data;     //原图数据区指针
	    int wide,height,DibWidth;    //原图长、宽
	    p_data=this->GetData ();   //取得原图的数据区指针
        wide=this->GetWidth ();  //取得原图的数据区宽度
        height=this->GetHeight ();   //取得原图的数据区高度
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	    BYTE *p_temp1=new BYTE[height*DibWidth];
	    BYTE *p_temp2=new BYTE[height*DibWidth];
		//对原图二值化
		for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_data[DibWidth*j+i]>128)
				 p_temp1[DibWidth*j+i]=1; 
			}
		}
		//对背景二值化
			for(j=0;j<height;j++)
		{
			for(i=0;i<DibWidth;i++)
			{
				if(p_dataBK[DibWidth*j+i]>128)
				   p_temp2[DibWidth*j+i]=1; 
			}
		}
		//进行每个像素的异或运算
        for(j=1;j<height-1;j++)
		{
			for(i=3;i<DibWidth-3;i++)
			{   
				if( p_temp2[DibWidth*j+i]+ p_temp1[DibWidth*j+i]==1)
					p_temp1[DibWidth*j+i]=255;
				else 
					p_temp1[DibWidth*j+i]=0;
			}
		}
        memcpy(p_data, p_temp1,DibWidth*height);   //复制处理后的图像
		delete  []p_temp1;
		delete  []p_temp2;
	}
}
