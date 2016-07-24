#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "ZaoShengXiaoChuDib.h"
#include "MainFrm.h"
#include "DSplitView.h"
 
ZaoShengXiaoChuDib::ZaoShengXiaoChuDib()
{

}

ZaoShengXiaoChuDib::~ZaoShengXiaoChuDib()
{
}

/***************************************************************/           
/*函数名称：GuDing()                                           */
/*函数类型：void                                               */
/*参数：int Yuzhi，设定的阈值                                  */
/*功能：对图像使用固定阈值法进行二值化。                       */
/***************************************************************/
void ZaoShengXiaoChuDib::GuDing(int YuZhi)
{
	LPBYTE  p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();     //取得原图的数据区指针
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像		
	{
		wide=this->GetWidth ();
		height=this->GetHeight ();
		for(int j=0;j<height;j++)
		{
			for(int i=0;i<wide;i++)
			{
				if(*p_data>YuZhi)          //灰度值大于给定阈值，置为255
					*p_data=255;
				else
					*p_data=0;             //不大于置为0
				p_data++;
			}
		}
	}
	else	//24位彩色
	{
		wide=this->GetWidth();
		height=this->GetHeight ();
		for(int j=0;j<height;j++)
		{
			for(int i=0;i<wide;i++)    //所有像素依次循环
			{
				if(*p_data>YuZhi)          //若像素值不为0
					*p_data=255;   //将其置为255
				else
					*p_data=0;             //不大于置为0
				p_data++; 				 
			}
		}
	}
}

/***************************************************************/ 
/*函数名称：HeiBaiFanZhuan()                                   */
/*函数类型：void                                               */
/*功能：对二值图像的黑白点噪声消除。                           */
/***************************************************************/
void ZaoShengXiaoChuDib::HeiBaiFanZhuan()
{
	int averg,averg2,averg3;
	BYTE *p_data;		//原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();//取得原图的数据区指针
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		wide=this->GetWidth ();		//取得原图的数据区宽
		height=this->GetHeight ();  //取得原图的数据区高
		GuDing(100);				//进行二值化
		BYTE* p_temp=new BYTE[wide*height];// 申请并分配中间缓存
		memcpy(p_temp,m_pData,wide*height);// 复制图象数据到中间缓存
		//用3*3屏蔽窗口的8近邻均值进行滤波
		for(int j=1;j<height-1;j++)
		{	
			for(int i=1;i<wide-1;i++)
			{
				averg=0;			
				//求周围8近邻均值
				averg=(int)((p_data[(j-1)*wide+(i-1)]+p_data[(j-1)*wide+i]
					+p_data[(j-1)*wide+(i+1)]+p_data[j*wide+(i-1)]
					+p_data[j*wide+i+1]+p_data[(j+1)*wide+(i-1)]
					+p_data[(j+1)*wide+i]+p_data[(j+1)*wide+i+1])/8);
				if(abs(averg-p_temp[j*wide+i])>127.5)
					p_temp[j*wide+i]=averg;
			}
		}
		memcpy(p_data,p_temp,wide*height);
		delete p_temp;
	}
	else	//24位彩色
	{
		wide=this->GetWidth();  //取得原图的数据区宽
		height=this->GetHeight ();  //取得原图的数据区高
		GuDing(100);  //进行二值化
		BYTE* p_temp=new BYTE[wide*height*3];// 申请并分配中间缓存
		memcpy(p_temp,m_pData,wide*height*3);// 复制图象数据到中间缓存
		//用3*3屏蔽窗口的8近邻均值进行滤波
		for(int j=1;j<height-1;j++)
		{	
			for(int i=1;i<wide-1;i++)
			{
				averg=0;
				averg2=0;
				averg3=0;
				//求周围8近邻均值
				averg=(int)((p_data[(j-1)*wide*3+(i-1)*3]+p_data[(j-1)*wide*3+i*3]
					+p_data[(j-1)*wide*3+(i+1)*3]+p_data[j*wide*3+(i-1)*3]
					+p_data[j*wide*3+(i+1)*3]+p_data[(j+1)*wide*3+(i-1)*3]
					+p_data[(j+1)*wide*3+i*3]+p_data[(j+1)*wide*3+(i+1)*3])/8);
				averg2=(int)((p_data[(j-1)*wide*3+(i-1)*3+1]+p_data[(j-1)*wide*3+i*3+1]
					+p_data[(j-1)*wide*3+(i+1)*3+1]+p_data[j*wide*3+(i-1)*3+1]
					+p_data[j*wide*3+(i+1)*3+1]+p_data[(j+1)*wide*3+(i-1)*3+1]
					+p_data[(j+1)*wide*3+i*3+1]+p_data[(j+1)*wide*3+(i+1)*3+1])/8);
				averg3=(int)((p_data[(j-1)*wide*3+(i-1)*3+2]+p_data[(j-1)*wide*3+i*3+2]
					+p_data[(j-1)*wide*3+(i+1)*3+2]+p_data[j*wide*3+(i-1)*3+2]
					+p_data[j*wide*3+(i+1)*3+2]+p_data[(j+1)*wide*3+(i-1)*3+2]
					+p_data[(j+1)*wide*3+i*3+2]+p_data[(j+1)*wide*3+(i+1)*3+2])/8);
				if(abs(averg-p_temp[j*wide*3+i*3])>127.5)
					p_temp[j*wide*3+i*3]=averg;
				if(abs(averg2-p_temp[j*wide*3+i*3+1])>127.5)
					p_temp[j*wide*3+i*3+1]=averg2;
				if(abs(averg3-p_temp[j*wide*3+i*3+2])>127.5)
					p_temp[j*wide*3+i*3+2]=averg3;
			}
		}
		memcpy(p_data,p_temp,wide*height*3);
		delete p_temp;
	}
}

