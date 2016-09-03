//======================================================================
// 文件： SmoothProcessDib.cpp
// 内容： 图像平滑处理函数-源文件
// 功能： （1）邻域平均平滑函数；
//        （2）加权平均平滑函数；
//        （3）选择式掩膜平滑函数；
//        （4）中值滤波平滑函数       
// 
//======================================================================
#include "StdAfx.h"
#include "SmoothProcessDib.h"



//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CSmoothProcessDib::CSmoothProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CSmoothProcessDib::~CSmoothProcessDib(void)
{
	
}

//=======================================================
// 函数功能： 邻域平均平滑函数
// 输入参数： 无
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CSmoothProcessDib::Average_Smooth( )
{
		
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j;			//循环变量
	BYTE average;	//领域均值变量

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

	//逐个扫描图像中的像素点,求其邻域均值
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// 指向目标图像第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;

            //求当前点及其周围8个点的均值
			average=(BYTE)((float)(lpDIBBits[(j-1)*lWidth+(i-1)]+lpDIBBits[(j-1)*lWidth+i]+lpDIBBits[(j-1)*lWidth+(i+1)]
					+lpDIBBits[j*lWidth+(i-1)]+lpDIBBits[j*lWidth+i]+lpDIBBits[j*lWidth+i+1]
					+lpDIBBits[(j+1)*lWidth+(i-1)]+lpDIBBits[(j+1)*lWidth+i]+lpDIBBits[(j+1)*lWidth+i+1])/9+0.5);
					
				
				
			//将求得的均值赋值给目标图像中与当前点对应的像素点

			*lpDst = average;

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
// 函数功能： 加权平均平滑函数
// 输入参数： int Structure[3][3]-领域加权模板
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CSmoothProcessDib::Value_Average_Smooth(int Structure[3][3])
{
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	long i,j,m,n;			//循环变量
	int sum=0; //模板中各个元素总和
	BYTE value_average;	//领域加权均值变量

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

	//求模板中各元素权值总和
	for (m = 0;m < 3;m++ )
	{
		for (n = 0;n < 3;n++)
		{
			sum+=Structure[m][n];		
		}
	}			

	//逐个扫描图像中的像素点,求其邻域加权均值
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			// 指向目标图像第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;

            //求加权均值
			value_average=(BYTE)((float)(lpDIBBits[(j+1)*lWidth+(i-1)]*Structure[0][0]+lpDIBBits[(j+1)*lWidth+i]*Structure[0][1]
			                     +lpDIBBits[(j+1)*lWidth+i+1]*Structure[0][2]+lpDIBBits[j*lWidth+(i-1)]*Structure[1][0]
								 +lpDIBBits[j*lWidth+i]*Structure[1][1]+lpDIBBits[j*lWidth+i+1]*Structure[1][2]
					             +lpDIBBits[(j-1)*lWidth+(i-1)]*Structure[2][0]+lpDIBBits[(j-1)*lWidth+i]*Structure[2][1]
								 +lpDIBBits[(j-1)*lWidth+(i+1)]*Structure[2][2])/sum+0.5); 				
				
			//将求得的加权均值赋值给目标图像中与当前点对应的像素点
			*lpDst = value_average;

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
// 函数功能： 选择式掩膜平滑函数
// 输入参数： 无
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CSmoothProcessDib::Select_Smooth( )
{
    LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	int i,j,n;			//循环变量
   
	BYTE pixel[9];	//领域各点的像素值
    float mean[9],var[9],varMin; //邻域均值，邻域方差，方差最小值
    int nMin; //方差最小时的邻域号     

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

    //求9种邻域的均值与方差
    for(j=2;j<=lHeight-3;j++)
    {
	    for(i=2;i<=lWidth-3;i++)
		{
		    //第1邻域
			pixel[0]=lpDIBBits[(j-1)*lWidth+(i-1)];
			pixel[1]=lpDIBBits[(j-1)*lWidth+i];
			pixel[2]=lpDIBBits[(j-1)*lWidth+(i+1)];
			pixel[3]=lpDIBBits[j*lWidth+(i-1)];
			pixel[4]=lpDIBBits[j*lWidth+i];
			pixel[5]=lpDIBBits[j*lWidth+(i+1)];
			pixel[6]=lpDIBBits[(j+1)*lWidth+(i-1)];
			pixel[7]=lpDIBBits[(j+1)*lWidth+i];
			pixel[8]=lpDIBBits[(j+1)*lWidth+(i+1)];
			mean[0]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6]+pixel[7]+pixel[8])/9;
			var[0]=0;
			for(n=0;n<=8;n++)
				var[0]+=pixel[n]*pixel[n]-mean[0]*mean[0];
			//第2邻域
			pixel[0]=lpDIBBits[(j-2)*lWidth+(i-1)];
			pixel[1]=lpDIBBits[(j-2)*lWidth+i];
			pixel[2]=lpDIBBits[(j-2)*lWidth+(i+1)];
			pixel[3]=lpDIBBits[(j-1)*lWidth+(i-1)];
			pixel[4]=lpDIBBits[(j-1)*lWidth+i];
			pixel[5]=lpDIBBits[(j-1)*lWidth+(i+1)];
			pixel[6]=lpDIBBits[j*lWidth+i];
			mean[1]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
			var[1]=0;
			for(n=0;n<=6;n++)
				var[1]+=pixel[n]*pixel[n]-mean[1]*mean[1];
			//第3邻域
			pixel[0]=lpDIBBits[(j-1)*lWidth+(i-2)];
			pixel[1]=lpDIBBits[(j-1)*lWidth+(i-1)];
			pixel[2]=lpDIBBits[j*lWidth+(i-2)];
			pixel[3]=lpDIBBits[j*lWidth+(i-1)];
			pixel[4]=lpDIBBits[j*lWidth+i];
			pixel[5]=lpDIBBits[(j+1)*lWidth+(i-2)];
			pixel[6]=lpDIBBits[(j+1)*lWidth+(i-1)];
			mean[2]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
			var[2]=0;
			for(n=0;n<=6;n++)
				var[2]+=pixel[n]*pixel[n]-mean[2]*mean[2];
			//第4邻域
			pixel[0]=lpDIBBits[j*lWidth+i];
			pixel[1]=lpDIBBits[(j+1)*lWidth+(i-1)];
			pixel[2]=lpDIBBits[(j+1)*lWidth+i];
			pixel[3]=lpDIBBits[(j+1)*lWidth+(i+1)];
			pixel[4]=lpDIBBits[(j+2)*lWidth+(i-1)];
			pixel[5]=lpDIBBits[(j+2)*lWidth+i];
			pixel[6]=lpDIBBits[(j+2)*lWidth+(i+1)];
			mean[3]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[3]=0;
			for(n=0;n<=6;n++)
				var[3]+=pixel[n]*pixel[n]-mean[3]*mean[3];
			//第5邻域
			pixel[0]=lpDIBBits[(j-1)*lWidth+(i+1)];
			pixel[1]=lpDIBBits[(j-1)*lWidth+(i+2)];
			pixel[2]=lpDIBBits[j*lWidth+i];
			pixel[3]=lpDIBBits[j*lWidth+(i+1)];
			pixel[4]=lpDIBBits[j*lWidth+(i+2)];
			pixel[5]=lpDIBBits[(j+1)*lWidth+(i+1)];
			pixel[6]=lpDIBBits[(j+1)*lWidth+(i+2)]; 
			mean[4]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
			var[4]=0;
			for(n=0;n<=6;n++)
				var[4]+=pixel[n]*pixel[n]-mean[4]*mean[4];	
			//第6邻域
			pixel[0]=lpDIBBits[(j-2)*lWidth+(i+1)];
			pixel[1]=lpDIBBits[(j-2)*lWidth+(i+2)];
			pixel[2]=lpDIBBits[(j-1)*lWidth+i];
			pixel[3]=lpDIBBits[(j-1)*lWidth+(i+1)];
			pixel[4]=lpDIBBits[(j-1)*lWidth+(i+2)];
			pixel[5]=lpDIBBits[j*lWidth+i];
			pixel[6]=lpDIBBits[j*lWidth+(i+1)]; 
			mean[5]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[5]=0;
			for(n=0;n<=6;n++)
				var[5]+=pixel[n]*pixel[n]-mean[5]*mean[5];
			//第7邻域
			pixel[0]=lpDIBBits[(j-2)*lWidth+(i-2)];
			pixel[1]=lpDIBBits[(j-2)*lWidth+(i-1)];
			pixel[2]=lpDIBBits[(j-1)*lWidth+(i-2)];
			pixel[3]=lpDIBBits[(j-1)*lWidth+(i-1)];
			pixel[4]=lpDIBBits[(j-1)*lWidth+i];
			pixel[5]=lpDIBBits[j*lWidth+(i-1)];
			pixel[6]=lpDIBBits[j*lWidth+i];
			mean[6]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	 		var[6]=0;
			for(n=0;n<=6;n++)
				var[6]+=pixel[n]*pixel[n]-mean[6]*mean[6];
			//第8邻域
			pixel[0]=lpDIBBits[j*lWidth+(i-1)];
			pixel[1]=lpDIBBits[j*lWidth+i];
			pixel[2]=lpDIBBits[(j+1)*lWidth+(i-2)];
			pixel[3]=lpDIBBits[(j+1)*lWidth+(i-1)];
			pixel[4]=lpDIBBits[(j+1)*lWidth+i];
			pixel[5]=lpDIBBits[(j+2)*lWidth+(i-2)];
			pixel[6]=lpDIBBits[(j+2)*lWidth+(i-1)];
			mean[7]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[7]=0;
			for(n=0;n<=6;n++)
				var[7]+=pixel[n]*pixel[n]-mean[7]*mean[7];
			//第9邻域
			pixel[0]=lpDIBBits[j*lWidth+i];
			pixel[1]=lpDIBBits[j*lWidth+(i+1)];
			pixel[2]=lpDIBBits[(j+1)*lWidth+i];
			pixel[3]=lpDIBBits[(j+1)*lWidth+(i+1)];
			pixel[4]=lpDIBBits[(j+1)*lWidth+(i+2)];
			pixel[5]=lpDIBBits[(j+2)*lWidth+(i+1)];
			pixel[6]=lpDIBBits[(j+2)*lWidth+(i+2)];
			mean[8]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[8]=0;
			for(n=0;n<=6;n++)
				var[8]+=pixel[n]*pixel[n]-mean[8]*mean[8];

			//求方差最小的邻域nMin
			varMin=var[0];
			nMin=0;
			for(n=0;n<=8;n++)
			{
				if(varMin>var[n])
				{
					varMin=var[n];
					nMin=n;
				}
				
			}

            // 指向目标图像第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
            //将方差最小的邻域均值赋值给目标像素点
            *lpDst = (BYTE)(mean[nMin]+0.5);  

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
// 函数功能： 中值滤波平滑函数
// 输入参数： 无
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CSmoothProcessDib::Middle_Smooth( )
{
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;	
	int i,j,x,y,m;			//循环变量
	int flag=1;  //循环标志变量
	BYTE pixel[9],mid; //窗口像素值及中值
	BYTE temp;//中间变量
	
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

	//中值滤波
	for(j=1;j<lHeight-1;j++)
	{
		for(i=1;i<lWidth-1;i++)
		{
			//把3*3屏蔽窗口的所有像素值放入pixel[m]
			m=0;
			for(y=j-1;y<=j+1;y++)
				for(x=i-1;x<=i+1;x++)
				{
					pixel[m]=lpDIBBits[y*lWidth+x];
					m++;
				}
			//把pixel[m]中的值按降序排序
			do{
				flag=0;
				for(m=0;m<9;m++)
				{
					if(pixel[m]<pixel[m+1])
					{
						temp=pixel[m];
						pixel[m]=pixel[m+1];
						pixel[m+1]=temp;
						flag=1;
					}
				}

			}while(flag==1);

			//求中值mid				
			mid=pixel[4];

			// 指向目标图像第j行，第i个像素的指针			
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
            //将中值赋给目标图像的当前点
            *lpDst = mid;

		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
     //释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

    return TRUE;
}