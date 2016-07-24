#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BingXingBianJieDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
BingXingBianJieDib::BingXingBianJieDib()
{
}
BingXingBianJieDib::~BingXingBianJieDib()
{
}

///***************************************************************/           
/*函数名称：Lunkuotiqu()                                      
/*函数类型：void                                     
/*功能：对图像进行轮廓提取。            
/***************************************************************/ 
void BingXingBianJieDib::Lunkuotiqu()
{
    LPBYTE  p_data ;     //原图数据区指针
	int wide,height;    //原图长、宽
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;	
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	//循环变量
	long i;
	long j;
	unsigned char n1,n2,n3,n4,n5,n6,n7,n8;
	//像素值
	unsigned char pixel;
	// 暂时分配内存，以保存新图像
	p_data=GetData();
	wide=GetWidth();
    height=GetHeight();
	temp = new BYTE[wide * height];
	for (j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{
			lpSrc = (LPBYTE)p_data + wide * j + i;
			if(*lpSrc>127)
				*lpSrc=255;
			else
				*lpSrc=0;
		}
	}
	// 初始化新分配的内存，设定初始值为255
	memset(temp,  255, wide * height);
	for(j = 1; j <height-1; j++)
	{
		for(i = 1;i <wide-1; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (LPBYTE)p_data + wide * j + i;
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (LPBYTE)temp + wide * j + i;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
			if(pixel == 0)
			{
				*lpDst = (unsigned char)0;
				n1 = (unsigned char)*(lpSrc + wide -1);
				n2  = (unsigned char)*(lpSrc + wide );
				n3 = (unsigned char)*(lpSrc + wide +1);
				n4 = (unsigned char)*(lpSrc -1);
				n5= (unsigned char)*(lpSrc +1);
				n6 = (unsigned char)*(lpSrc - wide -1);
				n7 = (unsigned char)*(lpSrc - wide );
				n8 = (unsigned char)*(lpSrc - wide +1);
				//如果相邻的八个点都是黑点
				if(n1+n2+n3+n4+n5+n6+n7+n8==0)
				{
					*lpDst = (unsigned char)255;
				}
			}
		}
	}
	// 复制腐蚀后的图像
	memcpy(p_data,temp, wide * height);
	// 释放内存
	delete temp;	 
}

///***************************************************************/           
/*函数名称：Diedaifazhi(int *tongji)                                       
/*函数类型：void
/*参数说明：tonji     ---直方图灰度值统计                                      
/*功能：对图像进行迭代阀值选取。            
/***************************************************************/ 
void BingXingBianJieDib::Diedaifazhi(int *tongji)
{
    // 循环变量
	LONG i;
	LONG j;
	// 指向DIB象素指针
	LPBYTE p_data;
	// 找到DIB图像象素起始位置
	p_data = GetData();
	// DIB的宽度
	LONG wide = this->GetDibWidthBytes();
	// DIB的高度
	LONG height = this->GetHeight();
	// 迭代阀值
	int T1, T2;
	T1 = 127;
	T2 = 0;
	// 临时变量
	int Temp0, Temp1, Temp2, Temp3;
	Temp0 = Temp1 = Temp2 = Temp3 = 0;
	while (true)
	{
		// 计算下一个迭代阀值
		for (i = 0; i < T1 + 1; i++)
		{
			Temp0 += tongji[i] * i;
			Temp1 += tongji[i];
		}
		for (i = T1 + 1; i < 256; i++)
		{
			Temp2 += tongji[i] * i;
			Temp3 += tongji[i];
		}
		T2 = (Temp0 / Temp1 + Temp2 / Temp3) / 2;
		// 看迭代结果是否已收敛
		if (T1 == T2)
			break;
		else
			T1 = T2;
	}
	// 对各像素进行灰度转换
	for (j = 0; j < height; j ++)
	{
		for (i = 0; i < wide; i ++)
		{
			// 读取像素
			unsigned char temp = *((unsigned char *)p_data + wide * j + i);
			// 判断像素灰度值是否超出范围
			if (temp < T1)
				temp = 0;
			else
				temp = 255;
			// 回写处理完的像素
			*((unsigned char *)p_data + wide * j + i) = temp;
		}
	}
}

