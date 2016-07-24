// JisuanProcessDib.cpp: implementation of the JisuanProcessDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSplit.h"
#include "JisuanProcessDib.h"
#include "SquareDlg.h"
#include "LINEDLG.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JisuanProcessDib::JisuanProcessDib()
{
	x_sign=0;
	m_temp=0;
	x_temp=0;
	y_temp=0;
    p_temp=0;
	stop=0;
}

JisuanProcessDib::~JisuanProcessDib()
{
	
}

/***************************************************************/
/*函数名称：erzhihua(int yuzhi_gray)                           */
/*函数类型：void                                               */
/*参数：int yuzhi_gray，用户给定的阈值                         */
/*功能：对图像使用固定阈值法进行二值化。                       */
/***************************************************************/
void JisuanProcessDib::erzhihua(int yuzhi_gray)
{
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
    if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		for(int j=0;j<height;j++)	// 每行
			for(int i=0;i<wide;i++)	// 每列
			{
				int temp=0;
				temp= *(p_data+(height-j-1)*wide+i);
				if(temp<yuzhi_gray)
					*(p_data+(height-j-1)*wide+i)=0;
				else
					*(p_data+(height-j-1)*wide+i)=255;
			}
	}
	else	//24位彩色
	{
		for(int j=0;j<height;j++)	// 每行
			for(int i=0;i<wide;i++)	// 每列
			{
				int temp=0;
				temp= *(p_data+(height-j-1)*wide*3+i*3);
				if(temp<yuzhi_gray)
				{
					*(p_data+(height-j-1)*wide*3+i*3)=0;
					*(p_data+(height-j-1)*wide*3+i*3+1)=0;
					*(p_data+(height-j-1)*wide*3+i*3+2)=0;
				}
				else
				{
					*(p_data+(height-j-1)*wide*3+i*3)=255;
					*(p_data+(height-j-1)*wide*3+i*3+1)=255;
					*(p_data+(height-j-1)*wide*3+i*3+2)=255;
				}
			}
	}
}

/***************************************************************/
/*函数名称：xiaochugulidianHEI()                               */
/*函数类型：void                                               */
/*功能：消除孤立黑点。                                         */
/***************************************************************/
void JisuanProcessDib::xiaochugulidianHEI()
{
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		for(int j=1;j<height-1;j++)	// 每行
			for(int i=1;i<wide-1;i++)	// 每列
			{
				int temp=0;	
				if(*(p_data+(height-j-1)*wide+i)==0)//本身为黑点
				{
					for(int m=0;m<3;m++)
						for(int n=0;n<3;n++)
						{
							temp+=*(p_data+(height-j-m)*wide+i+n-1);
						}
					if(temp>=255*6)///周围8个中点有大于等于6个白点
						*(p_data+(height-j-1)*wide+i)=255;
				}
			}
	}
	else	//24位彩色
	{
		for(int j=1;j<height-1;j++)	// 每行
			for(int i=1;i<wide-1;i++)	// 每列
			{
				int temp=0;	
				if(*(p_data+(height-j-1)*wide*3+i*3)==0)//本身为黑点
				{
					for(int m=0;m<3;m++)
						for(int n=0;n<3;n++)
						{
							temp+=*(p_data+(height-j-m)*wide*3+(i+n-1)*3);
						}
					if(temp>=255*6)///周围8个中点有大于等于6个白点
					{
						*(p_data+(height-j-1)*wide*3+i*3)=255;
						*(p_data+(height-j-1)*wide*3+i*3+1)=255;
						*(p_data+(height-j-1)*wide*3+i*3+2)=255;
					}
				}
			}
	}
}