/***************************************************************/ 
/*函数名称：black(int connec)                                  */
/*函数类型：void                                               */
/*参数：int connec，设定的连通选择                             */
/*功能：对二值图像进行消除孤立黑像素点。                       */
/***************************************************************/
void ZaoShengXiaoChuDib::black(int connec)
{
	// 指向DIB象素指针
	BYTE *p_data;
    p_data=this->GetData();  	//取得原图的数据区指针 
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		int wide=this->GetWidth();  //取得原图的数据区宽
		int height=this->GetHeight();  //取得原图的数据区高
		//二值化
		GuDing(100);	
		// 申请并分配中间缓存
		BYTE* p_temp=new BYTE[wide*height]; 	
		// 复制图象数据到中间缓存
		memcpy(p_temp,p_data,wide*height);
		// 4连接的情况下，消去上下左右都为255(白点)的孤立黑点	
		if (connec==4)
		{
			for (int j=1;j<height-1;j++)
			{
				for (int i=1;i<wide-1;i++)
				{
					if (*(p_temp + wide * j + i)==255) 
						continue;
					if((*(p_temp + wide * (j-1) + i)+
						*(p_temp + wide * (j+1) + i)+
						*(p_temp + wide * j + i-1)+
						*(p_temp + wide * j + i+1))==255*4)
					{
						*(p_data + wide * j + i)=255;
					}
				}
			}
	delete p_temp;
		}
		// 8连接的情况下，消去周围都为255(白点)的孤立黑点	
		if (connec==8)
		{
			for (int j = 1; j < height-1; j ++)
			{
				for (int i = 1; i < wide-1; i ++)
				{
					if (*(p_temp + wide * j + i)==255)
						continue;
					if((*(p_temp + wide * (j-1) + i)+
						*(p_temp + wide * (j+1) + i)+
						*(p_temp + wide * j + i-1)+
						*(p_temp + wide * j + i-1)+
						*(p_temp + wide * (j-1) + i-1)+
						*(p_temp + wide * (j+1) + i+1)+
						*(p_temp + wide * (j-1) + i+1)+
						*(p_temp + wide * (j+1) + i-1))==255*8)
					{	
						*(p_data + wide * j + i)=255;
					}
				}
			}
delete p_temp;
		}
			
	}
/*	else	//24位彩色
	{
		int wide= this->GetWidth();  //取得原图的数据区宽
		int height=this->GetHeight();  //取得原图的数据区高
		//二值化
		GuDing(100);	
		// 申请并分配中间缓存
		BYTE* p_temp=new BYTE[wide*height*3]; 	
		// 复制图象数据到中间缓存
		memcpy(p_temp,p_data,wide*height*3);
		// 4连接的情况下，消去上下左右都为255(白点)的孤立黑点	
		if (connec==4)
		{
			for (int j=1;j<height-1;j++)
			{
				for (int i=1;i<wide-1;i++)
				{
					if ((*(p_temp + wide * j*3 + i*3)==255)&&
						(*(p_temp + wide * j*3 + i*3+1)==255) &&
						(*(p_temp + wide * j*3 + i*3+2)==255))
						continue;
					if(((*(p_temp + wide * (j-1)*3 + i*3)+
						*(p_temp + wide * (j+1)*3 + i*3)+
						*(p_temp + wide * j*3 + (i-1)*3)+
						*(p_temp + wide * j*3 + (i+1)*3))==255*4)&&
						((*(p_temp + wide * (j-1)*3 + i*3+1)+
						*(p_temp + wide * (j+1)*3 + i*3+1)+
						*(p_temp + wide * j*3 + (i-1)*3+1)+
						*(p_temp + wide * j*3 + (i+1)*3+1))==255*4)&&
						((*(p_temp + wide * (j-1)*3 + i*3+2)+
						*(p_temp + wide * (j+1)*3 + i*3+2)+
						*(p_temp + wide * j*3 + (i-1)*3+2)+
						*(p_temp + wide * j*3 + (i+1)*3+2))==255*4))
					{	
						*(p_data + wide * j*3 + i*3)=255;
						*(p_data + wide * j*3 + i*3+1)=255;
						*(p_data + wide * j*3 + i*3+2)=255;
					}
				}
			}
			delete p_temp;
		}
		// 8连接的情况下，消去周围都为255(白点)的孤立黑点	
		if (connec==8)
		{
			for (int j = 1; j < height-1; j ++)
			{
				for (int i = 1; i < wide-1; i ++)
				{
					if ((*(p_temp + wide * j*3 + i*3)==255)&&
						(*(p_temp + wide * j*3 + i*3+1)==255)&&
						(*(p_temp + wide * j*3 + i*3+2)==255))
						continue;
					if(((*(p_temp + wide * (j-1)*3 + i*3)+
						*(p_temp + wide * (j+1)*3 + i*3)+
						*(p_temp + wide * j*3 + (i-1)*3)+
						*(p_temp + wide * j*3 + (i-1)*3)+
						*(p_temp + wide * (j-1)*3 + (i-1)*3)+
						*(p_temp + wide * (j+1)*3 + (i+1)*3)+
						*(p_temp + wide * (j-1)*3 + (i+1)*3)+
						*(p_temp + wide * (j+1)*3 + (i-1)*3))==255*8)&&
						((*(p_temp + wide * (j-1)*3 + i*3+1)+
						*(p_temp + wide * (j+1)*3 + i*3+1)+
						*(p_temp + wide * j*3 + (i-1)*3+1)+
						*(p_temp + wide * j*3 + (i-1)*3+1)+
						*(p_temp + wide * (j-1)*3 + (i-1)*3+1)+
						*(p_temp + wide * (j+1)*3 + (i+1)*3+1)+
						*(p_temp + wide * (j-1)*3 + (i+1)*31+1)+
						*(p_temp + wide * (j+1)*3 + (i-1)*3+1))==255*8)&&
						((*(p_temp + wide * (j-1)*3 + i*3+2)+
						*(p_temp + wide * (j+1)*3 + i*3+2)+
						*(p_temp + wide * j*3 + (i-1)*3+2)+
						*(p_temp + wide * j*3 + (i-1)*3+2)+
						*(p_temp + wide * (j-1)*3 + (i-1)*3+2)+
						*(p_temp + wide * (j+1)*3 + (i+1)*3+2)+
						*(p_temp + wide * (j-1)*3 + (i+1)*3+2)+
						*(p_temp + wide * (j+1)*3 + (i-1)*3+2))==255*8))
					{	
						*(p_data + wide * j*3 + i*3)=255;
						*(p_data + wide * j*3 + i*3+1)=255;
						*(p_data + wide * j*3 + i*3+2)=255;
					}
				}
			}
		delete p_temp;
		}
		
	}*/
}

