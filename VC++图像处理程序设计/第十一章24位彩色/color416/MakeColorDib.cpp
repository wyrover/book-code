// MakeColorDib.cpp: implementation of the MakeColorDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSplit.h"
#include "MakeColorDib.h"
#include "MainFrm.h"
#include "math.h"
#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MakeColorDib::MakeColorDib()
{

}

MakeColorDib::~MakeColorDib()
{

}

/***************************************************************/
/*函数名称：MakegGray()                                        */
/*函数类型：void                                               */
/*功能：真彩色转化成256色灰度图像。                            */
/***************************************************************/
void MakeColorDib::MakegGray()    //灰度变化
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();   //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	for(int j=0;j<height;j++)	// 每行
		for(int i=0;i<DibWidth;i+=3)	// 每列
		{
			BYTE* pbyBlue = p_data++;   //得到蓝色值
			BYTE* pbyGreen = p_data++;  //得到绿色值
			BYTE* pbyRed = p_data++;    //得到红色值
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			//取到原r,g,b中的最大值作为像素三分量值的新值
			int gray=0;
			if(r>g)
				gray=r;
			else 
				gray=g;
			if(gray<b)
				gray=b;			
       		*pbyBlue = gray;     //将取到的最大值赋给像素的蓝分量
			*pbyGreen = gray;    //将取到的最大值赋给像素的绿分量
			*pbyRed = gray;	     //将取到的最大值赋给像素的红分量
		}
}

/***************************************************************/
/*函数名称：LightAlter(int m_Light)                            */
/*函数类型：void                                               */
/*参数：int m_Light，用户给定的阈值                            */
/*功能：对图像使用阈值法进行亮度调整。                         */
/***************************************************************/
void MakeColorDib::LightAlter(int m_Light)    //亮度调整
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();    //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	for(int j=0;j<height;j++)	// 每行
	for(int i=0;i<DibWidth;i++)	// 每列
	{   
		int a=0;
		a=int(*p_data*m_Light/100);   //调整当前点的亮度
		*p_data=a;
		//判断范围，取得合理的值
		if(a<0) 
			*p_data=0;  
	    if(a>255)
			*p_data=255;
        p_data++;   //指向下一指针
	}
}

/***************************************************************/
/*函数名称：LightReverse()                                     */
/*函数类型：void                                               */
/*功能：图像的亮度取反。                                       */
/***************************************************************/
void MakeColorDib::LightReverse()    //亮度取反
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	for(int j=0;j<height;j++)	// 每行
		for(int i=0;i<DibWidth;i++)	// 每列
		{
			int a=*p_data;   //取得当前点的值
			*p_data=255-a;    //取反
			p_data++;  //指向下一指针  
		}
}

