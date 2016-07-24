#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "TiDuRuiHuaDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
#include "CDIB.h"
 
TiDuRuiHuaDib::TiDuRuiHuaDib()
{

}
TiDuRuiHuaDib::~TiDuRuiHuaDib()
{

}

///***************************************************************/           
/*函数名称：Menxianruihua()                                        
/*函数类型：void                                   
/*功能：对图像进行门限梯度锐化。            
/*****************************************************************/ 
void TiDuRuiHuaDib::Menxianruihua()
{
    LPBYTE p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{		
        LPBYTE p_temp=new BYTE [wide*height];    //开辟图像一缓冲区
		memset(p_temp,255,wide*height);  //初始化为255
		int temp;
		for(int j=1;j<height-1;j++)
		for(int i=1;i<wide-1;i++)
		{
			//根据双向一次微分公式计算当前像素的灰度值
			temp=(int)sqrt((p_data[wide*j+i]-p_data[wide*j+(i-1)])*
				(p_data[wide*j+i]-p_data[wide*j+(i-1)])+(p_data[wide*j+i]-p_data[wide*(j-1)+i])*
				(p_data[wide*j+i]-p_data[wide*(j-1)+i]));
			if (temp>=30)
			{   
				if((temp+100)>255)
					p_temp[wide*j+i]=255;
				else 
					p_temp[wide*j+i]=temp+100;
			}
			if (temp<30)
				p_temp[wide*j+i]=p_data[wide*j+i];
		}
		//将缓冲区中的图像复制回原图数据区
		memcpy(p_data,p_temp,wide*height);
		//删除缓冲区
		delete p_temp;
	}
	else	//24位彩色
	{	
		int DibWidth;    //原图长、宽、字节宽	
		DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp=new BYTE[height*DibWidth];
		for(int j=1;j<height-1;j++)	// 每行
		{
			for(int i=3;i<DibWidth-3;i++)	// 每列
			{
				int pby_pt1=0,pby_pt2=0,pby_pt=0;	
				pby_pt1=p_data[DibWidth*j+i]-p_data[DibWidth*j+(i-3)];
				pby_pt2=p_data[DibWidth*j+i]-p_data[DibWidth*(j-1)+i];
				pby_pt=(int)sqrt(pby_pt1*pby_pt1+pby_pt2*pby_pt2);
				if (pby_pt>=30)
				{	 
					if((pby_pt+100)>255)
						p_temp[DibWidth*j+i]=255;
					else 
						p_temp[DibWidth*j+i]=pby_pt+100;
				}
				if (pby_pt<30)
					p_temp[DibWidth*j+i]=p_data[DibWidth*j+i];
			}
		}
		memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
		delete []p_temp;  //删除暂时分配内存
	}
}

///***************************************************************/           
/*函数名称：Erzhirihua()                                      
/*函数类型：void                                   
/*功能： 根据梯度二值化图像。            
/***************************************************************/ 
void TiDuRuiHuaDib::Erzhirihua()
{
	LPBYTE  p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{        
        LPBYTE p_temp=new BYTE [wide*height];    //开辟图像一缓冲区
		memset(p_temp,255,wide*height);  //初始化为255
		int temp;
		for(int j=1;j<height-1;j++)
			for(int i=1;i<wide-1;i++)
			{			
				//根据双向一次微分公式计算当前像素的灰度值
				temp=(int)sqrt((p_data[wide*j+i]-p_data[wide*j+(i-1)])*(p_data[wide*j+i]-p_data[wide*j+(i-1)])+(p_data[wide*j+i]-p_data[wide*(j-1)+i])*(p_data[wide*j+i]-p_data[wide*(j-1)+i]));
                if (temp>30)
				{   
					p_temp[wide*j+i]=255;					 
				}
				else 
					p_temp[wide*j+i]=0;
			}
		//将缓冲区中的图像复制回原图数据区
		memcpy(p_data,p_temp,wide*height);
		//删除缓冲区
		delete p_temp;
	}	
	else	//24位彩色
	{	
	    int DibWidth;    //原图长、宽、字节宽	
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp=new BYTE[height*DibWidth];
		for(int j=1;j<height-1;j++)	// 每行
		{
			for(int i=3;i<DibWidth-3;i++)	// 每列
			{
				int pby_pt1=0,pby_pt2=0,pby_pt=0;	
				pby_pt1=*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-1)*DibWidth+i-3);
				pby_pt2=*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j)*DibWidth+i);
				pby_pt=(int)sqrt(pby_pt1*pby_pt1+pby_pt2*pby_pt2);
                if (pby_pt>30)
				{   
					*(p_temp+(height-j-1)*DibWidth+i)=255;					 
				}
				else 
					*(p_temp+(height-j-1)*DibWidth+i)=0;
			}
		}
        memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
        delete []p_temp;  //删除暂时分配内存
	}
}

///***************************************************************/           
/*函数名称：GuDingRuiHua()                                       
/*函数类型：void                                   
/*功能：给边缘规定一个特定的灰度级。            
/***************************************************************/ 
void TiDuRuiHuaDib::GuDingRuiHua()
{
	LPBYTE p_data;     //原图数据区指针
	int wide,height;    //原图长、宽 	
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{     
        LPBYTE p_temp=new BYTE [wide*height];    //开辟图像一缓冲区
		memset(p_temp,255,wide*height);  //初始化为255
		int temp;
		for(int j=1;j<height-1;j++)
		    for(int i=1;i<wide-1;i++)
			{
				//根据双向一次微分公式计算当前像素的灰度值
				temp=(int)sqrt((p_data[wide*j+i]-p_data[wide*j+(i-1)])*(p_data[wide*j+i]-p_data[wide*j+(i-1)])+(p_data[wide*j+i]-p_data[wide*(j-1)+i])*(p_data[wide*j+i]-p_data[wide*(j-1)+i]));
                if (temp>30)
				{   
                   p_temp[wide*j+i]=255;
				}
				else 
					p_temp[wide*j+i]=p_data[wide*j+i];
			}
		//将缓冲区中的图像复制回原图数据区
		memcpy(p_data,p_temp,wide*height);
		//删除缓冲区
		delete p_temp;
	}
	else	//24位彩色
	{	    
	    int DibWidth;    //原图长、宽、字节宽	
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp=new BYTE[height*DibWidth];
	    for(int j=1;j<height-1;j++)	// 每行
		{
			for(int i=3;i<DibWidth-3;i++)	// 每列
			{
				int pby_pt1=0,pby_pt2=0,pby_pt=0;	
				pby_pt1=*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-1)*DibWidth+i-3);
				pby_pt2=*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j)*DibWidth+i);
				pby_pt=(int)sqrt(pby_pt1*pby_pt1+pby_pt2*pby_pt2);
				if (pby_pt>30)
				{   
					*(p_temp+(height-j-1)*DibWidth+i)=255;
				}
				else 
					*(p_temp+(height-j-1)*DibWidth+i)=*(p_data+(height-j-1)*DibWidth+i);
			}
		}
        memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
        delete []p_temp;  //删除暂时分配内存
	}
}