/***************************************************************/ 
/*函数名称：threethree()                                       */
/*函数类型：void                                               */
/*功能：对图像进行3*3均值滤波处理。                            */
/***************************************************************/
void ZaoShengXiaoChuDib::threethree() 
{
	float averg,averg2,averg3;
	BYTE *p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();//取得原图的数据区指针
	wide=this->GetWidth (); //取得原图的数据区宽
    height=this->GetHeight ();  //取得原图的数据区高
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{    
		BYTE* p_temp=new BYTE[wide*height];
		int size=wide*height;
		memset(p_temp,255,size);
		//用3*3屏蔽窗口的8近邻均值进行滤波
		for(int j=1;j<height-1;j++)
		{
			for(int i=1;i<wide-1;i++)
			{
				averg=0;
				//求周围8近邻均值
				averg=(int)(p_data[(j-1)*wide+(i-1)]+p_data[(j-1)*wide+i]
					+p_data[(j-1)*wide+(i+1)]+p_data[j*wide+(i-1)]
					+p_data[j*wide+i+1]+p_data[(j+1)*wide+(i-1)]
					+p_data[(j+1)*wide+i]+p_data[(j+1)*wide+i+1])/8;
				
				p_temp[j*wide+i]=averg;
			}
		}
		memcpy(p_data,p_temp,wide*height);
		delete p_temp;
	}
	else	//24位彩色
	{		
		BYTE* p_temp=new BYTE[wide*height*3];
		int size=wide*height*3;
		memset(p_temp,255,size);
		
		//用3*3屏蔽窗口的8近邻均值进行滤波
		for(int j=1;j<height-1;j++)
		{
			for(int i=1;i<wide-1;i++)
			{
				averg=0;
				averg2=0;
				averg3=0;
				//求周围8近邻均值
				averg=(int)(p_data[(j-1)*wide*3+(i-1)*3]+p_data[(j-1)*wide*3+i*3]
					+p_data[(j-1)*wide*3+(i+1)*3]+p_data[j*wide*3+(i-1)*3]
					+p_data[j*wide*3+(i+1)*3]+p_data[(j+1)*wide*3+(i-1)*3]
					+p_data[(j+1)*wide+i*3]+p_data[(j+1)*wide+(i+1)*3])/8;
				averg2=(int)(p_data[(j-1)*wide*3+(i-1)*3+1]+p_data[(j-1)*wide*3+i*3+1]
					+p_data[(j-1)*wide*3+(i+1)*3+1]+p_data[j*wide*3+(i-1)*3+1]
					+p_data[j*wide*3+(i+1)*3+1]+p_data[(j+1)*wide*3+(i-1)*3+1]
					+p_data[(j+1)*wide+i*3+1]+p_data[(j+1)*wide+(i+1)*3+1])/8;
				averg3=(int)(p_data[(j-1)*wide*3+(i-1)*3+2]+p_data[(j-1)*wide*3+i*3+2]
					+p_data[(j-1)*wide*3+(i+1)*3+2]+p_data[j*wide*3+(i-1)*3+2]
					+p_data[j*wide*3+(i+1)*3+2]+p_data[(j+1)*wide*3+(i-1)*3+2]
					+p_data[(j+1)*wide+i*3+2]+p_data[(j+1)*wide+(i+1)*3+2])/8;
				p_temp[j*wide*3+i*3]=(int)averg;
				p_temp[j*wide*3+i*3+1]=(int)averg2;
				p_temp[j*wide*3+i*3+2]=(int)averg3;
			}
		}
		memcpy(p_data,p_temp,wide*height*3);
		delete p_temp;
	}
}

/***************************************************************/
/*函数名称：Chaoxian(int T)                                    */
/*函数类型：void                                               */
/*参数：int T，设定的阈值                                      */
/*功能：超限邻域平均法。                                       */
/***************************************************************/
void ZaoShengXiaoChuDib::Chaoxian(int T)
{
	int averg,averg2,averg3;
	BYTE *p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();//取得原图的数据区指针
	wide=this->GetWidth (); //取得原图的数据区宽
    height=this->GetHeight ();  //取得原图的数据区高
	if(m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{    
		BYTE* p_temp=new BYTE[wide*height];
		int size=wide*height;
		memset(p_temp,255,size);
		//用3*3屏蔽窗口的8近邻均值进行滤波
		for(int j=1;j<height-1;j++)
		{
			for(int i=1;i<wide-1;i++)
			{
				averg=0;
				//求周围8近邻均值
				averg=(int)(p_data[(j-1)*wide+(i-1)]+p_data[(j-1)*wide+i]
					+p_data[(j-1)*wide+(i+1)]+p_data[j*wide+(i-1)]
					+p_data[j*wide+i+1]+p_data[(j+1)*wide+(i-1)]
					+p_data[(j+1)*wide+i]+p_data[(j+1)*wide+i+1])/8;
				if(abs(p_temp[j*wide+i]-averg)>T)
					p_temp[j*wide+i]=averg;
			}
		}
		memcpy(p_data,p_temp,wide*height);
		delete p_temp;
	}
	else	//24位彩色
	{	
		BYTE* p_temp=new BYTE[wide*height*3];
		int size=wide*height*3;
		memset(p_temp,255,size);
		//用3*3屏蔽窗口的8近邻均值进行滤波
		for(int j=1;j<height-1;j++)
		{
			for(int i=1;i<wide-1;i++)
			{
				averg=0;
				averg2=0;
				averg3=0;
				//求周围8近邻均值
				averg=(int)(p_data[(j-1)*wide*3+(i-1)*3]+p_data[(j-1)*wide+i*3]
					+p_data[(j-1)*wide*3+(i+1)*3]+p_data[j*wide+(i-1)*3]
					+p_data[j*wide*3+(i+1)*3]+p_data[(j+1)*wide+(i-1)*3]
					+p_data[(j+1)*wide*3+i*3]+p_data[(j+1)*wide+(i+1)*3])/8;
				averg2=(int)(p_data[(j-1)*wide*3+(i-1)*3+1]+p_data[(j-1)*wide+i*3+1]
					+p_data[(j-1)*wide*3+(i+1)*3+1]+p_data[j*wide+(i-1)*3+1]
					+p_data[j*wide*3+(i+1)*3+1]+p_data[(j+1)*wide+(i-1)*3+1]
					+p_data[(j+1)*wide*3+i*3+1]+p_data[(j+1)*wide+(i+1)*3+1])/8;
				averg3=(int)(p_data[(j-1)*wide*3+(i-1)*3+2]+p_data[(j-1)*wide+i*3+2]
					+p_data[(j-1)*wide*3+(i+1)*3+2]+p_data[j*wide+(i-1)*3+2]
					+p_data[j*wide*3+(i+1)*3+2]+p_data[(j+1)*wide+(i-1)*3+2]
					+p_data[(j+1)*wide*3+i*3+2]+p_data[(j+1)*wide+(i+1)*3+2])/8;
				if(abs(p_temp[j*wide*3+i*3]-averg)>T)
					p_temp[j*wide*3+i*3]=averg;
				if(abs(p_temp[j*wide*3+i*3+1]-averg2)>T)
					p_temp[j*wide*3+i*3+1]=averg2;
				if(abs(p_temp[j*wide*3+i*3+2]-averg3)>T)
					p_temp[j*wide*3+i*3*2]=averg3;
			}
		}
		memcpy(p_data,p_temp,wide*height*3);
		delete p_temp;
	}
}

/***************************************************************/ 
/*函数名称：nn(int n)                                          */
/*函数类型：void                                               */
/*参数：int n，设定的屏蔽窗口参数（奇数）                      */
/*功能：对图像进行n*n均值滤波处理。                            */
/***************************************************************/
void ZaoShengXiaoChuDib::nn(int n)
{
    DWORD size;
	size=GetSize();
	BYTE *p_data;
	int xx,yy,n2,sum,sum2,sum3;
	int wide,height;    //原图长、宽
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		BYTE* p_temp=new BYTE [size];
		memset(p_temp,255,size);
		if(n<3||n%2!=1)//确认n为奇数
			AfxMessageBox("请输入一个大于等于3的奇数");
		if(n>=3&&n%2==1)
			n2=(n-1)/2;
		p_data=this->GetData();//取得原图的数据区指针
		wide=this->GetWidth(); //取得原图的数据区宽
		height=this->GetHeight();  //取得原图的数据区高
		//用N*N屏蔽窗口平均化
		for(int j=n2;j<height-n2;j++)
		{
			for(int i=n2;i<wide-n2;i++)
			{	//求N*N屏蔽窗口内的总和sum
				sum=0;
				for(yy=j-n2;yy<=j+n2;yy++)
					for(xx=i-n2;xx<=i+n2;xx++)
						sum+=p_data[yy*wide+xx];
				//把n*n屏蔽窗口内的平均值四舍五入后作为显示图像像素值
				p_temp[j*wide+i]=(int)((float)sum/(n*n)+0.5);
					
			}
		}
		memcpy(p_data,p_temp,size);
		delete p_temp;
	}
	else	//24位彩色
	{			
		if(n<3||n%2!=1)//确认n为奇数
			AfxMessageBox("请输入一个大于等于3的奇数");
		if(n>=3&&n%2==1)
			n2=(n-1)/2;
		p_data=this->GetData();//取得原图的数据区指针
		wide=this->GetWidth(); //取得原图的数据区宽
		height=this->GetHeight();  //取得原图的数据区高
		BYTE* p_temp=new BYTE [wide*height*3];
		memset(p_temp,255,wide*height*3);
		//用N*N屏蔽窗口平均化
		for(int j=n2;j<height-n2;j++)
		{
			for(int i=n2;i<wide-n2;i++)
			{	
				//求N*N屏蔽窗口内的总和sum
				sum=0;
				sum2=0;
				sum3=0;
				for(yy=j-n2;yy<=j+n2;yy++)
					for(xx=i-n2;xx<=i+n2;xx++)
					{
						sum+=p_data[yy*wide*3+xx*3];
						sum2+=p_data[yy*wide*3+xx*3+1];
						sum3+=p_data[yy*wide*3+xx*3+2];
					}
				//把n*n屏蔽窗口内的平均值四舍五入后作为显示图像像素值
				p_temp[j*wide*3+i*3]=(int)((float)sum/(n*n)+0.5);
				p_temp[j*wide*3+i*3+1]=(int)((float)sum2/(n*n)+0.5);
				p_temp[j*wide*3+i*3+2]=(int)((float)sum3/(n*n)+0.5);
			}
		}
		memcpy(p_data,p_temp,wide*height*3);
		delete p_temp;
	}
} 