/***************************************************************/
/*函数名称：ContrastAlter(int m_Increment)                     */
/*函数类型：void                                               */
/*参数：int m_Increment，用户给定的阈值                        */
/*功能：对图像使用阈值法调整对比度处理。                       */
/***************************************************************/
void MakeColorDib::ContrastAlter(int m_Increment)   ///对比度处理
{
	int nHigh = 255 - m_Increment;
	//对于极端情况加以处理
	if(nHigh < m_Increment)
	{
		nHigh = 127;
		m_Increment = 120;
	}
	if(m_Increment < -127)
		m_Increment = -120;
	//扩展或压缩区间的长度
	int nStretch = 255;
	if(m_Increment >= 0)
		nStretch = 255 - 2 * m_Increment;
	else
		nStretch = 255 + 2 * m_Increment;
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	if(m_Increment >= 0)   // m_Increment>=0时
	{
		for(int j=0;j<height;j++)	// 每行
			for(int i=0;i<DibWidth-3;i+=3)	// 每列
			{   
				//取得当前点（蓝色）的值，调整
				BYTE* pbyBlue = p_data++;    
				if(*pbyBlue<=m_Increment)
					*pbyBlue=0;
				else if(*pbyBlue>nHigh)
					*pbyBlue=255;
				else
					*pbyBlue=(BYTE)((((int)*pbyBlue - m_Increment) * 255) / nStretch );
				//取得当前点（绿色）的值，调整
				BYTE* pbyGreen = p_data++;
				if(*pbyGreen<=m_Increment)
					*pbyGreen=0;
				else if(*pbyGreen>nHigh)
					*pbyGreen=255;
				else
					*pbyGreen=(BYTE)((((int)*pbyGreen - m_Increment) * 255) / nStretch );
				//取得当前点（红色）的值，调整
				BYTE* pbyRed = p_data++;
				if(*pbyRed<=m_Increment)
					*pbyRed=0;
				else if(*pbyRed>nHigh)
					*pbyRed=255;
				else
					*pbyRed=(BYTE)((((int)*pbyRed - m_Increment) * 255) / nStretch );
			}
			
	}
	else  // m_Increment < 0 时
	{
		for(int j=0;j<height;j++)
			for(int i=0;i<DibWidth-3;i+=3)
			{	//取得当前点（蓝色）的值，调整
				BYTE* pbyBlue = p_data++;
				*pbyBlue = (BYTE)((((int)(*pbyBlue) * nStretch) / 255) - m_Increment);
				//取得当前点（红色）的值，调整
				BYTE* pbyGreen = p_data++;
				*pbyGreen = (BYTE)((((int)(*pbyGreen) * nStretch) / 255) - m_Increment);
				//取得当前点（红色）的值，调整
				BYTE* pbyRed = p_data++;
				*pbyRed = (BYTE)((((int)(*pbyRed) * nStretch) / 255) - m_Increment);				
			}
	}
}

/***************************************************************/
/*函数名称：Exposal()                                          */
/*函数类型：void                                               */
/*功能：图像曝光处理。                                         */
/***************************************************************/
void MakeColorDib::Exposal() //曝光处理
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	for(int j=0;j<height;j++)	// 每行
		for(int i=0;i<DibWidth;i++)	// 每列
		{
			BYTE* pbydata = p_data++;  //取得当前点的值
			BYTE a=*pbydata;   //传给临时变量
			*pbydata=(a>128)?a:(255-a);   //调整
		}
		
}

/***************************************************************/
/*函数名称：PaintColor(int m_Red,int m_Green,int m_Blue)       */
/*函数类型：void                                               */
/*参数：int m_Red、m_Green、m_Blue，用户给定的红绿蓝值         */
/*功能：对图像使用阈值法进行着色处理。                         */
/***************************************************************/
void MakeColorDib::PaintColor(int m_Red,int m_Green,int m_Blue) //着色处理
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	for(int j=0;j<height;j++)	// 每行
		for(int i=0;i<DibWidth;i+=3)	// 每列
		{			
			BYTE* pbyBlue = p_data++;	//取得当前点（蓝色）的值					
			BYTE* pbyGreen = p_data++;  //取得当前点（绿色）的值
			BYTE* pbyRed = p_data++;    //取得当前点（红色）的值
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
    		BYTE gray=(BYTE)(((WORD)r * 59 + (WORD)g * 30 + (WORD)b * 11) / 100);
			*pbyBlue = (BYTE)((m_Blue * gray) / 255);  
			*pbyGreen = (BYTE)((m_Green * gray) / 255);
			*pbyRed = (BYTE)((m_Red * gray) / 255);
		}
}

