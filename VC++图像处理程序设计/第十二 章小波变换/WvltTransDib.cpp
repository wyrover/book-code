// WvltTrans.cpp: implementation of the CWvltTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "windowsx.h"
#include "math.h"

#include "MainFrm.h"
#include "DynSplitView2.h"
#include "WvltTransDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWvltTransDib::CWvltTransDib()
{

}

CWvltTransDib::~CWvltTransDib()
{

}

/********************************************************************************
*函数描述：	Hangbianhuan实现小波行变换
*函数参数：	无 
*函数返回值：函数无返回值						  
*********************************************************************************/
void CWvltTransDib::Hangbianhuan()
{
	int i,j;
	LONG wide,height;
	LPBYTE temp1,m_pData2;

	wide=this->GetWidth();
	height=this->GetHeight();
	m_pData2=this->GetData2();

	int nWide=wide/2;

	//分配临时数据空间
	temp1 = new BYTE[height*wide];

	//从设备缓存中获得原始图像数据
    for(j = 0; j < height; j ++)            
	{
		for(i = 0; i < nWide; i ++)           
		{
			int w = i *2;
			temp1[j*wide+i] = m_pData2[j*wide+w];		//偶
			temp1[j*wide+nWide+i] = m_pData2[j*wide+w+1];	//奇
		}
	}
	//通过图像的差分，完成小波变换
	for(j=0; j<height; j++)
	{
		for(i=0; i<nWide-1; i++)
		{
			temp1[j*wide+nWide-1+i] =temp1[j*wide+(nWide-1)+i] - temp1[j*wide+i]+128;	
		}
	}

	//小波经过处理后，放入显示缓存中
    for(j=0; j<(int)height; j++)
	{
		for(i=0; i<(int)wide; i++)
		{
			m_pData2[j*wide+i]  = temp1[j*wide+i];
		}
	}
	//删除临时的数据空间
	delete temp1;
}

/********************************************************************************
*函数描述：	Liebianhuan实现小波列变换
*函数参数：	无 
*函数返回值：函数无返回值						  
*********************************************************************************/
void CWvltTransDib::Liebianhuan()
{
	int i,j;
	LONG wide,height;
	LPBYTE  temp,m_pData2;
	wide=this->GetWidth();
	height=this->GetHeight();
	m_pData2=this->GetData2();
	int nHeight=height/2;
	//分配临时数据空间
	temp = new BYTE[height*wide];
	for(i = 0; i < wide; i ++)            
	{
		for(j = 0; j < nHeight; j ++)           
		{
			int h = j *2 ;
			temp[j*wide+i] = m_pData2[h*wide+i];		//even
			temp[(nHeight + j)*wide+i] = m_pData2[(h+1)*wide+i];	//odd
		}
	}
	//通过图像的差分，完成小波变换
	for(i=0; i<wide; i++)
	{
		for(j=0; j<nHeight-1; j++)
		{
			temp[ j*wide+i]=temp[(nHeight + j)*wide+i]- temp[j*wide+i]+128;
		}
	}
    for(j=0; j<(int)height; j++)
	{
		for(i=0; i<(int)wide; i++)
		{
			m_pData2[j*wide+i]  = temp[j*wide+i];
		}
	}
	//删除临时的数据空间
	delete temp;	
}


/********************************************************************************
*函数描述：	Once实现小波变换
*函数参数：	int n 小波变换的层数 
*函数返回值：函数无返回值						  
*********************************************************************************/
void CWvltTransDib::Once(int n)
{
	int i,j;
	LONG wide,height,nWide,nHeight;
	LPBYTE temp1, temp2,m_pData2;
	for(int k=1;k<=n;k++)
    {
		wide=this->GetWidth();
		height=this->GetHeight();
		m_pData2=this->GetData2();
		//分配临时数据空间
		temp1 = new BYTE[height*wide];
		temp2 = new BYTE[height*wide];
		nWide=wide/pow(2,k);
		nHeight=height/pow(2,k);
		//完成行变换
		for(j = height*(1-1/pow(2,(k-1))); j < height; j ++)            
		{
			for(i = 0; i < nWide; i ++)           
			{
				int w = i *2;
				temp1[j*wide+i] = m_pData2[j*wide+w];		//偶
				temp1[j*wide+nWide+i] = m_pData2[j*wide+w+1];	//奇
			}
		}
		//通过图像的差分，完成小波变换
		for(j= height*(1-1/pow(2,(k-1))); j<height; j++)
		{
			for(i=0; i<nWide-1; i++)
			{
				temp1[j*wide+nWide-1+i] =temp1[j*wide+nWide-1+i] - temp1[j*wide+i]+128;	
			}
		}
		//完成列变换
		for(i = 0; i < wide/pow(2,k-1); i ++)            
		{
			for(j =  height*(1-1/pow(2,(k-1))); j <  height*(1-1/pow(2,(k-1)))+nHeight; j ++)           
			{
				int m, h;
				m=height*(1-1/pow(2,(k-1)));
				h= (j-m)*2 ;
				temp2[j*wide+i] = temp1[(m+h)*wide+i];		//even
				temp2[(nHeight + j)*wide+i] = temp1[(m+h+1)*wide+i];	//odd
			}
		}
		//通过图像的差分，完成小波变换
		for(i=0; i<wide/pow(2,k-1); i++)
		{
			for(j =  height*(1-1/pow(2,(k-1))); j <  height*(1-1/pow(2,(k-1)))+nHeight; j ++) 
			{
				temp2[ j*wide+i] = temp2[j*wide+i]-temp2[(nHeight + j)*wide+i]+128;
			}
		}
		//小波经过处理后，放入显示缓存中
		for(j = height*(1-1/pow(2,(k-1))); j < height; j ++)    
		{
			for(i=0; i<wide/pow(2,k-1); i++)
			{
				m_pData2[j*wide+i]  = temp2[j*wide+i];
			}
		}
		//删除临时的数据空间
		delete temp1;
		delete temp2;
	}
}

