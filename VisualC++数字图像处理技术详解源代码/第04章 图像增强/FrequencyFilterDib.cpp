//======================================================================
// 文件： FrequencyFilterDib.cpp
// 内容： 图像频域滤波增强-源文件
// 功能： （1）傅立叶变换函数；       
//        （2）理想低通滤波函数
//        （3）理想高通滤波函数
// 
//======================================================================

#include "StdAfx.h"
#include "FrequencyFilterDib.h"
#include<math.h>
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 


#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359


//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CFrequencyFilterDib::CFrequencyFilterDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CFrequencyFilterDib::~CFrequencyFilterDib(void)
{
	
}


//=======================================================
// 函数功能： 傅立叶变换函数
// 输入参数： double * data-时域数据指针
//            int height-图像的高度
//            int width-图像宽度
//            int isign-表示正反变换
// 返回值：   无
//=======================================================

void CFrequencyFilterDib::fourier(double * data, int height, int width, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot,nn[3];
	double tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;	
	ntot=height*width; 
	nn[1]=height;
	nn[2]=width;
	nprev=1;
	for (idim=2;idim>=1;idim--) 
	{
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1)
		{
			if (i2 < i2rev) 
			{
				for (i1=i2;i1<=i2+ip1-2;i1+=2) 
				{
					for (i3=i1;i3<=ip3;i3+=ip2)
					{
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit)
			{
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) 
		{
			ifp2=ifp1 << 1;
			theta=isign*pi*2/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1)
			{
				for (i1=i3;i1<=i3+ip1-2;i1+=2) 
				{
					for (i2=i1;i2<=ip3;i2+=ifp2) 
					{
						k1=i2;
						k2=k1+ifp1;
						tempr=wr*data[k2]-wi*data[k2+1];
						tempi=wr*data[k2+1]+wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}


//=======================================================
// 函数功能： 理想低通滤波函数
// 输入参数： int u,int v-截止频率的分量值
// 返回值：   无
//=======================================================
void CFrequencyFilterDib::Perfect_Low_Filter(int u,int v)
{
    LPBYTE	lpSrc;			// 指向原图像当前点的指针
    long i,j;			//循环变量
	double d0;  //截止频域变量
    double max=0.0; //归一化因子

	double *t;  //空域数据指针
    double *H;  //传递函数指针

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度 	
	
    long lLineBytes=WIDTHBYTES(lWidth*8);//计算图象每行的字节数
	t=new double [lHeight*lLineBytes*2+1]; //分配空域空间
	H=new double [lHeight*lLineBytes*2+1]; //分配传递函数空间
	d0=sqrt((float)(u*u+v*v)) ;  //计算截止频率d0

    //给空域赋值，并计算传递函数
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			lpSrc=lpDIBBits+lLineBytes*j+i;//指向第i行第j列象素
			//给空域赋值
            t[(2*lLineBytes)*j+2*i+1]=*lpSrc;
			t[(2*lLineBytes)*j+2*i+2]=0.0;

            //计算传递函数
			if((sqrt((float)(i*i+j*j)))<=d0)
				H[2*i+(2*lLineBytes)*j+1]=1.0;
			else
				H[2*i+(2*lLineBytes)*j+1]=0.0;
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    //进行傅立叶变换
    fourier(t,lHeight,lLineBytes,1);

    //傅立叶变换结果与传递函数进行卷积运算
	for(j=1;j<lHeight*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
    //进行傅立叶逆变换
	fourier(t,lHeight,lLineBytes,-1);
    
    //计算归一化因子
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}

    //计算结果图像,并保存到原图像数据区
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			lpSrc=lpDIBBits+lLineBytes*j+i;
			*lpSrc=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
    //释放内存空间
	delete t;
	delete H;
}

//=======================================================
// 函数功能： 理想高通滤波函数
// 输入参数： int u,int v-截止频率的分量值
// 返回值：   无
//=======================================================
void CFrequencyFilterDib::Perfect_High_Filter(int u,int v)
{
    LPBYTE	lpSrc;			// 指向原图像当前点的指针
    long i,j;			//循环变量
	double d0;  //截止频域变量
    double max=0.0; //归一化因子

	double *t;  //空域数据指针
    double *H;  //传递函数指针

	LPBYTE lpDIBBits=m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth=m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight=m_pDib->GetHeight();  //获得原图像的高度 	
	
    long lLineBytes=WIDTHBYTES(lWidth*8);//计算图象每行的字节数
	t=new double [lHeight*lLineBytes*2+1]; //分配空域空间
	H=new double [lHeight*lLineBytes*2+1]; //分配传递函数空间
	d0=sqrt((float)(u*u+v*v)) ;  //计算截止频率d0

    //给空域赋值，并计算传递函数
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			lpSrc=lpDIBBits+lLineBytes*j+i;//指向第i行第j列象素
			//给空域赋值
            t[(2*lLineBytes)*j+2*i+1]=*lpSrc;
			t[(2*lLineBytes)*j+2*i+2]=0.0;

            //计算传递函数
			if((sqrt((float)(i*i+j*j)))<=d0)
				H[2*i+(2*lLineBytes)*j+1]=0.0;
			else
				H[2*i+(2*lLineBytes)*j+1]=1.0;
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    //进行傅立叶变换
    fourier(t,lHeight,lLineBytes,1);

    //傅立叶变换结果与传递函数进行卷积运算
	for(j=1;j<lHeight*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
    //进行傅立叶逆变换
	fourier(t,lHeight,lLineBytes,-1);
    
    //计算归一化因子
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}

    //计算结果图像
	for(j=0;j<lHeight;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			lpSrc=lpDIBBits+lLineBytes*j+i;
			*lpSrc=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
    //释放内存空间
	delete t;
	delete H;
}