/***************************************************************/
/*函数名称：NeonLight()                                        */
/*函数类型：void                                               */
/*功能：使图像产生霓虹处理效果。                               */
/***************************************************************/
void MakeColorDib::NeonLight()   //霓虹处理
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
    BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-4;j++)	// 每行
	{
		for(int i=0;i<DibWidth-1;i++)	// 每列
		{
			 int pby_pt=0;
			 //对像素执行算法
			 pby_pt=(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-1)*DibWidth+i+3))
				   *(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-1)*DibWidth+i+3))
				   +(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-2)*DibWidth+i))
				   *(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-2)*DibWidth+i));
			*(p_temp+(height-j-1)*DibWidth+i)=2*int(sqrt(pby_pt));
			//判断合法性
			if(*(p_temp+(height-j-1)*DibWidth+i)<0)
				 *(p_temp+(height-j-1)*DibWidth+i)=0;
			if(*(p_temp+(height-j-1)*DibWidth+i)>255)
				 *(p_temp+(height-j-1)*DibWidth+i)=255;
		}
	}
    memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
    delete []p_temp;   //删除暂时分配内存
}

/***************************************************************/
/*函数名称：Smoothness()                                       */
/*函数类型：void                                               */
/*功能：使图像平滑处理。                                       */
/***************************************************************/
void MakeColorDib::Smoothness()   //平滑处理
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
    int h[3][3];////定义(3x3)矩阵
	h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1;
	h[1][0] = 1;  h[1][1] = 1; h[1][2] = 1;
	h[2][0] = 1;  h[2][1] = 1; h[2][2] = 1;
    BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-2;j++)	// 每行
	{
		for(int i=0;i<DibWidth-8;i++)	// 每列
		{
	        double pby_pt=0;
                    //对应的第0行的值乘以矩阵对应值，再相加
		     pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				    +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
				    +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
                    //对应的第1行的值乘以矩阵对应值，再相加
			    	+h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
			    	+h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
			    	+h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
                    //对应的第2行的值乘以矩阵对应值，再相加
    		        +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
					+h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
					+h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
			*(p_temp+(height-j-2)*DibWidth+i+3)=abs(int(pby_pt/9));//取总和的的平均值
		}
	}
   	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
   	delete []p_temp;//删除暂时分配内存
}

/***************************************************************/
/*函数名称：Embossment()                                       */
/*函数类型：void                                               */
/*功能：产生图像浮雕处理效果。                                 */
/***************************************************************/
void MakeColorDib::Embossment()   //浮雕处理
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
    BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height;j++)	// 每行
	{
		for(int i=0;i<DibWidth-4;i++)	// 每列
		{
			 int pby_pt=0;
			 //对像素得每个分量执行算法
			 pby_pt=*(p_data+(height-j-1)*DibWidth+i)
				   -*(p_data+(height-j-1)*DibWidth+i+3)+128;
			 *(p_temp+(height-j-1)*DibWidth+i+3)=pby_pt;
			//检验合法性
			if(*(p_temp+(height-j-1)*DibWidth+i+3)<0)
				*(p_temp+(height-j-1)*DibWidth+i+3)=0;
			else if(*(p_temp+(height-j-1)*DibWidth+i+3)>255)
				*(p_temp+(height-j-1)*DibWidth+i+3)=255;
		}
	}
    memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
    delete []p_temp;   //删除暂时分配内存
}

/***************************************************************/
/*函数名称：Spread()                                           */
/*函数类型：void                                               */
/*功能：图像扩散处理。                                         */
/***************************************************************/
void MakeColorDib::Spread()   //扩散处理
{ 
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
    BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-4;j++)	// 每行
	{
		for(int i=0;i<DibWidth-14;i++)	// 每列
		{
			 int m=0,n=0;
			 m=rand()%5; //取得行随机数
			 n=rand()%5; //取得列随机数
			 int pby_pt=0;	
			 pby_pt=*(p_data+(height-j-1-m)*DibWidth+i+3*n);//得到对应随机像素值
			 *(p_temp+(height-j-3)*DibWidth+i+6)=pby_pt;
		}
	}
    memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
    delete []p_temp;   //删除暂时分配内存
}