/***************************************************************/
/*函数名称：xiaochugulidianBAI()                               */
/*函数类型：void                                               */
/*功能：消除孤立白点。                                         */
/***************************************************************/
void JisuanProcessDib::xiaochugulidianBAI()
{
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		for(int j=1;j<height-1;j++)	// 每行
			for(int i=1;i<wide-1;i++)	// 每列
			{
				int temp=0;			;
				if(*(p_data+(height-j-1)*wide+i)==255)//本身为白点
				{
					for(int m=0;m<3;m++)
						for(int n=0;n<3;n++)
						{
							temp+=*(p_data+(height-j-m)*wide+i+n-1);
						}
					if(temp<=255*3)///周围8个点中有小于等于2个白点
						*(p_data+(height-j-1)*wide+i)=0;
				}
			}
	}
	else	//24位彩色
	{
		for(int j=1;j<height-1;j++)	// 每行
			for(int i=1;i<wide-1;i++)	// 每列
			{
				int temp=0;			;
				if(*(p_data+(height-j-1)*wide*3+i*3)==255)//本身为白点
				{
					for(int m=0;m<3;m++)
						for(int n=0;n<3;n++)
						{
							temp+=*(p_data+(height-j-m)*wide*3+(i+n-1)*3);
						}
					if(temp<=255*3)///周围8个点中有小于等于2个白点
					{
						*(p_data+(height-j-1)*wide*3+i*3)=0;
						*(p_data+(height-j-1)*wide*3+i*3+1)=0;
						*(p_data+(height-j-1)*wide*3+i*3+2)=0;
					}
				}
			}
	}
}