///***************************************************************/           
/*函数名称：Fenbutongji(int *tongji)                                      
/*函数类型：void
/*参数说明：tonji     ---直方图灰度值统计                                      
/*功能：对图像进行灰度值统计。            
/***************************************************************/ 
void BingXingBianJieDib::Fenbutongji(int *tongji)
{
	// 循环变量
	LONG i;
	LONG j;
	//变量初始化
	memset(tongji,0,sizeof(int) * 256);
	// 指向DIB象素指针
	LPBYTE p_data;
	// 找到DIB图像象素起始位置
	p_data = this->GetData();
	// DIB的宽度
	LONG wide = GetDibWidthBytes();
	// DIB的高度
	LONG height = GetHeight();
	// 对各像素进行灰度转换
	for (j = 0; j < height; j ++)
	{
		for (i = 0; i <wide; i++)
		{
			// 对各像素进行灰度统计
			unsigned char temp = *((unsigned char *)p_data + wide * j + i);
			tongji[temp]++; 
		}
	}
}

///***************************************************************/           
/*函数名称：Lunkuogenzong()                                      
/*函数类型：void                                     
/*功能：对图像进行边界跟踪。            
/***************************************************************/ 
void BingXingBianJieDib::Lunkuogenzong()
{
   	// 指向源图像的指针
	LPBYTE	lpSrc;
	LPBYTE   p_data ;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	long wide;
	long height;
	//循环变量
	long i;
	long j;
	//像素值
    long pixel;
	//是否找到起始点及回到起始点
	bool bFindStartPoint;
	//是否扫描到一个边界点
	bool bFindPoint;
	//起始边界点与当前边界点
	Point StartPoint,CurrentPoint;
	//八个方向和起始扫描方向
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;
	p_data=GetData();
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		wide=GetWidth();
		height=GetHeight();
		for (j=0;j<height;j++)
		{
			for(i=0;i<wide;i++)
			{
				lpSrc = (LPBYTE)p_data + wide * j + i;
				if(*lpSrc>127)
					*lpSrc=255;
				else
					*lpSrc=0;
			}
		}	
		// 暂时分配内存，以保存新图像
		temp = new BYTE[wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = temp;
		memset(lpDst, (BYTE)255, wide * height);
		//先找到最左上方的边界点
		bFindStartPoint = false;
		for (j = 0;j < height && !bFindStartPoint;j++)
		{
			for(i = 0;i < wide && !bFindStartPoint;i++)
			{
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc =  (LPBYTE)(p_data + wide * j + i);
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if(pixel ==0)
				{
					bFindStartPoint = true;
					StartPoint.Height = j;
					StartPoint.Width = i;
					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (LPBYTE)(temp + wide * j + i);	
					*lpDst = 0;
				}		
			}
		}
		//由于起始点是在左下方，故起始扫描沿左上方向
		BeginDirect = 0;
		//跟踪边界
		bFindStartPoint = false;
		//从初始点开始扫描
		CurrentPoint.Height = StartPoint.Height;
		CurrentPoint.Width = StartPoint.Width;
		while(!bFindStartPoint)
		{
			bFindPoint = false;
			while(!bFindPoint)
			{
				//沿扫描方向查看一个像素
				lpSrc = (LPBYTE)(p_data + wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
					+ (CurrentPoint.Width + Direction[BeginDirect][0]));
				pixel =  *lpSrc;
				if(pixel== 0)
				{
					bFindPoint = true;
					CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
					CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
					if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
					{
						bFindStartPoint = true;
					}
					lpDst =  (LPBYTE)(temp + wide * CurrentPoint.Height + CurrentPoint.Width);
					*lpDst = 0;
					//扫描的方向逆时针旋转两格
					BeginDirect--;
					if(BeginDirect == -1)
						BeginDirect = 7;
					BeginDirect--;
					if(BeginDirect == -1)
						BeginDirect = 7;
				}
				else
				{
					//扫描方向顺时针旋转一格
					BeginDirect++;
					if(BeginDirect == 8)
						BeginDirect = 0;
				}
			}
		}
		// 复制图像
		memcpy(p_data, temp, wide * height);
		// 释放内存
		delete temp; 
	}	
}