/***************************************************************/
/*函数名称：Sharp()                                            */
/*函数类型：void                                               */
/*功能：图像锐化处理。                                         */
/***************************************************************/
void MakeColorDib::Sharp()   //图像锐化
{
 	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
    BYTE *p_temp=new BYTE[height*DibWidth];
	for(int j=0;j<height-1;j++)	// 每行
	{
		for(int i=0;i<DibWidth-5;i++)	// 每列
		{
	        int pby_pt=0;	
			pby_pt= *(p_data+(height-j-2)*DibWidth+i+3)
			       -*(p_data+(height-j-1)*DibWidth+i);
			*(p_temp+(height-j-2)*DibWidth+i+3)=*(p_data+(height-j-2)*DibWidth+i+3)
				                                 +abs(int(pby_pt/4));
			if(*(p_temp+(height-j-2)*DibWidth+i+3)>255)
			   *(p_temp+(height-j-2)*DibWidth+i+3)=255;
		}
	}
    memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
    delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：HighLVBO(int m_GaoTong)                            */
/*函数类型：void                                               */
/*参数：int m_GaoTong，用户给定的阈值来选择矩阵                */
/*功能：对图像使用阈值法进行高通滤波。                         */
/***************************************************************/
void MakeColorDib::HighLVBO(int m_GaoTong)   //高通滤波
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	int h[3][3];  ////定义(3x3)矩阵
	if(m_GaoTong==1)
	{   //矩阵1（基本高通）
		h[0][0] =1;   h[0][1] =-2;  h[0][2] =1;
		h[1][0] =-2;  h[1][1] =5;   h[1][2] =-2;
		h[2][0] =1;   h[2][1] =-2;  h[2][2] =1;
	}
	else if(m_GaoTong==2)
	{   //矩阵2（中等高通）
		h[0][0] = 0;   h[0][1] = -1; h[0][2] = 0;
		h[1][0] = -1;  h[1][1] =  5; h[1][2] = -1;
		h[2][0] = 0;   h[2][1] = -1; h[2][2] = 0;
	}
	else
	{   //矩阵3（过量高通）
		h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
		h[1][0] = -1;  h[1][1] =  9; h[1][2] = -1;
		h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1;
	}
    BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-2;j++)	// 每行
	{
		for(int i=0;i<DibWidth-8;i++)	// 每列
		{
			int pby_pt=0;
			//对应的第0行的值乘以矩阵对应值，再相加
			pby_pt=  h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				+h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
				+h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
				//对应的第1行的值乘以矩阵对应值，再相加
				+h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
				+h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
				+h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
				//对应的第2行的值乘以矩阵对应值，再相加
				+h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
				+h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
				+h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
			*(p_temp+(height-j-2)*DibWidth+i+3)=abs(pby_pt);
			if(pby_pt>255) //判断是否越界
				*(p_temp+(height-j-2)*DibWidth+i+3)=255;
		}
	}
   	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
    delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：LowLVBO()                                          */
/*函数类型：void                                               */
/*功能：实现图像低通滤波(3x3)。                                */
/***************************************************************/
void MakeColorDib::LowLVBO()   //低通滤波(3x3)
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
    double h[3][3];////定义(3x3)矩阵
	h[0][0] = 0.1;  h[0][1] = 0.1; h[0][2] = 0.1;
	h[1][0] = 0.1;  h[1][1] = 0.2; h[1][2] = 0.1;
	h[2][0] = 0.1;  h[2][1] = 0.1; h[2][2] = 0.1;
	BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-2;j++)	// 每行
	{
		for(int i=0;i<DibWidth-8;i++)	// 每列
		{
			double pby_pt=0;
            //对应的第0行的值乘以矩阵对应值，再相加	
			pby_pt=  h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				+h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
				+h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
				//对应的第0行的值乘以矩阵对应值，再相加
				+h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
				+h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
				+h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
				//对应的第0行的值乘以矩阵对应值，再相加
				+h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
				+h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
				+h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
			*(p_temp+(height-j-2)*DibWidth+i+3)=abs(int(pby_pt));
		}
	}
	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
	delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：LowLVBObig()                                       */