/***************************************************************/
/*函数名称：biaoji()                                           */
/*函数类型：void                                               */
/*功能：对图像进行标记,划分成不同的连通区域。                  */
/***************************************************************/
void JisuanProcessDib::biaoji()
{
	x_sign=0; 
	m_temp=0;
	x_temp=0;
	y_temp=0;
	stop=0;
	memset(flag,0,255);
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
    if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		p_temp=new BYTE[wide*height];//开辟一个临时内存区
		memset(p_temp,255,wide*height);
		//从左到右标号
		for(int j=1;j<height-1;j++)	// 每行
		{
			if(stop==1)//判断连通区是否太多
				break;
			for(int i=1;i<wide-1;i++)	// 每列
			{
				if(x_sign>250)
				{
					AfxMessageBox("连通区数目太多,请增大阈值");
					stop=1;
					break;
				}
				if(*(p_data+(height-j-1)*wide+i)==0)//若当前点为黑点
				{
					if(*(p_data+(height-j-1+1)*wide+i+1)==0)//右上
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i+1);
						x_temp=*(p_temp+(height-j-1+1)*wide+i+1);
						flag[x_temp]+=1;
						if(*(p_data+(height-j-1)*wide+i-1)==0&&*(p_temp+(height-j-1)*wide+i-1)!=x_temp)//左前
						{
							y_temp=*(p_temp+(height-j-1)*wide+i-1);
							for(int m=1;m<=height-1;m++)
								for(int n=1;n<=wide-1;n++)
								{
									if(*(p_temp+(height-m-1)*wide+n)==y_temp)
									{	
										flag[y_temp]=0;
										*(p_temp+(height-m-1)*wide+n)=x_temp;
										flag[x_temp]+=1;
									}
								}
						}//end//左前
						if(*(p_data+(height-j-1+1)*wide+i-1)==0&&*(p_temp+(height-j-1+1)*wide+i-1)!=x_temp)//左上
						{
							y_temp=*(p_temp+(height-j-1+1)*wide+i-1);
							for(int m=1;m<=height-1;m++)
								for(int n=1;n<=wide-1;n++)
								{
									if(*(p_temp+(height-m-1)*wide+n)==y_temp)
									{	
										flag[y_temp]=0;
										*(p_temp+(height-m-1)*wide+n)=x_temp;
										flag[x_temp]+=1;
									}
								}
						}//end//左上
					}
					else if(*(p_data+(height-j-1+1)*wide+i)==0)//正上
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i);
						x_temp=*(p_temp+(height-j-1+1)*wide+i);
						flag[x_temp]+=1;
					}
					else if(*(p_data+(height-j-1+1)*wide+i-1)==0)//左上
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i-1);
						x_temp=*(p_temp+(height-j-1+1)*wide+i-1);
						flag[x_temp]+=1;
					}
					else if(*(p_data+(height-j-1)*wide+i-1)==0)//左前
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1)*wide+i-1);
						x_temp=*(p_temp+(height-j-1)*wide+i-1);
						flag[x_temp]+=1;
					}
					else//没有
					{				
						++x_sign;
						m_temp=x_sign;
						*(p_temp+(height-j-1)*wide+i)=m_temp;
						flag[m_temp]=1;
						
					}
				}//end if
			}// 每列
		}//end 每行
	}
	else	//24位彩色
	{
		p_temp=new BYTE[wide*height];//开辟一个临时内存区
		memset(p_temp,255,wide*height);
		//从左到右标号
		for(int j=1;j<height-1;j++)	// 每行
		{
			if(stop==1)//判断连通区是否太多
				break;
			for(int i=1;i<wide-1;i++)	// 每列
			{
				if(x_sign>250)
				{
					AfxMessageBox("连通区数目太多,请增大阈值");
					stop=1;
					break;
				}
				if(*(p_data+(height-j-1)*wide*3+i*3)==0)//若当前点为黑点
				{
					
					if(*(p_data+(height-j-1+1)*wide*3+(i+1)*3)==0)//右上
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+(i+1));
						x_temp=*(p_temp+(height-j-1+1)*wide+(i+1));
						flag[x_temp]+=1;
						if(*(p_data+(height-j-1)*wide*3+(i-1)*3)==0&&*(p_temp+(height-j-1)*wide+(i-1))!=x_temp)//左前
						{
							y_temp=*(p_temp+(height-j-1)*wide+(i-1));
							for(int m=1;m<=height-1;m++)
								for(int n=1;n<=wide-1;n++)
								{
									if(*(p_temp+(height-m-1)*wide+n)==y_temp)
									{	
										flag[y_temp]=0;
										*(p_temp+(height-m-1)*wide+n)=x_temp;
										
										flag[x_temp]+=1;
									}
								}
						}//end//左前
						if(*(p_data+(height-j-1+1)*wide*3+(i-1)*3)==0&&*(p_temp+(height-j-1+1)*wide+(i-1))!=x_temp)//左上
						{
							y_temp=*(p_temp+(height-j-1+1)*wide+(i-1));
							for(int m=1;m<=height-1;m++)
								for(int n=1;n<=wide-1;n++)
								{
									if(*(p_temp+(height-m-1)*wide+n)==y_temp)
									{	
										flag[y_temp]=0;
										*(p_temp+(height-m-1)*wide+n)=x_temp;
										
										flag[x_temp]+=1;
									}
								}
						}//end//左上
					}
					else if(*(p_data+(height-j-1+1)*wide*3+i*3)==0)//正上
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i);
						x_temp=*(p_temp+(height-j-1+1)*wide+i);
						flag[x_temp]+=1;
					}
					else if(*(p_data+(height-j-1+1)*wide*3+(i-1)*3)==0)//左上
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+(i-1));
						x_temp=*(p_temp+(height-j-1+1)*wide+(i-1));
						flag[x_temp]+=1;
					}
					else if(*(p_data+(height-j-1)*wide*3+(i-1)*3)==0)//左前
					{
						*(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1)*wide+i-1);
						x_temp=*(p_temp+(height-j-1)*wide+(i-1));
						flag[x_temp]+=1;
					}
					else//没有
					{				
						++x_sign;
						m_temp=x_sign;
						*(p_temp+(height-j-1)*wide+i)=m_temp;
						
						flag[m_temp]=1;
					}
				}//end if
			}// 每列
		}//end 每行
	}
}