///***************************************************************/           
/*函数名称：Zhongzitianchong(CPoint SeedPoint)                                      
/*函数类型：void
/*参数说明：SeedPoint  ---选区种子点                                     
/*功能：对图像进行种子填充。            
/***************************************************************/ 
void BingXingBianJieDib::Zhongzitianchong(CPoint SeedPoint)
{
	// 指向源图像的指针
	LPBYTE	lpSrc;
    //图像的高和宽
	int height,wide ;
	//像素值
	long pixel;
	//种子堆栈及指针
	Seed *Seeds;
	int StackPoint;
    LPBYTE  temp;
	//当前像素位置
	int iCurrentPixelx,iCurrentPixely;
	temp =this->GetData();
	lpSrc=temp;
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		height=this->GetHeight();
		wide=this->GetWidth();		 
		for(int  j=0;j<height;j++)
		{
			for(int  i=0;i<wide;i++)
			{
				if(*lpSrc>110)
					*lpSrc=255;
				else
					*lpSrc=0; 
				lpSrc++;
			}
		}         
		//初始化种子
		Seeds = new Seed[wide*height];
		Seeds[1].Height = SeedPoint.y;
		Seeds[1].Width = SeedPoint.x;
		StackPoint = 1;
		while( StackPoint != 0)
		{
			//取出种子
			iCurrentPixelx = Seeds[StackPoint].Width;
			iCurrentPixely = Seeds[StackPoint].Height;
			StackPoint--;
			lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel =  *lpSrc;	 
			//将当前点涂黑
			*lpSrc =0;
			//判断左面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixelx > 0)
			{
				lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx - 1;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely;
					Seeds[StackPoint].Width = iCurrentPixelx - 1;
				}
			}
			//判断上面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixely < height - 1)
			{
				lpSrc = (LPBYTE)temp + wide * (iCurrentPixely + 1) + iCurrentPixelx;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely + 1;
					Seeds[StackPoint].Width = iCurrentPixelx;
				}
			}
			//判断右面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixelx < wide - 1)
			{
				lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx + 1;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely;
					Seeds[StackPoint].Width = iCurrentPixelx + 1;
				}
			}
			//判断下面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixely > 0)
			{
				lpSrc = (LPBYTE)temp + wide * (iCurrentPixely - 1) + iCurrentPixelx;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely - 1;
					Seeds[StackPoint].Width = iCurrentPixelx;
				}		 
			}
		}
		//释放堆栈
		delete Seeds;
	}
	else	//24位彩色
	{
		height=this->GetHeight();
		wide=this->GetDibWidthBytes();         
		//初始化种子
		Seeds = new Seed[wide*height];
		Seeds[1].Height = SeedPoint.y;
		Seeds[1].Width = SeedPoint.x*3;
		StackPoint = 1;
		while( StackPoint != 0)
		{
			//取出种子
			iCurrentPixelx = Seeds[StackPoint].Width;
			iCurrentPixely = Seeds[StackPoint].Height;
			StackPoint--;
			lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel =  *lpSrc;	 
			//将当前点涂黑
			*lpSrc =0;
			//判断左面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixelx > 0)
			{
				lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx - 1;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely;
					Seeds[StackPoint].Width = iCurrentPixelx - 1;
				}
			}
			//判断上面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixely < height - 1)
			{
				lpSrc = (LPBYTE)temp + wide * (iCurrentPixely + 1) + iCurrentPixelx;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely + 1;
					Seeds[StackPoint].Width = iCurrentPixelx;
				}		 
			}
			//判断右面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixelx < wide - 1)
			{
				lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx + 1;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely;
					Seeds[StackPoint].Width = iCurrentPixelx + 1;
				}
			}
			//判断下面的点，如果为白，则压入堆栈
			//注意防止越界
			if(iCurrentPixely > 0)
			{
				lpSrc = (LPBYTE)temp + wide * (iCurrentPixely - 1) + iCurrentPixelx;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel =  *lpSrc;
				if (pixel == 255)
				{
					StackPoint++;
					Seeds[StackPoint].Height = iCurrentPixely - 1;
					Seeds[StackPoint].Width = iCurrentPixelx;
				}		 
			}
		}
		//释放堆栈
		delete Seeds;
	}
}