/*函数类型：void                                               */
/*功能：实现图像低通滤波(5x5)。                                */
/***************************************************************/
void MakeColorDib::LowLVBObig()   //低通滤波(5x5)
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	int h[5][5];//定义(5x5)矩阵
	h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1; h[0][3] = 1; h[0][4] = 1;
	h[1][0] = 1;  h[1][1] = 2; h[1][2] = 2; h[1][3] = 2; h[1][4] = 1;
	h[2][0] = 1;  h[2][1] = 2; h[2][2] = 3; h[2][3] = 2; h[2][4] = 1;
	h[3][0] = 1;  h[3][1] = 2; h[3][2] = 2; h[3][3] = 2; h[3][4] = 1;
	h[4][0] = 1;  h[4][1] = 1; h[4][2] = 1; h[4][3] = 1; h[4][4] = 1;
	BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-4;j++)	// 每行
	{
		for(int i=0;i<DibWidth-14;i++)	// 每列
		{
	        int pby_pt=0;
	        //对应的第0行的值乘以矩阵对应值，再相加
			pby_pt=h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				  +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
				  +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
			      +h[0][3]*(*(p_data+(height-j-1)*DibWidth+i+9))
				  +h[0][4]*(*(p_data+(height-j-1)*DibWidth+i+12))
                  //对应的第1行的值乘以矩阵对应值，再相加
			      +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
			      +h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
				  +h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
				  +h[1][3]*(*(p_data+(height-j-2)*DibWidth+i+9))
				  +h[1][4]*(*(p_data+(height-j-2)*DibWidth+i+12))
                  //对应的第2行的值乘以矩阵对应值，再相加
			      +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
				  +h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
				  +h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6))
				  +h[2][3]*(*(p_data+(height-j-3)*DibWidth+i+9))
				  +h[2][4]*(*(p_data+(height-j-3)*DibWidth+i+12))
                  //对应的第3行的值乘以矩阵对应值，再相加
			      +h[3][0]*(*(p_data+(height-j-4)*DibWidth+i))
			      +h[3][1]*(*(p_data+(height-j-4)*DibWidth+i+3))
				  +h[3][2]*(*(p_data+(height-j-4)*DibWidth+i+6))
				  +h[3][3]*(*(p_data+(height-j-4)*DibWidth+i+9))
				  +h[3][4]*(*(p_data+(height-j-4)*DibWidth+i+12))
                  //对应的第4行的值乘以矩阵对应值，再相加
			      +h[4][0]*(*(p_data+(height-j-5)*DibWidth+i))
				  +h[4][1]*(*(p_data+(height-j-5)*DibWidth+i+3))
				  +h[4][2]*(*(p_data+(height-j-5)*DibWidth+i+6))
				  +h[4][3]*(*(p_data+(height-j-5)*DibWidth+i+9))
				  +h[4][4]*(*(p_data+(height-j-5)*DibWidth+i+12));
                  //为了计算方便我们把除以35（矩阵权和）放在求总和之后
			*(p_temp+(height-j-3)*DibWidth+i+6)=abs(int(pby_pt/35));
		}
	}
	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
	delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：ShuiPingGROW()                                     */
/*函数类型：void                                               */
/*功能：使图像水平增强。                                       */
/***************************************************************/
void MakeColorDib::ShuiPingGROW()   //水平增强
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	int h[3][1];//定义(3x1)矩阵
	h[0][0] = -1;  
	h[1][0] = 2; 
	h[2][0] = -1;
	BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-2;j++)	// 每行
	{
		for(int i=0;i<DibWidth-8;i++)	// 每列
		{
			int pby_pt=0;
			//对应的3行的值乘分别以矩阵对应值，再相加
			pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				+h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
				+h[2][0]*(*(p_data+(height-j-3)*DibWidth+i));
			if(pby_pt>20)
				*(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt)+100;
			else
				*(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt);
		}
	}
	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
    delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：ChuiZhiGROW()                                      */