/***************************************************************/
/*函数名称：LianTong()                                         */
/*函数类型：void                                               */
/*功能：对连通区调整,输出每个连通区的面积。                    */
/***************************************************************/
void JisuanProcessDib::LianTong()
{
	biaoji();  //调用标记函数
	if(stop!=1)//判断连通区是否太多
	{
		int fg[255]={0};//定义一个数组
		memset(fg,0,255);//初始化赋值都为0
		int y_sign=0;
		int m_Area=0;//定义一个面积
		for(int i=1;i<=x_sign;i++)
		{
			if(flag[i]!=0)
			{
				if(fg[y_sign]==0)
				{
					fg[y_sign]=flag[i];
					++y_sign;
				}			
			}
			m_Area+=flag[i];
		}
		SquareDlg  dlg;//输出对话框
		dlg.m_number=y_sign;//输出连通区域个数
		dlg.m_squareALL=m_Area;//输出连通区域的总积
		CString ss[20];
		//在对话框里输出每个连通区的面积（区域像素个数）
		for(i=0;i<y_sign;i++)
		{
			ss[i].Format("连通区：%3d  该区面积:%10.0d",i+1,fg[i]);
			dlg.m_ShuChu+=ss[i];
		}
		dlg.DoModal();
		for(i=0;i<255;i++) //初试设置pp_area都为0
		{
			pppp[i].pp_area=0;
		}
		for(int t=1;t<=x_sign;t++)
		{	pppp[t].pp_number=t;
		for(int j=1;j<height-1;j++)
			for(int i=1;i<wide-1;i++)
			{
				if(*(p_temp+(height-j-1)*wide+i)==t)
				{
					pppp[t].pp_x=i;
					pppp[t].pp_y=j;
					pppp[t].pp_area=flag[t];
					break;
				}				
			}
		}
	}//end if(stop!=1)
}

/***************************************************************/
/*函数名称：ClearSMALL(int m_value)                            */
/*函数类型：void                                               */
/*参数：int m_value，用户给定的阈值                            */
/*功能：消除面积小于给定的阈值的小区域。                       */
/***************************************************************/
void JisuanProcessDib::ClearSMALL(int m_value)
{
	biaoji();  //调用标记函数
    if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		if(stop!=1)//判断连通区是否太多
		{
			for(int i=1;i<=x_sign;i++)
			{
				if(flag[i]<m_value)//判断连通区的面积（像素个数）是否消除
				{
					for(int m=1;m<height-1;m++)
						for(int n=1;n<wide-1;n++)
						{
							if(*(p_temp+(height-m-1)*wide+n)==i)
								*(p_data+(height-m-1)*wide+n)=255;
						}		
				}
			}
		}
	}
	else	//24位彩色
	{
		if(stop!=1)//判断连通区是否太多
		{
			for(int i=1;i<=x_sign;i++)
			{
				if(flag[i]<m_value)//判断连通区的面积（像素个数）是否消除
				{
					for(int m=1;m<height-1;m++)
						for(int n=1;n<wide-1;n++)
						{
							if(*(p_temp+(height-m-1)*wide+n)==i)
							{
								*(p_data+(height-m-1)*wide*3+n*3)=255;
								*(p_data+(height-m-1)*wide*3+n*3+1)=255;
								*(p_data+(height-m-1)*wide*3+n*3+2)=255;
							}
						}		
				}
			}
		}
	}
}