///***************************************************************/           
/*函数名称：Yuzhifenge(int Yuzhi)                                      
/*函数类型：void
/*参数说明：Yuzhi  ---阈值选取                                     
/*功能：对图像进行阈值分割。            
/***************************************************************/
void BingXingBianJieDib::Yuzhifenge(int Yuzhi)
{
	// 指向源图像的指针
	LPBYTE  p_data;
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	//循环变量
	long i;
	long j;
	//图像的高和宽
	long wide;
	long height;
	p_data=GetData();
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
		wide=GetWidth();
	else	//24位彩色
		wide=GetDibWidthBytes();
	height=GetHeight();
	temp=new BYTE[wide*height];
	memset(temp,255,wide*height);
	for (j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{
			lpSrc=p_data+wide*j+i;
			lpDst=temp+wide*j+i;
			if(abs(*lpSrc-Yuzhi)<30)
				*lpDst=Yuzhi;			
		}
	}
    memcpy(p_data,temp,wide*height);
	delete temp;	
}

///***************************************************************/           
/*函数名称：Qiyuzengzhang(CPoint point)                                      
/*函数类型：void
/*参数说明：point  ---获得生长点                                     
/*功能：对图像进行区域生长。            
/***************************************************************/
void BingXingBianJieDib::Qiyuzengzhang(CPoint point)
{
    // 循环变量
	int i;
	int j;
	// 指向DIB象素指针
	LPBYTE p_data;
	// 找到DIB图像象素起始位置	
    p_data=GetData();
	// DIB的宽度
	LONG wide = GetWidth();	
	// DIB的高度
	LONG height =GetHeight();
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		// 计算种子点一的灰度值
		unsigned char  zhongzi=*(p_data+point.y*wide+point.x);	
		// 对各像素进行灰度转换
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				//获取各颜色分量
				unsigned char temp = *((unsigned char *)p_data + wide * j +i);
				if (abs(temp - zhongzi) < 10)    //当前点同种子一灰度值比较接近
				{								
					//将种子一的颜色赋给当前像素			 
					*((unsigned char *)p_data + wide * j + i ) = temp;
				}
				else 
					*((unsigned char *)p_data + wide * j + i ) =255;
			}
		}
	}
	else	//24位彩色
	{
		// 计算种子点一的灰度值
		int  zhongzi=*(p_data+(height-point.y)*wide*3+point.x*3);
		int   zhongzi2=*(p_data+(height-point.y)*wide*3+point.x*3+1);
		int   zhongzi3=*(p_data+(height-point.y)*wide*3+point.x*3+2);  	
		// 对各像素进行灰度转换
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				//获取各颜色分量
				int  temp = *((unsigned char *)p_data + 3*wide * j +i*3);
				int  temp2 = *((unsigned char *)p_data + 3*wide * j +i*3+1);
				int  temp3 = *((unsigned char *)p_data + 3*wide * j +i*3+2);
				if (abs(temp - zhongzi) < 10&&abs(temp2 - zhongzi2) < 10&&abs(temp3 - zhongzi3) < 10)    //当前点同种子一灰度值比较接近
				{								
					//将种子一的颜色赋给当前像素			 
					*(p_data + 3*wide * j + i*3 ) = temp;
					*(p_data +3*wide* j + i*3+1 ) = temp2;
					*(p_data +3*wide * j + i*3+2 ) = temp3;
				}
				else 
				{
					*(p_data + 3*wide * j + i*3 ) =255;
					*(p_data + 3*wide * j + i*3+1 ) =255;
					*(p_data + 3*wide * j + i*3+2 ) = 255;
				}
			}
		}
	} 
}
 