/***************************************************************/ 
/*函数名称：nnzhong(int n)                                     */
/*函数类型：void                                               */
/*参数：int n，设定的屏蔽窗口参数（奇数）                      */
/*功能：对图像进行n*n中值滤波处理。                            */
/***************************************************************/
void ZaoShengXiaoChuDib::nnzhong(int n)
{
	DWORD size;
	size=GetSize();
	BYTE* p_temp=new BYTE [size];
	memset(p_temp,255,size);
	int yy,xx,n2,nn,chuo,chg,m,medi,medi2,medi3,madom,mado[1000],mado2[1000],mado3[1000];
	BYTE *p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	if(n<3||n%2!=1)//检查取值是否为3.5.7等的奇数
		AfxMessageBox("请输入一个大于等于3的奇数");
	if(n>=3&&n%2==1)
		n2=(n-1)/2;
    nn=n*n;
	chuo=(nn-1)/2;
	p_data=this->GetData();//取得原图的数据区指针
	wide=this->GetWidth(); //取得原图的数据区宽
    height=this->GetHeight();  //取得原图的数据区高
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		//n*n中值滤波
		for(int j=n2;j<height-n2;j++)
			for(int i=n2;i<wide-n2;i++)
			{
				//把n*n屏蔽窗口部分的所有像素值放入mado[m]
				m=0;
				for(yy=j-n2;yy<=j+n2;yy++)
					for(xx=i-n2;xx<=i+n2;xx++)
					{
						mado[m]=p_data[yy*wide+xx];
						m++;
					}
				//把mado[m]中的值按下降顺序用冒泡法排序
				do{
					chg=0;
					for(m=0;m<nn-1;m++)
					{
						if(mado[m]<mado[m+1])
						{
							madom=mado[m];
							mado[m]=mado[m+1];
							mado[m+1]=madom;
							chg=1;
						}
					}
				}while(chg==1);	
				//求中值medi
				medi=mado[chuo];
				//把中值代入显示图像中
				p_temp[j*wide+i]=medi;
			}
		memcpy(p_data,p_temp,size);
		delete p_temp;
	}
	else	//24位彩色
	{
		//n*n中值滤波
		for(int j=n2;j<height-n2;j++)
			for(int i=n2;i<wide-n2;i++)
			{
				//把n*n屏蔽窗口部分的所有像素值放入mado[m]
				m=0;
				for(yy=j-n2;yy<=j+n2;yy++)
					for(xx=i-n2;xx<=i+n2;xx++)
					{
						mado[m]=p_data[yy*wide*3+xx*3];
						mado2[m]=p_data[yy*wide*3+xx*3+1];
						mado3[m]=p_data[yy*wide*3+xx*3+2];
						m++;
					}
				//把mado[m]中的值按下降顺序用冒泡法排序
				do{
					chg=0;
					for(m=0;m<nn-1;m++)
					{
						if(mado[m]<mado[m+1])
						{
							madom=mado[m];
							mado[m]=mado[m+1];
							mado[m+1]=madom;
							chg=1;
						}
						if(mado2[m]<mado2[m+1])
						{
							madom=mado2[m];
							mado2[m]=mado2[m+1];
							mado2[m+1]=madom;
							chg=1;
						}
						if(mado3[m]<mado3[m+1])
						{
							madom=mado3[m];
							mado3[m]=mado3[m+1];
							mado3[m+1]=madom;
							chg=1;
						}
					}
				}while(chg==1);	
				//求中值medi
				medi=mado[chuo];
				medi2=mado2[chuo];
				medi3=mado3[chuo];
				//把中值代入显示图像中
				p_temp[j*wide*3+i*3]=medi;
				p_temp[j*wide*3+i*3+1]=medi2;
				p_temp[j*wide*3+i*3+2]=medi3;
			}
		memcpy(p_data,p_temp,wide*height*3);
		delete p_temp;
	}
}