/********************************************************************************
*函数描述：	IDWT 实现小波反变换
*函数参数：	int n 小波变换的层数 
*函数返回值：函数无返回值						  
*********************************************************************************/
void CWvltTransDib::IDWT(int n)
{
	int i,j;
	LONG wide,height,nWide,nHeight;
	LPBYTE temp1, temp2,temp3,m_pData2;
	for(int k=n;k>=1;k--)
    {
		wide=(this->GetWidth());
		height=(this->GetHeight());
		m_pData2=this->GetData2();
		//分配临时数据空间
		temp1 = new BYTE[height*wide];
		temp2 = new BYTE[height*wide];
		temp3 = new BYTE[height*wide];
		nWide=wide/pow(2,k);
		nHeight=height/pow(2,k);
		memcpy(temp1,m_pData2,height*wide);
		for(i=0; i<wide/pow(2,k-1); i++)
		{
			for(j =  height*(1-1/pow(2,(k-1))); j <  height*(1-1/pow(2,(k-1)))+nHeight; j ++) 
			{
				temp1[ j*wide+i]=temp1[(nHeight + j)*wide+i]+ temp1[j*wide+i]-128;
			}
		}
		for(i = 0; i < wide/pow(2,k-1); i ++)            
		{
			for(j =  height*(1-1/pow(2,(k-1))); j <  height*(1-1/pow(2,(k-1)))+nHeight; j ++)           
			{
				int m, h;
				m=height*(1-1/pow(2,(k-1)));
				h= (j-m)*2 ;
				temp2[(m+h)*wide+i]=temp1[j*wide+i];
				temp2[(m+h+1)*wide+i]=temp1[(nHeight + j)*wide+i];
			}
		}
		for(j= height*(1-1/pow(2,(k-1))); j<height; j++)
		{
			for(i=0; i<nWide-1; i++)
			{
				temp2[j*wide+nWide+i] += temp2[j*wide+i]-128;	
			}
		}
		for(j = height*(1-1/pow(2,(k-1))); j < height; j ++)            
		{
			for(i = 0; i < nWide; i ++)           
			{
				int w = i *2;
				temp3[j*wide+w]=temp2[j*wide+i];
				temp3[j*wide+w+1]=temp2[j*wide+nWide+i];
			}
		}		
		//小波经过处理后，放入显示缓存中
		for(j = height*(1-1/pow(2,(k-1))); j < height; j ++)    
		{
			for(i=0; i<wide/pow(2,k-1); i++)
			{
				m_pData2[j*wide+i]  = temp3[j*wide+i];
			}
		}
		//删除临时的数据空间
		delete temp1;
		delete temp2;
	}
}

/********************************************************************************
*函数描述：	LowFilter实现小波低通滤波效果
*函数参数：	int n 小波变换层数 
*函数返回值：函数无返回值						  
*********************************************************************************/
void CWvltTransDib::LowFilter(int n)
{
	int i,j;
	LONG wide,height,nWide,nHeight;
	LPBYTE m_pData2;
	wide=this->GetWidth();
	height=this->GetHeight();
	m_pData2=this->GetData2();
	nWide=wide/2;
	nHeight=height/2;
	Once(n);
	for(j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{
			if((j>=height*(1-1/pow(2,(n-1))))&&(i<wide/pow(2,n)))
				continue;
			else
				m_pData2[j*wide+i] =(BYTE)128;
		}
	}
	IDWT(n);
}

/********************************************************************************
*函数描述：	HighFilter 实现高通滤波效果
*函数参数：	int n 小波变换层数 
*函数返回值：函数无返回值						  
*********************************************************************************/
void CWvltTransDib::HighFilter(int n)
{
	int i,j;
	LONG wide,height,nWide,nHeight;
	LPBYTE m_pData2;
	wide=this->GetWidth();
	height=this->GetHeight();
	m_pData2=this->GetData2();
	nWide=wide/2;
	nHeight=height/2;
	LONG lLineBytes = (wide*3+3)/4 *4;	
	Once(n);
	for(j=height*(1-1/pow(2,(n-1)));j<height;j++)
	{
		for(i=0;i<wide/pow(2,n);i++)
		{
			m_pData2[j*wide+i] =(BYTE)128;
		}
	}
	IDWT(n);
}