///***************************************************************/           
/*函数名称：Zhifangtu(float *fPs_Y)                              */
/*函数类型：void                                                 */
/*变量说明：tongji  灰度分布密度统计                             */                           
/*功能：对图像进行灰度直方图统计。								 */
/*****************************************************************/
void BingXingBianJieDib::Zhifangtu(float *tongji)
{
	// 循环变量
	int i;
	int j;
	// 灰度计数
	int huidu[256];
	int wide,height;    //原图长、宽
	// 变量初始化
	memset(huidu,0,sizeof(huidu));
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
        wide=this->GetWidth ();
        height=this->GetHeight ();
		int width= (((wide*24) + 31) / 32 * 4) ;
		LPBYTE temp1=new BYTE[wide*height+1024];    //新图像缓冲区
		//拷贝原图像到缓存图像
		memcpy( temp1,m_pData,wide*height );
		// 对各像素进行灰度统计
		for (i = 0; i < height; i ++)
		{
			for (j = 0; j <wide; j ++)
			{
				unsigned char  temp =  temp1[wide* i + j] ;			  			 
				// 灰度统计计数
				huidu[temp]++;		
			}
		}
		// 计算灰度分布密度
		for(i=0;i<256;i++)
			tongji[i] = huidu[i] / (height * wide *1.0f);
	}
	else	//24位彩色
	{
       	wide=this->GetDibWidthBytes();
        height=this->GetHeight ();
		LPBYTE  temp1=new BYTE[wide*height];    //新图像缓冲区
		//拷贝原图像到缓存图像
		memcpy(temp1,m_pData,wide*height );
		// 对各像素进行灰度统计
		for (i = 0; i < height; i ++)
		{
			for (j = 0; j <wide; j ++)
			{
				unsigned char  temp = temp1[wide* i + j] ;
				// 灰度统计计数
				huidu[temp]++;		
			}
		} 
		// 计算灰度分布密度
		for(i=0;i<256;i++)
			tongji[i] = huidu[i] / (height * wide *1.0f);
	} 
}

///***************************************************************/           
/*函数名称：BanYuZhi(int Yuzhi)                                      
/*函数类型：void
/*参数说明：Yuzhi  ---阈值选取                                     
/*功能：对图像进行半阈值分割。            
/***************************************************************/
void BingXingBianJieDib::BanYuZhi(int Yuzhi)
{
    // 指向源图像的指针
	LPBYTE  p_data;
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	//循环变量
	long i;
	long j;
	//图像的高和宽
	long wide;
	long height;
	p_data=GetData();
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		wide=GetWidth();
	}
	else	//24位彩色
		wide=GetDibWidthBytes();
	height=GetHeight();
	temp=new BYTE[wide*height];
	memset(temp,255,wide*height);
	for (j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{
			lpSrc=p_data+wide*j+i;
			
			lpDst=temp+wide*j+i;
			
			if((*lpSrc-Yuzhi)<30)
				*lpDst=*lpSrc;			
		}
	}
    memcpy(p_data,temp,wide*height);
	delete temp;
	
}

///***************************************************************/           
/*函数名称：Lunkuotiqu(CPoint SeedPoint)                                      
/*函数类型：void
/*参数说明：SeedPoint  ---种子点选取                                     
/*功能：对24位彩色图像进行轮廓提取。            
/***************************************************************/
void BingXingBianJieDib::Lunkuotiqu(CPoint SeedPoint)
{
    LPBYTE  p_data ;     //原图数据区指针
    int R,G,B,R1,G1,B1;
	int wide,height;    //原图长、宽
	// 指向源图像的指针
	LPBYTE	lpSrc;	
	// 指向缓存图像的指针
	LPBYTE	lpDst;	
	// 指向缓存DIB图像的指针
	LPBYTE	temp;	
	//循环变量
	int i;
	int j;
	p_data=GetData();
	wide=GetWidth();
    height=GetHeight();
	temp = new BYTE[wide * height*3];
	// 初始化新分配的内存，设定初始值为255
	memset(temp,  255, wide * height*3);
	lpSrc=p_data+wide*3*(height-SeedPoint.y)+3*SeedPoint.x;
	R1=*lpSrc;
    lpSrc++;
	G1=*lpSrc;
	lpSrc++;
	B1=*lpSrc;
	for(j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{
			lpSrc = (LPBYTE)p_data + wide * j*3 + i*3;
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (LPBYTE)temp + wide * j*3 + i*3;
			int I;
			I=0;
			for(int k=i-1;k<i+2;k++)
			{
				for(int l=j-1;l<j+2;l++)
				{
					if(k>=0&&l>=0&&k<wide&&l<height)
					{
						R=*(p_data+l*wide*3+k*3);
						G=*(p_data+l*wide*3+k*3+1);
						B=*(p_data+l*wide*3+k*3+2);
						if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
							I++;
					}
				}
			}
			if(I!=9)
			{	
				*lpDst=*lpSrc;
				*(lpDst+1)=*(lpSrc+1);
				*(lpDst+2)=*(lpSrc+2);
			} 
			else
			{
				*lpDst=255;
				*(lpDst+1)=255;
				*(lpDst+2)=255;
			}
		}
	}
	memcpy(p_data,temp, wide * height*3);
	delete temp;
}