/***************************************************************/
/*函数名称：shizi(int n)                                       */
/*函数类型：void                                               */
/*参数：int n，设定的屏蔽窗口参数（奇数）                      */
/*功能：对图像进行十字型中值滤波。                             */
/***************************************************************/
void ZaoShengXiaoChuDib::shizi(int n)
{ 
    DWORD size;
	size=GetSize();
	BYTE* p_temp=new BYTE [size];
	memset(p_temp,255,size);
    int yy,xx,n2,nn,chuo,chg,m,medi,medi2,medi3,madom,mado[1000],mado2[1000],mado3[1000];
	BYTE *p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	if(n<3||n%2!=1)//检查取值是否为3.5.7等的奇数
	AfxMessageBox("请输入一个大于等于3的奇数");
	if(n>=3&&n%2==1)
    n2=(n-1)/2;
    nn=n+n-1;
    chuo=(nn-1)/2;
	p_data=this->GetData();//取得原图的数据区指针
	wide=this->GetWidth(); //取得原图的数据区宽
    height=this->GetHeight();  //取得原图的数据区高
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		//十字型中值滤波
		for(int j=n2;j<height-n2;j++)
		for(int i=n2;i<wide-n2;i++)
		{
			//把十字型屏蔽窗口部分的所有像素值代入mado[m]
			//(代入含中心的垂直部分)
			m=0;
			for(yy=j-n2;yy<=j+n2;yy++)
			{
				mado[m]=p_data[yy*wide+i];
				m++;
			}
			//（代入中心以外的的水平部分）
			for(xx=i-n2;xx<=i+n2;xx++)
			{
				if(xx==i)continue;
				mado[m]=p_data[j*wide+xx];
				m++;
			}
			//把mado[m]的内容按下降顺序冒泡法分类
			do{
				chg=0;
				for(m=0;m<nn-1;m++)
				{
					if(mado[m]<mado[m+1])
					{
						madom=mado[m];
						mado[m]=mado[m+1];
						mado[m+1]=madom;
						chg=1;
					}
				}
			}while(chg==1);
			//求中值medi
			medi=mado[chuo];
			p_temp[j*wide+i]=medi;
		}		
		memcpy(m_pData,p_temp,size);
		delete p_temp;
	}
	else	//24位彩色
	{	
		//十字型中值滤波
		for(int j=n2;j<height-n2;j++)
		for(int i=n2;i<wide-n2;i++)
		{
			//把十字型屏蔽窗口部分的所有像素值代入mado[m]
			//(代入含中心的垂直部分)
			m=0;
			for(yy=j-n2;yy<=j+n2;yy++)
			{
				mado[m]=p_data[yy*wide*3+i*3];
				mado2[m]=p_data[yy*wide*3+i*3+1];
				mado3[m]=p_data[yy*wide*3+i*3+2];
				m++;
			}
			//（代入中心以外的的水平部分）
			for(xx=i-n2;xx<=i+n2;xx++)
			{
				if(xx==i)continue;
				mado[m]=p_data[j*wide*3+xx*3];
				mado2[m]=p_data[j*wide*3+xx*3+1];
				mado3[m]=p_data[j*wide*3+xx*3+2];
				m++;
			}
			//把mado[m]的内容按下降顺序冒泡法分类
			do{
				chg=0;
				for(m=0;m<nn-1;m++)
				{
					if(mado[m]<mado[m+1])
					{
						madom=mado[m];
						mado[m]=mado[m+1];
						mado[m+1]=madom;
						chg=1;
					}
					if(mado2[m]<mado2[m+1])
					{
						madom=mado2[m];
						mado2[m]=mado2[m+1];
						mado2[m+1]=madom;
						chg=1;
					}
					if(mado3[m]<mado3[m+1])
					{
						madom=mado3[m];
						mado3[m]=mado3[m+1];
						mado3[m+1]=madom;
						chg=1;
					}
				}
			}while(chg==1);
			//求中值medi
			medi=mado[chuo];
			medi2=mado2[chuo];
			medi3=mado3[chuo];
			p_temp[j*wide*3+i*3]=medi;
			p_temp[j*wide*3+i*3+1]=medi2;
			p_temp[j*wide*3+i*3+2]=medi3;					
		}		
		memcpy(m_pData,p_temp,wide*height*3);
		delete p_temp;
	}
}

