#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BianYuanJianCeDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
BianYuanJianCeDib::BianYuanJianCeDib()
{
  
}
BianYuanJianCeDib::~BianYuanJianCeDib()
{
}

/***************************************************************/
/*函数名称：Templat(BYTE *m_pdata, int wide, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)                                         */
/*函数类型：void                                               */
/*参数：BYTE* m_pdata:指向原DIB图像指针                        */
/*      int wide:原图像宽度                                    */
/*      int height:原图像高度                                  */
/*      int tempH:模板高度                                     */
/*      int tempW:模板宽度                                     */
/*      int tempMX:模板的中心元素X坐标(<tempW-1)               */
/*      int tempMY:模板的中心元素Y坐标(<tempH-1)               */
/*      float *fpArray:指向模板数组的指针                      */
/*      float fCoef：模板系数                                  */
/*返回值：处理成功返回TRUE;处理失败返回FALSE。                 */
/*功能：用指定模板对灰度图像进行操作。                         */
/***************************************************************/
void BianYuanJianCeDib::Templat(BYTE *m_pdata, int wide, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{   
	int i,j,k,l;  //循环变量
    BYTE*  temp=new BYTE[wide*height];    //新图像缓冲区
	//初始化新图像为原始图像
    memcpy( temp,m_pdata,wide*height);
    float fResult;    //像素值计算结果
	for(j=tempMY;j<height-tempH+tempMY+1;j++)
		for(i=tempMX;i<wide-tempW+tempMX+1;i++)
		{
			//计算像素值
			fResult=0;
			for(k=0;k<tempH;k++)
				for(l=0;l<tempW;l++)
					fResult=fResult+m_pdata[(j-tempMY+k)*wide+(i-tempMX+l)]*fpArray[k*tempW+l];
			//乘上系数
			fResult*=fCoef;
			//取绝对值
			fResult=(float)fabs(fResult);
			//判断是否超过255
			if(fResult>255)
				//若超过255，直接赋值为255
                temp[j*wide+i]=255;
			else
				//未超过255，赋值为计算结果
				temp[j*wide+i]=(int)(fResult+0.5);
		}
    memcpy(m_pdata, temp,wide*height);   //复制处理后的图像
	delete  temp;
}

/***************************************************************/
/*函数名称：Templat24bit(BYTE *m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)                                         */
/*函数类型：void                                               */
/*参数：BYTE* m_pdata:指向原DIB图像指针                        */
/*      int DibWidth:原图像宽度                                */
/*      int height:原图像高度                                  */
/*      int tempH:模板高度                                     */
/*      int tempW:模板宽度                                     */
/*      int tempMX:模板的中心元素X坐标(<tempW-1)               */
/*      int tempMY:模板的中心元素Y坐标(<tempH-1)               */
/*      float *fpArray:指向模板数组的指针                      */
/*      float fCoef：模板系数                                  */
/*返回值：处理成功返回TRUE;处理失败返回FALSE。                 */
/*功能：用指定模板对24位彩色图像进行操作。                     */
/***************************************************************/
void BianYuanJianCeDib::Templat24bit(BYTE *m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{
    int i,j,k,l;  //循环变量
    BYTE*  p_temp=new BYTE[DibWidth*height];    //新图像缓冲区
	//初始化新图像为原始图像
    memcpy( p_temp,m_pdata,DibWidth*height);
    float fResult;    //像素值计算结果
	for(j=tempMY;j<height-tempH+tempMY+1;j++)
		for(i=3*tempMX;i<DibWidth-3*tempW+3*tempMX+1;i++)
		{
			//计算像素值
			fResult=0;
			for(k=0;k<tempH;k++)
				for(l=0;l<tempW;l++)
					fResult=fResult+m_pdata[(j-tempMY+k)*DibWidth+(i-3*tempMX+l*3)]*fpArray[k*tempW+l];
			//乘上系数
			fResult*=fCoef;
			//取绝对值
			fResult=(float)fabs(fResult);
			//判断是否超过255
			if(fResult>255)
				//若超过255，直接赋值为255
                p_temp[j*DibWidth+i]=255;
			else
				//未超过255，赋值为计算结果
				p_temp[j*DibWidth+i]=(int)(fResult+0.5);
		}
    memcpy(m_pdata, p_temp,DibWidth*height);   //复制处理后的图像
	delete  []p_temp;
}

///***************************************************************/           
/*函数名称：Robert()                                      
/*函数类型：void                                     
/*功能：用罗伯特算子对图像进行边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Robert()
{    
	LPBYTE  p_data;     //原图数据区指针
	int wide,height,DibWidth;    //原图长、宽
	p_data=this->GetData ();   //取得原图的数据区指针
    wide=this->GetWidth ();  //取得原图的数据区宽度
    height=this->GetHeight ();   //取得原图的数据区高度
	DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
	int i,j;  //循环变量
    if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{
	    int pixel[4];   //Robert算子
		LPBYTE   temp=new BYTE[wide*height];    //新图像缓冲区
 	    memset(temp,255,wide*height);  //设定新图像初值为255
	    //由于使用2*2的模板，为防止越界，所以不处理最下边和最右边的两列像素
	    for(j=0;j<height-1;j++)
		    for(i=0;i<wide-1;i++)
			{
		        //生成Robert算子
				pixel[0]=p_data[j*wide+i];
                pixel[1]=p_data[j*wide+i+1];
				pixel[2]=p_data[(j+1)*wide+i];
				pixel[3]=p_data[(j+1)*wide+i+1];
				//处理当前像素
				 temp[j*wide+i]=(int)sqrt((pixel[0]-pixel[3])*(pixel[0]-pixel[3])+(pixel[1]-pixel[2])*(pixel[1]-pixel[2]));
			}
		//将缓冲区中的数据复制到原图数据区
		memcpy(p_data, temp,wide*height);
		//删除缓冲区
		delete  temp;
	}
  else		//24位彩色
  {
	 int pixel[4];   //Robert算子
     BYTE *p_temp=new BYTE[height*DibWidth];

	 for(j=0;j<height-1;j++)
		for(i=0;i<DibWidth-3;i++)
		{
			//生成Robert算子
			pixel[0]=p_data[j*DibWidth+i];
            pixel[1]=p_data[j*DibWidth+i+3];
			pixel[2]=p_data[(j+1)*DibWidth+i];
			pixel[3]=p_data[(j+1)*DibWidth+i+3];
			//处理当前像素
			p_temp[j*DibWidth+i]=(int)sqrt((pixel[0]-pixel[3])*(pixel[0]-pixel[3])+(pixel[1]-pixel[2])*(pixel[1]-pixel[2]));
		}
     memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
     delete []p_temp;  //删除暂时分配内存  
  }
}
 
///***************************************************************/           
/*函数名称：PreWitt()                                      
/*函数类型：void                                     
/*功能：用普瑞维特算子对图像进行边缘检测。            
/***************************************************************/
void BianYuanJianCeDib::PreWitt()
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组	
	LPBYTE  p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{	
		LPBYTE   temp1=new BYTE[wide*height];    //新图像缓冲区
        LPBYTE   temp2=new BYTE[wide*height];    //新图像缓冲区
        //拷贝原图像到缓存图像
		memcpy( temp1,p_data,wide*height);
        memcpy( temp2,p_data,wide*height);
	 	//设置Prewitt模板1参数
		tempW=3;
		tempH=3;
		tempC=1.0;
		tempMY=1;
		tempMX=1;
		Template[0]=-1.0;
        Template[1]=-1.0;
		Template[2]=-1.0;
		Template[3]=0.0;
		Template[4]=0.0;
		Template[5]=0.0;
		Template[6]=1.0;
		Template[7]=1.0;
		Template[8]=1.0;
		//调用Templat()函数
		Templat( temp1,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //设置Prewitt模板2参数
        Template[0]=1.0;
        Template[1]=0.0;
		Template[2]=-1.0;
		Template[3]=1.0;
		Template[4]=0.0;
		Template[5]=-1.0;
		Template[6]=1.0;
		Template[7]=0.0;
		Template[8]=-1.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
		//求两幅缓存图像的最大值
		for(int j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
        memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else	//24位彩色
	{	    
	    int DibWidth;    //原图长、宽    	
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		//将缓存中的图像复制到原图数据区
		memcpy(p_temp1,p_data,DibWidth*height);
		memcpy(p_temp2,p_data,DibWidth*height);
		//设置Prewitt模板1参数
		tempW=3;
		tempH=3;
		tempC=1.0;
		tempMY=1;
		tempMX=1;
		Template[0]=-1.0;
        Template[1]=-1.0;
		Template[2]=-1.0;
		Template[3]=0.0;
		Template[4]=0.0;
		Template[5]=0.0;
		Template[6]=1.0;
		Template[7]=1.0;
		Template[8]=1.0;
		Templat24bit( p_temp1,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //设置Prewitt模板2参数
        Template[0]=1.0;
        Template[1]=0.0;
		Template[2]=-1.0;
		Template[3]=1.0;
		Template[4]=0.0;
		Template[5]=-1.0;
		Template[6]=1.0;
		Template[7]=0.0;
		Template[8]=-1.0;
		Templat24bit( p_temp2,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
		//求两幅缓存图像的最大值
		for(int j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
	   memcpy(p_data,p_temp1,height*DibWidth);  // 复制处理后的图像
	   delete []p_temp1;  //删除暂时分配内存
	   delete []p_temp2;  //删除暂时分配内存
	}	
}

///***************************************************************/           
/*函数名称：Sobel()                                      
/*函数类型：void                                     
/*功能：用索伯尔算子对图像进行边缘检测。            
/***************************************************************/
void BianYuanJianCeDib::Sobel()
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组
	LPBYTE  p_data;     //原图数据区指针
    int wide,height;    //原图长、宽
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{        
		LPBYTE   temp1=new BYTE[wide*height];    //新图像缓冲区
        LPBYTE   temp2=new BYTE[wide*height];    //新图像缓冲区
        //拷贝原图像到缓存图像
		memcpy( temp1,p_data,wide*height);
        memcpy( temp2,p_data,wide*height);
		//设置Sobel模板参数
		tempW=3;
		tempH=3;
		tempC=1.0;
		tempMY=1;
		tempMX=1;
		Template[0]=-1.0;
        Template[1]=-2.0;
		Template[2]=-1.0;
		Template[3]=0.0;
		Template[4]=0.0;
		Template[5]=0.0;
		Template[6]=1.0;
		Template[7]=2.0;
		Template[8]=1.0;
		//调用Templat()函数
		Templat( temp1,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC); 
        //设置Sobel模板参数
        Template[0]=-1.0;
        Template[1]=0.0;
		Template[2]=1.0;
		Template[3]=-2.0;
		Template[4]=0.0;
		Template[5]=2.0;
		Template[6]=-1.0;
		Template[7]=0.0;
		Template[8]=1.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC); 
		//求两幅缓存图像的最大值
		for(int j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
        //将缓存中的图像复制到原图数据区
		memcpy(p_data, temp1,wide*height);
		//删除缓冲区
		delete  temp1;
		delete  temp2;
	}
	else	//24位彩色
	{	    
	    int DibWidth;    //原图长、宽    	
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		//将缓存中的图像复制到原图数据区
		memcpy(p_temp1,p_data,DibWidth*height);
		memcpy(p_temp2,p_data,DibWidth*height);
		//设置Sobel模板参数
		tempW=3;
		tempH=3;
		tempC=1.0;
		tempMY=1;
		tempMX=1;
		Template[0]=-1.0;
        Template[1]=-2.0;
		Template[2]=-1.0;
		Template[3]=0.0;
		Template[4]=0.0;
		Template[5]=0.0;
		Template[6]=1.0;
		Template[7]=2.0;
		Template[8]=1.0;
		Templat24bit( p_temp1,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC); 
        //设置Sobel模板参数
        Template[0]=-1.0;
        Template[1]=0.0;
		Template[2]=1.0;
		Template[3]=-2.0;
		Template[4]=0.0;
		Template[5]=2.0;
		Template[6]=-1.0;
		Template[7]=0.0;
		Template[8]=1.0;
		Templat24bit( p_temp2,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC); 
		//求两幅缓存图像的最大值
		for(int j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
	   memcpy(p_data,p_temp1,height*DibWidth);  // 复制处理后的图像
	   delete []p_temp1;  //删除暂时分配内存
	   delete []p_temp2;  //删除暂时分配内存
	}
}

///***************************************************************/           
/*函数名称：Laplacian()                                      
/*函数类型：void                                     
/*功能：用拉普拉斯算子对图像边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Laplacian()
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组
	LPBYTE p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{    
		LPBYTE   temp1=new BYTE[wide*height];    //新图像缓冲区
        //拷贝原图像到缓存图像
		memcpy( temp1,p_data,wide*height);
		//设置Laplacian模板 参数
		tempW=3;
		tempH=3;
		tempC=1.0;
		tempMY=1;
		tempMX=1;
		Template[0]=-1.0;
        Template[1]=-1.0;
		Template[2]=-1.0;
		Template[3]=-1.0;
		Template[4]=8.0;
		Template[5]=-1.0;
		Template[6]=-1.0;
		Template[7]=-1.0;
		Template[8]=-1.0;
		//调用Templat()函数
		Templat( temp1,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
	}
	else	//24位彩色
	{    
	    int DibWidth;    //原图长、宽    	
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp1=new BYTE[height*DibWidth];
		//将缓存中的图像复制到原图数据区
		memcpy(p_temp1,p_data,DibWidth*height);
		//设置Laplacian模板 参数
		tempW=3;
		tempH=3;
		tempC=1.0;
		tempMY=1;
		tempMX=1;
		Template[0]=-1.0;
        Template[1]=-1.0;
		Template[2]=-1.0;
		Template[3]=-1.0;
		Template[4]=8.0;
		Template[5]=-1.0;
		Template[6]=-1.0;
		Template[7]=-1.0;
		Template[8]=-1.0;
		Templat24bit( p_temp1,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
		memcpy(p_data,p_temp1,height*DibWidth);  // 复制处理后的图像
	    delete []p_temp1;  //删除暂时分配内存
	}
}

///***************************************************************/           
/*函数名称：Guasslaplacian()                                      
/*函数类型：void                                    
/*功能：用高斯拉普拉斯算子对图像边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Guasslaplacian()
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[25];   //模板数组
	LPBYTE  p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{        
		LPBYTE   temp1=new BYTE[wide*height];    //新图像缓冲区
        LPBYTE   temp2=new BYTE[wide*height];    //新图像缓冲区
        //拷贝原图像到缓存图像
		memcpy( temp1,p_data,wide*height);
        memcpy( temp2,p_data,wide*height);
		//设置Guasslaplacian模板参数
		tempW=5;
		tempH=5;
		tempC=0.25;
		tempMY=4;
		tempMX=4;
		Template[0]=-2.0;
        Template[1]=-4.0;
		Template[2]=-4.0;
		Template[3]=-4.0;
		Template[4]=-2.0;
		Template[5]=-4.0;
		Template[6]=0.0;
		Template[7]=8.0;
		Template[8]=0.0;
		Template[9]=-4.0;
		Template[10]=-4.0;
		Template[11]=8.0;
		Template[12]=24.0;
		Template[13]=8.0;
		Template[14]=-4.0;
		Template[15]=-4.0;
		Template[16]=0.0;
		Template[17]=8.0;
		Template[18]=0.0;
		Template[19]=-4.0;
		Template[20]=-2.0;
		Template[21]=-4.0;
		Template[22]=-4.0;
		Template[23]=-4.0;
		Template[24]=-2.0;
		//调用Templat()函数
		Templat( temp1,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;         //释放内存
		delete  temp2;
	}
	else	//24位彩色
	{
	    int DibWidth;
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp1=new BYTE[height*DibWidth];
		//将缓存中的图像复制到原图数据区
		memcpy(p_temp1,p_data,DibWidth*height);
		//设置Guasslaplacian模板参数
		tempW=5;
		tempH=5;
		tempC=0.25;
		tempMY=4;
		tempMX=4;
		Template[0]=-2.0;
        Template[1]=-4.0;
		Template[2]=-4.0;
		Template[3]=-4.0;
		Template[4]=-2.0;
		Template[5]=-4.0;
		Template[6]=0.0;
		Template[7]=8.0;
		Template[8]=0.0;
		Template[9]=-4.0;
		Template[10]=-4.0;
		Template[11]=8.0;
		Template[12]=24.0;
		Template[13]=8.0;
		Template[14]=-4.0;
		Template[15]=-4.0;
		Template[16]=0.0;
		Template[17]=8.0;
		Template[18]=0.0;
		Template[19]=-4.0;
		Template[20]=-2.0;
		Template[21]=-4.0;
		Template[22]=-4.0;
		Template[23]=-4.0;
		Template[24]=-2.0;
		//调用Templat24bit()函数
		Templat24bit( p_temp1,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        memcpy(p_data,p_temp1,height*DibWidth);  // 复制处理后的图像
	    delete []p_temp1;  //删除暂时分配内存
	}
}

///***************************************************************/           
/*函数名称：Krisch()                                      
/*函数类型：void                                     
/*功能：用克瑞斯算子对图像边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Krisch()
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组
	LPBYTE  p_data;     //原图数据区指针
	int wide,height;    //原图长、宽 	 
	p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
	if (m_pBitmapInfoHeader->biBitCount<9)		//灰度图像
	{    
		LPBYTE   temp1=new BYTE[wide*height];    //新图像缓冲区
        LPBYTE   temp2=new BYTE[wide*height];    //新图像缓冲区
        //拷贝原图像到缓存图像
		memcpy( temp1,p_data,wide*height);
        memcpy( temp2,p_data,wide*height);
		//设置Kirsch模板1参数
		tempW=3;
		tempH=3;
		tempC=0.5;
		tempMY=1;
		tempMX=1;
		Template[0]=5.0;
        Template[1]=5.0;
		Template[2]=5.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
        //调用Templat()函数
		Templat( temp1,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //设置Kirsch模板2参数
        Template[0]=-3.0;
        Template[1]=5.0;
		Template[2]=5.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=5.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(int j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
		//拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
        //设置Kirsch模板3参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=5.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=5.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=5.0;
		//调用Templat()函数
	    Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
        //拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
        //设置Kirsch模板4参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=5.0;
		Template[6]=-3.0;
		Template[7]=5.0;
		Template[8]=5.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
		//拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
		//设置Kirsch模板5参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=5.0;
		Template[7]=5.0;
		Template[8]=5.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
		//拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
		//拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
		//设置Kirsch模板6参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=5.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=5.0;
		Template[7]=5.0;
		Template[8]=-3.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
		//拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
		//设置Kirsch模板7参数
        Template[0]=5.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=5.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=5.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
		//调用Templat()函数
		Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
		//拷贝原图像到缓存图像2中
        memcpy( temp2,p_data,wide*height);
		//设置Kirsch模板8参数
        Template[0]=5.0;
        Template[1]=5.0;
		Template[2]=-3.0;
		Template[3]=5.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
		//调用Templat()函数
	    Templat( temp2,wide,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<wide;i++)
				if( temp2[j*wide+i]> temp1[j*wide+i])
					 temp1[j*wide+i]= temp2[j*wide+i];
		memcpy(p_data, temp1,wide*height);   //复制处理后的图像
		delete  temp1;
		delete  temp2;
	}
	else	//24位彩色
	{	    
	    int DibWidth;    //原图长、宽    	
	    DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp1=new BYTE[height*DibWidth];
		BYTE *p_temp2=new BYTE[height*DibWidth];
		 //将缓存中的图像复制到原图数据区
		memcpy(p_temp1,p_data,DibWidth*height);
		memcpy(p_temp2,p_data,DibWidth*height);
		//设置Kirsch模板1参数
		tempW=3;
		tempH=3;
		tempC=0.5;
		tempMY=1;
		tempMX=1;
		Template[0]=5.0;
        Template[1]=5.0;
		Template[2]=5.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
		Templat24bit(p_temp1,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //设置Kirsch模板2参数
        Template[0]=-3.0;
        Template[1]=5.0;
		Template[2]=5.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=5.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
		Templat24bit(p_temp2,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(int j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
		//拷贝原图像到缓存图像2中
        memcpy(p_temp2,p_data,DibWidth*height);
        //设置Kirsch模板3参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=5.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=5.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=5.0;
		Templat24bit(p_temp2,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
		//拷贝原图像到缓存图像2中
        memcpy(p_temp2,p_data,DibWidth*height);
        //设置Kirsch模板4参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=5.0;
		Template[6]=-3.0;
		Template[7]=5.0;
		Template[8]=5.0;
		Templat24bit(p_temp2,DibWidth,height,tempH,tempW,tempMX,tempMY,Template,tempC);
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
		//拷贝原图像到缓存图像2中
        memcpy(p_temp2,p_data,DibWidth*height);
		//设置Kirsch模板5参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=-3.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=5.0;
		Template[7]=5.0;
		Template[8]=5.0;
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
		//拷贝原图像到缓存图像2中
        memcpy(p_temp2,p_data,DibWidth*height);
		//设置Kirsch模板6参数
        Template[0]=-3.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=5.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=5.0;
		Template[7]=5.0;
		Template[8]=-3.0;
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
		//拷贝原图像到缓存图像2中
        memcpy(p_temp2,p_data,DibWidth*height);
		//设置Kirsch模板7参数
        Template[0]=5.0;
        Template[1]=-3.0;
		Template[2]=-3.0;
		Template[3]=5.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=5.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
       //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
		//拷贝原图像到缓存图像2中
        memcpy(p_temp2,p_data,DibWidth*height);
		//设置Kirsch模板8参数
        Template[0]=5.0;
        Template[1]=5.0;
		Template[2]=-3.0;
		Template[3]=5.0;
		Template[4]=0.0;
		Template[5]=-3.0;
		Template[6]=-3.0;
		Template[7]=-3.0;
		Template[8]=-3.0;
        //求两幅缓存图像的最大值
		for(j=0;j<height;j++)
			for(int i=0;i<DibWidth;i++)
				if( p_temp2[j*DibWidth+i]> p_temp1[j*DibWidth+i])
					 p_temp1[j*DibWidth+i]= p_temp2[j*DibWidth+i];
	    memcpy(p_data,p_temp1,height*DibWidth);  // 复制处理后的图像
	    delete []p_temp1;  //删除暂时分配内存
	    delete []p_temp2;  //删除暂时分配内存
	}
}