/***************************************************************/
/*函数名称：Borderline()                                       */
/*函数类型：void                                               */
/*功能：对每个连通区进行边界跟踪，提取边界，输出周长。         */
/***************************************************************/
void JisuanProcessDib::Borderline()
{
	biaoji();  //调用标记函数
	LPBYTE	lpSrc;  // 指向源图像的指针
	LPBYTE	lpDst;	// 指向缓存图像的指针
	LPBYTE	temp;  // 指向缓存DIB图像的指针
    int pixel;	//像素值
	bool bFindStartPoint;	//是否找到起始点及回到起始点
	bool bFindPoint;	//是否扫描到一个边界点
	Point StartPoint,CurrentPoint;	//起始边界点与当前边界点
	//八个方向和起始扫描方向
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;
	if(m_pBitmapInfoHeader->biBitCount<9)	//灰度图像
	{
		temp = new BYTE[wide*height]; 	// 暂时分配内存，以保存新图像
		lpDst = temp;	// 初始化新分配的内存，设定初始值为255
		memset(lpDst, (BYTE)255, wide * height);
		if(stop!=1)//判断连通区是否太多
		{
			//先找到最左上方的边界点
			for(int t=1;t<=x_sign;t++)
			{
				if(flag[t]!=0)
				{	
					bFindStartPoint = false;
					for (int j = 0;j < height && !bFindStartPoint;j++)
					{
						for(int i = 0;i < wide && !bFindStartPoint;i++)
						{
							// 指向源图像倒数第j行，第i个象素的指针			
							lpSrc =  (LPBYTE)(p_temp + wide * j + i);			
							//取得当前指针处的像素值，注意要转换为unsigned char型
							pixel =  *lpSrc;			
							if(pixel ==t)
							{
								bFindStartPoint = true;
								StartPoint.Height = j;
								StartPoint.Width = i;
								// 指向目标图像倒数第j行，第i个象素的指针			
								lpDst = (LPBYTE)(temp + wide * j + i);	
								*lpDst =  t;
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
							lpSrc = (LPBYTE)(p_temp + wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
								+ (CurrentPoint.Width + Direction[BeginDirect][0]));
							pixel =  *lpSrc;
							if(pixel == t)
							{
								bFindPoint = true;
								CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
								CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
								if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
								{
									bFindStartPoint = true;
								}
								lpDst =  (LPBYTE)(temp + wide * CurrentPoint.Height + CurrentPoint.Width);
								*lpDst = t;
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
						}//////end{while(!bFindPoint)}
					}//////end{while(!bFindStartPoint)}
				}//////end{if(flag[t]!=0)}
			}//////end{for(int t=1;t<=x_sign;t++)}
			memcpy(p_data, temp, wide * height);// 复制到原数据区
			delete[] temp; 	// 释放内存
			/////////////////////////////////////////////////
			int x_line=0;
			int fm[255]={0};//定义一个数组
			memset(fm,0,255);//初始化赋值都为0
			//统计每个边界的像素个数
			for(int j=0;j<height;j++)
			{
				for(int i=0;i<wide;i++)
				{
					if(*(p_data+(height-j-1)*wide+i)!=0)
					{
						x_line=*(p_data+(height-j-1)*wide+i);
						++fm[x_line];
					}
				}
			}	
			int fn[255]={0};//定义一个数组
			memset(fn,0,255);//初始化赋值都为0
			int y_line=0;
			int m_line=0;//定义一个总面积
			for(int i=0;i<x_line;i++)
			{
				if(fm[i]!=0)
				{
					if(fn[y_line]==0)
					{
						fn[y_line]=fm[i];
						++y_line;
					}			
				}
				m_line+=fm[i];
			}
			LINEDLG  dlg;//输出对话框
			dlg.m_shumu=y_line;//输出连通区域个数
			dlg.m_zongshu=m_line;//输出连通区域的总积
			CString ss[20];
			//在对话框里输出每个连通区的周长（边界像素个数）
			for(i=0;i<y_line;i++)
			{
				ss[i].Format("连通区：%3d  该区周长:%10.0d",i+1,fn[i]);
				dlg.m_line+=ss[i];
			}
			dlg.DoModal();
			/////////////////////////////////////////////////////////////////
			for(i=0;i<255;i++) //初试设置pp_lin都为0
			{
				pppp[i].pp_line=0;
			}
			for(t=1;t<=x_line;t++)
			{	
				for(int j=1;j<height-1;j++)
					for(int i=1;i<wide-1;i++)
					{
						if(*(p_temp+(height-j-1)*wide+i)==t)
						{
							pppp[t].pp_x=i;
							pppp[t].pp_y=j;
							pppp[t].pp_line=fm[t];
							break;
						}				
					}
			}
		}//end if(stop!=1)
	}
	else	//24位彩色
	{
		p_data=this->GetData();
		temp = new BYTE[wide*height*3]; 	// 暂时分配内存，以保存新图像
		lpDst = temp;	// 初始化新分配的内存，设定初始值为255
		memset(lpDst, (BYTE)255, wide * height*3);
		if(stop!=1)//判断连通区是否太多
		{
			//先找到最左上方的边界点
			for(int t=1;t<=x_sign;t++)
			{
				if(flag[t]!=0)
				{	
					bFindStartPoint = false;
					for (int j = 0;j < height && !bFindStartPoint;j++)
					{
						for(int i = 0;i < wide && !bFindStartPoint;i++)
						{
							// 指向源图像倒数第j行，第i个象素的指针			
							lpSrc =  (LPBYTE)(p_temp + wide * j + i);			
							//取得当前指针处的像素值，注意要转换为unsigned char型
							pixel =  *lpSrc;			
							if(pixel ==t)
							{
								bFindStartPoint = true;
								StartPoint.Height = j;
								StartPoint.Width = i;
								// 指向目标图像倒数第j行，第i个象素的指针			
								lpDst = (LPBYTE)(temp + wide * j*3 + i*3);	
								*lpDst =  t;
								*(lpDst+1) =  t;
								*(lpDst+2) =  t;
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
							lpSrc = (LPBYTE)(p_temp + wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
								+ (CurrentPoint.Width + Direction[BeginDirect][0]));
							pixel =  *lpSrc;
							if(pixel == t)
							{
								bFindPoint = true;
								CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
								CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
								if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
								{
									bFindStartPoint = true;
								}
								lpDst =  (LPBYTE)(temp + 3*wide * CurrentPoint.Height + CurrentPoint.Width*3);
								*lpDst = t;
								*(lpDst+1) = t;
								*(lpDst+2) = t;
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
							
						}//////end{while(!bFindPoint)}
					}//////end{while(!bFindStartPoint)}
				}//////end{if(flag[t]!=0)}
			}//////end{for(int t=1;t<=x_sign;t++)}
			memcpy(p_data, temp, wide * height*3);// 复制到原数据区
			delete[] temp; 	// 释放内存
			/////////////////////////////////////////////////
			int x_line=0;
			int fm[255]={0};//定义一个数组
			memset(fm,0,255);//初始化赋值都为0
			//统计每个边界的像素个数
			for(int j=0;j<height;j++)
				for(int i=0;i<wide;i++)
				{
					if(*(p_data+(height-j-1)*wide*3+i*3)!=0)
					{
						x_line=*(p_data+(height-j-1)*wide*3+i*3);
						++fm[x_line];
					}
				}
			int fn[255]={0};//定义一个数组
			memset(fn,0,255);//初始化赋值都为0
			int y_line=0;
			int m_line=0;//定义一个总面积
			for(int i=0;i<x_line;i++)
			{
				if(fm[i]!=0)
				{
					if(fn[y_line]==0)
					{
						fn[y_line]=fm[i];
						++y_line;
					}			
				}
				m_line+=fm[i];
			}
			LINEDLG  dlg;//输出对话框
			dlg.m_shumu=y_line;//输出连通区域个数
			dlg.m_zongshu=m_line;//输出连通区域的总积
			CString ss[20];
			//在对话框里输出每个连通区的周长（边界像素个数）
			for(i=0;i<y_line;i++)
			{
				ss[i].Format("连通区：%3d  该区周长:%10.0d",i+1,fn[i]);
				dlg.m_line+=ss[i];
			}
			dlg.DoModal();
			/////////////////////////////////////////////////////////////////
			for(i=0;i<255;i++) //初试设置pp_lin都为0
			{
				pppp[i].pp_line=0;
			}
			for(t=1;t<=x_line;t++)
			{	
				for(int j=1;j<height-1;j++)
					for(int i=1;i<wide-1;i++)
					{
						if(*(p_temp+(height-j-1)*wide+i)==t)
						{
							pppp[t].pp_x=i;
							pppp[t].pp_y=j;
							pppp[t].pp_line=fm[t];
							break;
						}				
					}
			}
		}//end if(stop!=1)
	}
}

//将24位彩色图像转换为24位灰度图
void JisuanProcessDib::MakeGray()
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
			BYTE* pbyRed = p_data++;    //得 到红色值
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			int gray=0;
			gray=(30*r+59*g+11*b)/100;		
			*pbyBlue = gray;     
			*pbyGreen = gray;    
			*pbyRed = gray;	    	
		}
}

//保存未处理的原图像
void JisuanProcessDib::Baoliu(LPBYTE temp)
{
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 找到DIB图像象素起始位置
	p_data= GetData();
	// DIB的宽度
	LONG wide = GetDibWidthBytes();
	// DIB的高度
	LONG height = GetHeight();	
	// 初始化新分配的内存，设定初始值为255
	lpDst = (LPBYTE)temp;
	memset(lpDst, (BYTE)255, wide * height);
	memcpy(lpDst,p_data,wide*height);
}