/***************************************************************/ 
/*函数名称：nnzuida(int n)                                     */
/*函数类型：void                                               */
/*参数：int n，设定的屏蔽窗口参数（奇数）                      */
/*功能：对图像进行N*N最大值滤波。                              */
/***************************************************************/
void ZaoShengXiaoChuDib::nnzuida(int n)
{
    DWORD size;
	size=GetSize();
	int yy,xx,n2,nn,m,madomax,madomax2,madomax3,mado[1000],mado2[1000],mado3[1000];
	BYTE *p_data;     //原图数据区指针
	int wide,height;
    BYTE* p_temp=new BYTE [size];
	memset(p_temp,255,size);
	if(n<3||n%2!=1)//确认n是3以上的奇数
	AfxMessageBox("请输入一个大于等于3的奇数");
	if(n>=3&&n%2==1)
	n2=(n-1)/2;
	nn=n*n;
	p_data=this->GetData();//取得原图的数据区指针
   	wide=this->GetWidth(); //取得原图的数据区宽
    height=this->GetHeight();  //取得原图的数据区高
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		//n*n最大值频率滤波
		for(int j=n2;j<height-n2;j++)
		for(int i=n2;i<wide-n2;i++)
		{
			//把n*n屏蔽窗口部分复制到mado[m]
			m=0;
			for(yy=j-n2;yy<=j+n2;yy++)
			for(xx=i-n2;xx<=i+n2;xx++)
			{
				mado[m]=p_data[yy*wide+xx];
				m++;
			}
			//找出mado[m]中的最大值madomax,用作最大频率值
			madomax=mado[0];
			for(m=1;m<nn;m++)
				if(madomax<mado[m])
				madomax=mado[m];
			p_temp[j*wide+i]=madomax;
		}                 
		memcpy(m_pData,p_temp,size);
		delete p_temp;
	}
	else	//24位彩色
	{		    
		//n*n最大值频率滤波
		for(int j=n2;j<height-n2;j++)
		for(int i=n2;i<wide-n2;i++)
		{
			//把n*n屏蔽窗口部分复制到mado[m]
			m=0;
			for(yy=j-n2;yy<=j+n2;yy++)
			for(xx=i-n2;xx<=i+n2;xx++)
			{
				mado[m]=p_data[yy*wide*3+xx*3];
				mado2[m]=p_data[yy*wide*3+xx*3+1];
				mado3[m]=p_data[yy*wide*3+xx*3+2];
				m++;
			}
			//找出mado[m]中的最大值madomax,用作最大频率值
			madomax=mado[0];
			madomax2=mado2[0];
			madomax3=mado3[0];
			for(m=1;m<nn;m++)
			{
				if(madomax<mado[m])
					madomax=mado[m];
				if(madomax2<mado2[m])
					madomax2=mado2[m];
				if(madomax3<mado3[m])
					madomax3=mado3[m];
			}
			p_temp[j*wide*3+i*3]=madomax;
			p_temp[j*wide*3+i*3+1]=madomax2;
			p_temp[j*wide*3+i*3+2]=madomax3;
		}                 
		memcpy(m_pData,p_temp,wide*height*3);
		delete p_temp;
	}
}

/***************************************************************/  
/*函数名称：suijizaosheng()                                    */
/*函数类型：void                                               */
/*功能：对图像进行随机噪声处理。                               */
/***************************************************************/
void ZaoShengXiaoChuDib::suijizaosheng()
{
	int noisepoint;
    DWORD size;
	size=GetSize();
	BYTE *p_data;     //原图数据区指针
	int wide,height;
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		BYTE* p_temp=new BYTE [size];
		memset(p_temp,255,size);
		p_data=this->GetData();//取得原图的数据区指针
		memcpy(p_temp,p_data,size);
		wide=this->GetWidth(); //取得原图的数据区宽
		height=this->GetHeight();  //取得原图的数据区高
		//产生随机噪声
		for(int j=0;j<height;j++)
		for(int i=0;i<wide;i++)
		{
			noisepoint=rand()/1024;
			p_temp[j*wide+i]=(p_temp[j*wide+i]*224/256+(int)noisepoint);
		}
		memcpy(m_pData,p_temp,size);
		delete p_temp;
	}
	else	//24位彩色
	{
		size=GetSize();
		BYTE* p_temp=new BYTE [size];
		memset(p_temp,255,size);
		p_data=this->GetData();//取得原图的数据区指针
		memcpy(p_temp,p_data,size);
		wide=this->GetWidth()*3; //取得原图的数据区宽
		height=this->GetHeight();  //取得原图的数据区高
		//产生随机噪声
		for(int j=0;j<height;j++)
		for(int i=0;i<wide;i++)
		{
			noisepoint=rand()/1024;
			p_temp[j*wide+i]=(p_temp[j*wide+i]*224/256+(int)noisepoint);
		}
		memcpy(m_pData,p_temp,size);
		delete p_temp;
	}
}

/***************************************************************/
/*函数名称：jiaoyanzaosheng()                                  */
/*函数类型：void                                               */
/*功能：对图像进行椒盐噪声处理。                               */
/***************************************************************/
void ZaoShengXiaoChuDib::jiaoyanzaosheng()
{
    DWORD size;
	size=GetSize();
	BYTE *p_data;     //原图数据区指针
	int wide,height;
    BYTE* p_temp=new BYTE [size];
	p_data=this->GetData();//取得原图的数据区指针
	memcpy(p_temp,p_data,size);
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		wide=this->GetWidth(); //取得原图的数据区宽
		height=this->GetHeight();  //取得原图的数据区高
		for(int j=0;j<height;j++)
		for(int i=0;i<wide;i++)
		{
			if(rand()>31500)
				p_temp[j*wide+i]=0;
		}
		memcpy(m_pData,p_temp,size);
		delete p_temp;
	}
	else	//24位彩色
	{
		size=GetSize();
		BYTE* p_temp=new BYTE [size];
		p_data=this->GetData();//取得原图的数据区指针
		memcpy(p_temp,p_data,size);
		wide=this->GetWidth()*3; //取得原图的数据区宽
		height=this->GetHeight();  //取得原图的数据区高
		for(int j=0;j<height;j++)
		for(int i=0;i<wide;i++)
		{
			if(rand()>31500)
				p_temp[j*wide+i]=0;
		}
		memcpy(m_pData,p_temp,size);
		delete p_temp;
	}
}

