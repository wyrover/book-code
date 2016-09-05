//======================================================================
// 文件： SharpenProcessDib.cpp
// 内容： 图像锐化处理函数-源文件
// 功能： （1）门限梯度锐化函数；
//        （2）卷积运算
//        （3）拉普拉斯掩膜锐化函数       
// 
//======================================================================

#include "StdAfx.h"
#include "SharpenProcessDib.h"
#include<math.h>


//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CSharpenProcessDib::CSharpenProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CSharpenProcessDib::~CSharpenProcessDib(void)
{
	
}


//=======================================================
// 函数功能： 门限梯度锐化处理函数
// 输入参数： BYTE t -门限值
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CSharpenProcessDib::GateGrad(BYTE t)
{
	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	
    BYTE temp; //暂存双向一次微分结果

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
	memset(lpNewDIBBits, (BYTE)0, lWidth * lHeight);

	//逐个扫描图像中的像素点,进行门限梯度锐化处理
	
	for(j=1;j<lHeight-1;j++)
    {
		for(i=1;i<lWidth-1;i++)
		{
			//根据双向一次微分公式计算当前像素的灰度值
			temp=(BYTE)sqrt((float)((lpDIBBits[lWidth*j+i]-lpDIBBits[lWidth*j+(i-1)])*(lpDIBBits[lWidth*j+i]-lpDIBBits[lWidth*j+(i-1)])
                +(lpDIBBits[lWidth*j+i]-lpDIBBits[lWidth*(j-1)+i])*(lpDIBBits[lWidth*j+i]-lpDIBBits[lWidth*(j-1)+i])));
			if (temp>=t)
			{   
				if((temp+100)>255)
					lpNewDIBBits[lWidth*j+i]=255;
				else 
					lpNewDIBBits[lWidth*j+i]=temp+100;
			}
			if (temp<t)
				lpNewDIBBits[lWidth*j+i]=lpDIBBits[lWidth*j+i];
		}
    }
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;  
}


//=======================================================
// 函数功能： 卷积运算函数
// 输入参数： int tempH:模板高度        
//            int tempW:模板宽度                                   
//            int tempMX:模板的中心元素X坐标               
//            int tempMY:模板的中心元素Y坐标             
//            float *fpTempArray:指向模板数组的指针                      
//            float fCoef：模板系数                                 
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================

BOOL CSharpenProcessDib::Convolution(int tempH, int tempW, int tempMX, int tempMY, float *fpTempArray, float fCoef)
{
    	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j,k,l;			//循环变量
	float fResult;    //暂存计算中间结果     

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
	memset(lpNewDIBBits, (BYTE)0, lWidth * lHeight);

	//逐个扫描图像中的像素点,进行卷积运算
    
	for(j=tempMY;j<lHeight-tempH+tempMY+1;j++)
    {
		for(i=tempMX;i<lWidth-tempW+tempMX+1;i++)
		{
			//计算像素值
			fResult=0;
			for(k=0;k<tempH;k++)
				for(l=0;l<tempW;l++)
					fResult=fResult+lpDIBBits[(j-tempMY+k)*lWidth+(i-tempMX+l)]*fpTempArray[k*tempW+l];
			//乘上系数
			fResult*=fCoef;
			//取绝对值
			fResult=(float)fabs(fResult);
			//判断是否超过255
			if(fResult>255)
				//若超过255，直接赋值为255
                lpNewDIBBits[j*lWidth+i]=255;
			else
				//未超过255，赋值为计算结果
				lpNewDIBBits[j*lWidth+i]=(BYTE)(fResult+0.5);
		}
    }

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
    //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;  

}

//=======================================================
// 函数功能： 拉普拉斯锐化处理函数
// 输入参数： 无
// 返回值：   无
//=======================================================
void CSharpenProcessDib::Laplacian( )
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组	

	//设置拉普拉斯模板参数
	tempW=3;
	tempH=3;
	tempC=1.0;
	tempMY=1;
	tempMX=1;
	Template[0]=0.0;
    Template[1]=1.0;
	Template[2]=0.0;
	Template[3]=1.0;
	Template[4]=-4.0;
	Template[5]=1.0;
	Template[6]=0.0;
	Template[7]=1.0;
	Template[8]=0.0;

	//调用卷积函数
	Convolution(tempH,tempW,tempMX,tempMY,Template,tempC); 

}