/*函数类型：void                                               */
/*功能：使图像垂直增强。                                       */
/***************************************************************/
void MakeColorDib::ChuiZhiGROW()   //垂直增强
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	int h[1][3];//定义(1x3)矩阵
	h[0][0] = -1; 
	h[0][1] = 2;
	h[0][2] = -1;
	BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-2;j++)	// 每行
	{
		for(int i=0;i<DibWidth-8;i++)	// 每列
		{
			int pby_pt=0;
			//对应的第0行的值乘以矩阵对应值，再相加
			pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				+h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
				+h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6));
			if(pby_pt>20)
				*(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt)+100;
			else
				*(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt);
		}
	}
	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
	delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：ShuangXiangGROW()                                  */
/*函数类型：void                                               */
/*功能：使图像双向增强。                                       */
/***************************************************************/
void MakeColorDib::ShuangXiangGROW()    //双向增强
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	int h[3][3];//定义(3x3)矩阵
	h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
	h[1][0] = -1;  h[1][1] =  8; h[1][2] = -1;
	h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1; 
	BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-2;j++)	// 每行
	{	
		for(int i=0;i<DibWidth-8;i++)	// 每列
		{
			int pby_pt=0;
			//对应的第0行的值乘以矩阵对应值，再相加
			pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
				+h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
				+h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
				//对应的第1行的值乘以矩阵对应值，再相加
				+h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
				+h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
				+h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
				//对应的第2行的值乘以矩阵对应值，再相加
				+h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
				+h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
				+h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
			if(pby_pt>20)
				*(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt)+100;
			else
				*(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt);
		}
	}	
	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
	delete []p_temp;  //删除暂时分配内存
}

/***************************************************************/
/*函数名称：Mosaic()                                           */
/*函数类型：void                                               */
/*功能：使图像产生马赛克效果。                                 */
/***************************************************************/
void MakeColorDib::Mosaic()    //马赛克
{
	BYTE *p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽、字节宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	BYTE *p_temp=new BYTE[height*DibWidth];	// 暂时分配内存，以保存新图像
	for(int j=0;j<height-4;j+=5)	// 每行
	{	
	    for(int i=0;i<DibWidth-14;i+=15)	// 每列
		{   //对应周围(5x5)矩阵蓝色值求和平均
			int pby_pt=0;
			for(int m=0;m<5;m++)
				for(int n=0;n<15;n+=3)
				{   
					pby_pt+=*(p_data+(height-j-1-m)*DibWidth+i+n);
				}
				
			for(m=0;m<5;m++)
				for(int n=0;n<14;n+=3)
				{
					*(p_temp+(height-j-1-m)*DibWidth+i+n)=int(pby_pt/25);
				}	
            //对应周围(5x5)矩阵绿色值求和平均
			pby_pt=0;
			for(m=0;m<5;m++)
				for(int n=0;n<15;n+=3)
				{
					pby_pt+=*(p_data+(height-j-1-m)*DibWidth+i+n+1);
				}
			for(m=0;m<5;m++)
				for(int n=0;n<14;n+=3)
				{
					*(p_temp+(height-j-1-m)*DibWidth+i+n+1)=int(pby_pt/25);
				}
            //对应周围(5x5)矩阵红色值求和平均
			pby_pt=0;
			for(m=0;m<5;m++)
				for(int n=0;n<15;n+=3)
				{
					pby_pt+=*(p_data+(height-j-1-m)*DibWidth+i+n+2);
				}
			for(m=0;m<5;m++)
				for(int n=0;n<14;n+=3)
				{
					*(p_temp+(height-j-1-m)*DibWidth+i+n+2)=int(pby_pt/25);
				}
		}			
	}
	memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
	delete []p_temp;  //删除暂时分配内存
}