/***************************************************************/ 
/*函数名称：jubupingjun()                                      */
/*函数类型：void                                               */
/*功能：对图像进行局部平均化处理。                             */
/***************************************************************/
void ZaoShengXiaoChuDib::jubupingjun() 
{
    DWORD size;
	size=GetSize();
	BYTE *p_data;//原图数据区指针
	BYTE* p_temp=new BYTE[size];
	int wide,height;
    int n,ji[9],nmin;
	float mean[9],bunsan[9],bmin;
	p_data=this->GetData();//取得原图的数据区指针
	wide=this->GetWidth(); //取得原图的数据区宽
    height=this->GetHeight();  //取得原图的数据区高
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{    
		memset(p_temp,255,size);
		for(int j=2;j<=height-3;j++)
		for(int i=2;i<=wide-3;i++)
		{
		//求9种近邻区域的均值及其方差
			//第1近邻区域
			ji[0]=p_data[(j-1)*wide+(i-1)];
			ji[1]=p_data[(j-1)*wide+i];
			ji[2]=p_data[(j-1)*wide+(i+1)];
			ji[3]=p_data[j*wide+(i-1)];
			ji[4]=p_data[j*wide+i];
			ji[5]=p_data[j*wide+(i+1)];
			ji[6]=p_data[(j+1)*wide+(i-1)];
			ji[7]=p_data[(j+1)*wide+i];
			ji[8]=p_data[(j+1)*wide+(i+1)];
			mean[0]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6]+ji[7]+ji[8])/9;
			bunsan[0]=0;
			for(n=0;n<=8;n++)
				bunsan[0]+=ji[n]*ji[n]-mean[0]*mean[0];
			//第2近邻区域
			ji[0]=p_data[(j-2)*wide+(i-1)];
			ji[1]=p_data[(j-2)*wide+i];
			ji[2]=p_data[(j-2)*wide+(i+1)];
			ji[3]=p_data[(j-1)*wide+(i-1)];
			ji[4]=p_data[(j-1)*wide+i];
			ji[5]=p_data[(j-1)*wide+(i+1)];
			ji[6]=p_data[j*wide+i];
			mean[1]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[1]=0;
			for(n=0;n<=6;n++)
				bunsan[1]+=ji[n]*ji[n]-mean[1]*mean[1];
			//第3近邻区域
			ji[0]=p_data[(j-1)*wide+(i-2)];
			ji[1]=p_data[(j-1)*wide+(i-1)];
			ji[2]=p_data[j*wide+(i-2)];
			ji[3]=p_data[j*wide+(i-1)];
			ji[4]=p_data[j*wide+i];
			ji[5]=p_data[(j+1)*wide+(i-2)];
			ji[6]=p_data[(j+1)*wide+(i-1)];
			mean[2]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[2]=0;
			for(n=0;n<=6;n++)
				bunsan[2]+=ji[n]*ji[n]-mean[2]*mean[2];
			//第4近邻区域
			ji[0]=p_data[j*wide+i];
			ji[1]=p_data[(j+1)*wide+(i-1)];
			ji[2]=p_data[(j+1)*wide+i];
			ji[3]=p_data[(j+1)*wide+(i+1)];
			ji[4]=p_data[(j+2)*wide+(i-1)];
			ji[5]=p_data[(j+2)*wide+i];
			ji[6]=p_data[(j+2)*wide+(i+1)];
			mean[3]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  		bunsan[3]=0;
			for(n=0;n<=6;n++)
				bunsan[3]+=ji[n]*ji[n]-mean[3]*mean[3];
			//第5近邻区域
			ji[0]=p_data[(j-1)*wide+(i+1)];
			ji[1]=p_data[(j-1)*wide+(i+2)];
			ji[2]=p_data[j*wide+i];
			ji[3]=p_data[j*wide+(i+1)];
			ji[4]=p_data[j*wide+(i+2)];
			ji[5]=p_data[(j+1)*wide+(i+1)];
			ji[6]=p_data[(j+1)*wide+(i+2)]; 
			mean[4]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[4]=0;
			for(n=0;n<=6;n++)
				bunsan[4]+=ji[n]*ji[n]-mean[4]*mean[4];	
			//第6近邻区域
			ji[0]=p_data[(j-2)*wide+(i+1)];
			ji[1]=p_data[(j-2)*wide+(i+2)];
			ji[2]=p_data[(j-1)*wide+i];
			ji[3]=p_data[(j-1)*wide+(i+1)];
			ji[4]=p_data[(j-1)*wide+(i+2)];
			ji[5]=p_data[j*wide+i];
			ji[6]=p_data[j*wide+(i+1)]; 
			mean[5]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  		bunsan[5]=0;
			for(n=0;n<=6;n++)
				bunsan[5]+=ji[n]*ji[n]-mean[5]*mean[5];
			//第7近邻区域
			ji[0]=m_pData[(j-2)*wide+(i-2)];
			ji[1]=p_data[(j-2)*wide+(i-1)];
			ji[2]=p_data[(j-1)*wide+(i-2)];
			ji[3]=p_data[(j-1)*wide+(i-1)];
			ji[4]=p_data[(j-1)*wide+i];
			ji[5]=p_data[j*wide+(i-1)];
			ji[6]=p_data[j*wide+i];
			mean[6]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	 		bunsan[6]=0;
			for(n=0;n<=6;n++)
				bunsan[6]+=ji[n]*ji[n]-mean[6]*mean[6];
			//第8近邻区域
			ji[0]=m_pData[j*wide+(i-1)];
			ji[1]=p_data[j*wide+i];
			ji[2]=p_data[(j+1)*wide+(i-2)];
			ji[3]=p_data[(j+1)*wide+(i-1)];
			ji[4]=p_data[(j+1)*wide+i];
			ji[5]=p_data[(j+2)*wide+(i-2)];
			ji[6]=p_data[(j+2)*wide+(i-1)];
			mean[7]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  		bunsan[7]=0;
			for(n=0;n<=6;n++)
				bunsan[7]+=ji[n]*ji[n]-mean[7]*mean[7];
			//第9近邻区域
			ji[0]=p_data[j*wide+i];
			ji[1]=p_data[j*wide+(i+1)];
			ji[2]=p_data[(j+1)*wide+i];
			ji[3]=p_data[(j+1)*wide+(i+1)];
			ji[4]=p_data[(j+1)*wide+(i+2)];
			ji[5]=p_data[(j+2)*wide+(i+1)];
			ji[6]=p_data[(j+2)*wide+(i+2)];
			mean[8]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  		bunsan[8]=0;
			for(n=0;n<=6;n++)
				bunsan[8]+=ji[n]*ji[n]-mean[8]*mean[8];
			//求方差最小的近邻区域nmin
			bmin=bunsan[0];
			nmin=0;
			for(n=0;n<=8;n++)
			{
				if(bmin>bunsan[n])
				{
					bmin=bunsan[n];
					nmin=n;
				}
				//把nmin的值四舍五入后作为显示图像的值
				p_temp[j*wide+i]=(int)(mean[nmin]+0.5);
			}				 
		}
		memcpy(p_data,p_temp,size);
		delete p_temp;	
	}
	else	//24位彩色
	{		    
		p_temp=new BYTE[wide*height*3];
		memset(p_temp,255,wide*height*3);
		for(int k=0;k<3;k++)
		for(int j=2;j<=height-3;j++)
		{
			for(int i=2;i<=wide-3;i++)
			{
			//求9种近邻区域的均值及其方差
				//第1近邻区域
				ji[0]=p_data[(j-1)*wide*3+(i-1)*3+k+k];
				ji[1]=p_data[(j-1)*wide*3+i*3+k];
				ji[2]=p_data[(j-1)*wide*3+(i+1)*3+k];
				ji[3]=p_data[j*wide*3+(i-1)*3+k];
				ji[4]=p_data[j*wide*3+i*3+k];
				ji[5]=p_data[j*wide*3+(i+1)*3+k];
				ji[6]=p_data[(j+1)*wide*3+(i-1)*3+k];
				ji[7]=p_data[(j+1)*wide*3+i*3+k];
				ji[8]=p_data[(j+1)*wide*3+(i+1)*3+k];
				mean[0]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6]+ji[7]+ji[8])/9;
				bunsan[0]=0;
				for(n=0;n<=8;n++)
					bunsan[0]+=ji[n]*ji[n]-mean[0]*mean[0];
				//第2近邻区域
				ji[0]=p_data[(j-2)*wide*3+(i-1)*3+k];
				ji[1]=p_data[(j-2)*wide*3+i*3+k];
				ji[2]=p_data[(j-2)*wide*3+(i+1)*3+k];
				ji[3]=p_data[(j-1)*wide*3+(i-1)*3+k];
				ji[4]=p_data[(j-1)*wide*3+i*3+k];
				ji[5]=p_data[(j-1)*wide*3+(i+1)*3+k];
				ji[6]=p_data[j*wide*3+i*3+k];
				mean[1]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
				bunsan[1]=0;
				for(n=0;n<=6;n++)
					bunsan[1]+=ji[n]*ji[n]-mean[1]*mean[1];
				//第3近邻区域
				ji[0]=p_data[(j-1)*wide*3+(i-2)*3+k];
				ji[1]=p_data[(j-1)*wide*3+(i-1)*3+k];
				ji[2]=p_data[j*wide*3+(i-2)*3+k];
				ji[3]=p_data[j*wide*3+(i-1)*3+k];
				ji[4]=p_data[j*wide*3+i*3+k];
				ji[5]=p_data[(j+1)*wide*3+(i-2)*3+k];
				ji[6]=p_data[(j+1)*wide*3+(i-1)*3+k];
				mean[2]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
				bunsan[2]=0;
				for(n=0;n<=6;n++)
					bunsan[2]+=ji[n]*ji[n]-mean[2]*mean[2];
				//第4近邻区域
				ji[0]=p_data[j*wide*3+i*3+k];
				ji[1]=p_data[(j+1)*wide*3+(i-1)*3+k];
				ji[2]=p_data[(j+1)*wide*3+i*3+k];
				ji[3]=p_data[(j+1)*wide*3+(i+1)*3+k];
				ji[4]=p_data[(j+2)*wide*3+(i-1)*3+k];
				ji[5]=p_data[(j+2)*wide*3+i*3+k];
				ji[6]=p_data[(j+2)*wide*3+(i+1)*3+k];
				mean[3]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  			bunsan[3]=0;
				for(n=0;n<=6;n++)
					bunsan[3]+=ji[n]*ji[n]-mean[3]*mean[3];
				//第5近邻区域
				ji[0]=p_data[(j-1)*wide*3+(i+1)*3+k];
				ji[1]=p_data[(j-1)*wide*3+(i+2)*3+k];
				ji[2]=p_data[j*wide*3+i*3+k];
				ji[3]=p_data[j*wide*3+(i+1)*3+k];
				ji[4]=p_data[j*wide*3+(i+2)*3+k];
				ji[5]=p_data[(j+1)*wide*3+(i+1)*3+k];
				ji[6]=p_data[(j+1)*wide*3+(i+2)*3+k]; 
				mean[4]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
				bunsan[4]=0;
				for(n=0;n<=6;n++)
					bunsan[4]+=ji[n]*ji[n]-mean[4]*mean[4];	
				//第6近邻区域
				ji[0]=p_data[(j-2)*wide*3+(i+1)*3+k];
				ji[1]=p_data[(j-2)*wide*3+(i+2)*3+k];
				ji[2]=p_data[(j-1)*wide*3+i*3+k];
				ji[3]=p_data[(j-1)*wide*3+(i+1)*3+k];
				ji[4]=p_data[(j-1)*wide*3+(i+2)*3+k];
				ji[5]=p_data[j*wide*3+i*3+k];
				ji[6]=p_data[j*wide*3+(i+1)*3+k]; 
				mean[5]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  			bunsan[5]=0;
				for(n=0;n<=6;n++)
					bunsan[5]+=ji[n]*ji[n]-mean[5]*mean[5];
				//第7近邻区域
				ji[0]=m_pData[(j-2)*wide*3+(i-2)*3+k];
				ji[1]=p_data[(j-2)*wide*3+(i-1)*3+k];
				ji[2]=p_data[(j-1)*wide*3+(i-2)*3+k];
				ji[3]=p_data[(j-1)*wide*3+(i-1)*3+k];
				ji[4]=p_data[(j-1)*wide*3+i*3+k];
				ji[5]=p_data[j*wide*3+(i-1)*3+k];
				ji[6]=p_data[j*wide*3+i*3+k];
				mean[6]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	 			bunsan[6]=0;
				for(n=0;n<=6;n++)
					bunsan[6]+=ji[n]*ji[n]-mean[6]*mean[6];
				//第8近邻区域
				ji[0]=m_pData[j*wide*3+(i-1)*3+k];
				ji[1]=p_data[j*wide*3+i*3+k];
				ji[2]=p_data[(j+1)*wide*3+(i-2)*3+k];
				ji[3]=p_data[(j+1)*wide*3+(i-1)*3+k];
				ji[4]=p_data[(j+1)*wide*3+i*3+k];
				ji[5]=p_data[(j+2)*wide*3+(i-2)*3+k];
				ji[6]=p_data[(j+2)*wide*3+(i-1)*3+k];
				mean[7]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  			bunsan[7]=0;
				for(n=0;n<=6;n++)
					bunsan[7]+=ji[n]*ji[n]-mean[7]*mean[7];
				//第9近邻区域
				ji[0]=p_data[j*wide*3+i*3+k];
				ji[1]=p_data[j*wide*3+(i+1)*3+k];
				ji[2]=p_data[(j+1)*wide*3+i*3+k];
				ji[3]=p_data[(j+1)*wide*3+(i+1)*3+k];
				ji[4]=p_data[(j+1)*wide*3+(i+2)*3+k];
				ji[5]=p_data[(j+2)*wide*3+(i+1)*3+k];
				ji[6]=p_data[(j+2)*wide*3+(i+2)*3+k];
				mean[8]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
	  			bunsan[8]=0;
				for(n=0;n<=6;n++)
					bunsan[8]+=ji[n]*ji[n]-mean[8]*mean[8];
				//求方差最小的近邻区域nmin
				bmin=bunsan[0];
				nmin=0;
				for(n=0;n<=8;n++)
				{
					if(bmin>bunsan[n])
					{
						bmin=bunsan[n];
						nmin=n;
					}
					//把nmin的值四舍五入后作为显示图像的值
					p_temp[j*wide*3+i*3+k]=(int)(mean[nmin]+0.5);		
				}				 
			}
		}
		memcpy(p_data,p_temp,size);
		delete p_temp;
	}
}