/*************************************************************/
//此算法只适用于左边有边界的图像//
/*************************************************************/
void BingXingBianJieDib::Lunkuogenzong(CPoint SeedPoint)
{
	// 指向源图像的指针
	LPBYTE	lpSrc;
	LPBYTE   p_data ;
	int R1,G1,B1;
    int R,G,B;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	int wide;
	int height; 
	//循环变量
	int i;
	int j;
	//像素值
    int pixel;
	//是否找到起始点及回到起始点
	bool bFindStartPoint;
	//是否扫描到一个边界点
	bool bFindPoint;
	//起始边界点与当前边界点
	Point StartPoint,CurrentPoint;
	//八个方向和起始扫描方向
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;
	p_data=GetData();
	wide=this->GetWidth();
	height=this->GetHeight();
	// 暂时分配内存，以保存新图像
	temp = new BYTE[wide*height*3];
	// 初始化新分配的内存，设定初始值为255
	lpDst = temp;
	memset(temp, (BYTE)255, wide * height*3);
	//先找到最左方的边界点
    lpSrc=p_data+wide*3*(height-SeedPoint.y)+3*SeedPoint.x;//确定鼠标点的色度值
	R1=*lpSrc;
    lpSrc++;
	G1=*lpSrc;
	lpSrc++;
	B1=*lpSrc;
	bFindStartPoint = false;
    int s= height-SeedPoint.y;
	for(i=SeedPoint.x;i>1;i--)  //从鼠标点开始向左寻找边界起始点
	{
		lpSrc = (LPBYTE)p_data + wide * (height-SeedPoint.y)*3 + i*3;
		// 指向目标图像倒数第j行，第i个象素的指针			
		lpDst = (LPBYTE)temp + wide * (height-SeedPoint.y)*3 + i*3;
		int count;
		count=0;
		for(int k=i-1;k<i+2;k++)
		{
			for(int l=s-1;l<s+2;l++)
			{
				if(k>=0&&l>=0&&k<wide&&l<height)
				{
					R=*(p_data+l*wide*3+k*3);
					G=*(p_data+l*wide*3+k*3+1);
					B=*(p_data+l*wide*3+k*3+2);
					if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
						count++;
				}
			}
		}
		if(count!=9)
		{
			bFindStartPoint = true;
			StartPoint.Height = s;
			StartPoint.Width = i;
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (LPBYTE)(temp + wide * s*3 + i*3);	
			*lpDst = 0;
			*(lpDst+1)=0;
			*(lpDst+2)=0;
			break;
		} 
	}	        
	//由于起始点是在左下方，故起始扫描沿左上方向
	BeginDirect = 0;
	//跟踪边界
	bFindStartPoint = false;
	//从初始点开始扫描
	CurrentPoint.Height = StartPoint.Height;
	CurrentPoint.Width = StartPoint.Width;
	while(!bFindStartPoint)
	{
		bFindPoint = false;
		while(!bFindPoint)
		{
			//沿扫描方向查看一个像素
			lpSrc = (LPBYTE)(p_data + 3*wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
				+ 3*(CurrentPoint.Width + Direction[BeginDirect][0]));
			R =*lpSrc;
			G=*(lpSrc+1);
			B=*(lpSrc+2);
			if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
			{
				bFindPoint = true;
				CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
				CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
				if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
				{
					bFindStartPoint = true;
				}
				lpDst =  (LPBYTE)(temp + 3*wide * CurrentPoint.Height + 3*CurrentPoint.Width);
				*lpDst = *lpSrc;
                *(lpDst+1) = *(lpSrc+1);
				*(lpDst+2) = *(lpSrc+2);
				//扫描的方向逆时针旋转两格
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//扫描方向顺时针旋转一格
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;
			}
		}
	}
	// 复制图像
	memcpy(p_data, temp, 3*wide * height);
	// 释放内存
    delete temp; 
}
