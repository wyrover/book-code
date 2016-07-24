#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "PinYuLuBoDib.h"
#include "malloc.h"
#include "MainFrm.h"
#include "DSplitView.h"
#include "Cdib.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 


#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359
 
 
PinYuLuBoDib::PinYuLuBoDib()
{
}
PinYuLuBoDib::~PinYuLuBoDib()
{
}

CNumber PinYuLuBoDib::Add(CNumber c1,CNumber c2)
{
	CNumber c;
	c.re=c1.re+c2.re;
	c.im=c1.im+c2.im;
	return c;
}
CNumber PinYuLuBoDib::Sub(CNumber c1,CNumber c2)
{
	CNumber c;
	c.re=c1.re-c2.re;
	c.im=c1.im-c2.im;
	return c;
}
CNumber PinYuLuBoDib::Mul(CNumber c1,CNumber c2)
{
	CNumber c;
	c.re=c1.re*c2.re-c1.im*c2.im;
	c.im=c1.re*c2.im+c2.re*c1.im;
	return c;
}

///////////////////////////////////////////////////
//此函数实现快速傅立叶变换
//参数t、f分别是指向时域和频域的指针，power是2的幂数
///////////////////////////////////////////////////
void PinYuLuBoDib::QFC(CNumber* t,CNumber* f,int power)
{
	long count;//傅立叶变换点数
	int i,j,k,p,bfsize;
	CNumber *w,*x,*a,*b;//复数结构类型的指针变量，其中w指向加权系数
	double angle;//计算加权系数所用的角度值
	count=1<<power;//计算傅立叶变换点数
	//分配所需运算空间
	w=new CNumber[count/2];
	a=new CNumber[count];
	b=new CNumber[count];
	//计算加权系数
	for(i=0;i<count/2;i++)
	{
		angle=-i*pi*2/count;
		w[i].re=cos(angle);
		w[i].im=sin(angle);
	}
	memcpy(a,t,sizeof(CNumber)*count);
	//采用频率分解法进行蝶形运算
	for(k=0;k<power;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			bfsize=1<<(power-k);
			for(i=0;i<bfsize/2;i++)
			{
				p=j*bfsize;
				b[i+p]=Add(a[i+p],a[i+p+bfsize/2]);
				b[i+p+bfsize/2]=Mul(Sub(a[i+p],a[i+p+bfsize/2]),w[i*(1<<k)]);
			}
		}
		x=a;
		a=b;
		b=x;
	}
	//将乱序的变换序列重新排序
	for(j=0;j<count;j++)
	{
		p=0;
		for(i=0;i<power;i++)
		{
			if(j&(1<<i))
				p+=1<<(power-i-1);
		}
		f[j]=a[p];
	}
	//释放存储器空间
	delete w;
	delete a;
	delete b;
}

///////////////////////////////////////////////
//此函数用来实现图象的傅立叶变换
//两次调用快速傅立叶变换QFC()实现二维傅立叶变换
///////////////////////////////////////////////
void PinYuLuBoDib::QuickFourier()
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	long w=1,h=1;//进行傅立叶变换的宽度和高度（2的整数次方）
	int wp=0,hp=0;//迭代次数
	int i,j;
	double temp;//中间变量
	CNumber *t,*f;
    if(this->byBitCount==8)	//灰度图像
		   p_data=this->GetData();//指向原图象数据区
	else	//24位真彩色
		   p_data=this->GetData2();
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	while(w*2<=width)//计算进行傅立叶变换的宽度（2的整数次方）
	{
		w*=2;
		wp++;
	}
	while(h*2<=height)//计算进行傅立叶变换的高度（2的整数次方）
	{
		h*=2;
		hp++;
	}
	t=new CNumber[w*h];//分配存储器空间
	f=new CNumber[w*h];
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			p=p_data+lLineBytes*(height-j-1)+i;//指向第i行第j列象素
			t[i+w*j].re=*(p);//给时域赋值
			t[i+w*j].im=0;
		}
	}
	for(j=0;j<h;j++)//在垂直方向上进行快速傅立叶变换
	{
		QFC(&t[w*j],&f[w*j],wp);
	}
	for(j=0;j<h;j++)//转换变换结果
	{
		for(i=0;i<w;i++)
		{
			t[j+h*i]=f[i+w*j];
		}
	}
	for(j=0;j<w;j++)//水平方向进行快速傅立叶变换
	{
		QFC(&t[j*h],&f[j*h],hp);
	}
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			temp=sqrt(f[i*h+j].re*f[i*h+j].re+f[i*h+j].im*f[i*h+j].im)/100;
			if(temp>255)
				temp=255;
			
			p=p_data+lLineBytes*(height-(j<h/2?j+h/2:j-h/2)-1)+
				(i<w/2?i+w/2:i-w/2);//将变换后的原点移到中心
			
			*(p)=(BYTE)(temp);
		}
	}
	delete t;
	delete f;
}

///////////////////////////////////////////////
//此函数用来实现图象的傅立叶变换(没有对处理后的显示结果进行平移)
//两次调用快速傅立叶变换QFC()实现二维傅立叶变换
///////////////////////////////////////////////
void PinYuLuBoDib::FirstQuickFourier()
{
	LPBYTE  p_data,p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	long w=1,h=1;//进行傅立叶变换的宽度和高度（2的整数次方）
	int wp=0,hp=0;//迭代次数
	int i,j;
	double temp;//中间变量
	CNumber *t,*f;
    if(this->byBitCount==8)//灰度图像
		   p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		   p_data=this->GetData2();
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	while(w*2<=width)//计算进行傅立叶变换的宽度（2的整数次方）
	{
		w*=2;
		wp++;
	}
	while(h*2<=height)//计算进行傅立叶变换的高度（2的整数次方）
	{
		h*=2;
		hp++;
	}
	t=new CNumber[w*h];//分配存储器空间
	f=new CNumber[w*h];
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			p=p_data+lLineBytes*(height-j-1)+i;//指向第i行第j列象素
			t[i+w*j].re=*(p);//给时域赋值
			t[i+w*j].im=0;
		}
	}
	for(j=0;j<h;j++)//在垂直方向上进行快速傅立叶变换
	{
		QFC(&t[w*j],&f[w*j],wp);
	}
	for(j=0;j<h;j++)//转换变换结果
	{
		for(i=0;i<w;i++)
		{
			t[j+h*i]=f[i+w*j];
		}
	}
	for(j=0;j<w;j++)//水平方向进行快速傅立叶变换
	{
		QFC(&t[j*h],&f[j*h],hp);
	}
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			temp=sqrt(f[i*h+j].re*f[i*h+j].re+f[i*h+j].im*f[i*h+j].im)/100;
			if(temp>255)
				temp=255;
			p=p_data+lLineBytes*(height-(j<h/2?j+h/2:j-h/2)-1)+
				(i<w/2?i+w/2:i-w/2);//将变换后的原点移到中心
			p=p_data+lLineBytes*(height-(j<h/2?j:j)-1)+
				(i<w/2?i:i);
			*(p)=(BYTE)(temp);
		}
	}
	delete t;
	delete f;
}


//////////////////////////////////////////////////////////////
//该函数用来实现二维傅立叶变换
//参数height、width分别表示图象的高度和宽度，ising表示正反变换
//////////////////////////////////////////////////////////////
void PinYuLuBoDib::fourier(double * data, int height, int width, int isign)
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

/*************************************************************************
* 函数：BWFilterL(int u,int v,int u1,int v1)
*参数：u、v分别是截止频率的x、y分量值，由用户给定
*功能：此函数用来实现图象的布特沃斯低通滤波
/*************************************************************************/

void PinYuLuBoDib::BWFilterL(int u,int v,int n)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double max=0.0,d0,d;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		   p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		   p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			d=sqrt(i*i+j*j);
			H[2*i+(2*lLineBytes)*j+1]=1/(1+pow((d/d0),(2*n)));
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}



////////////////////////////////////////////////
//此函数用来实现图象的布特沃斯高通滤波
//参数u、v分别是截止频率的x、y分量值，由用户给定
////////////////////////////////////////////////
void PinYuLuBoDib::BWFilterH(int u,int v,int n)
{
	LPBYTE p_data,p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double max=0.0,d0,d;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		   p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		   p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			d=sqrt(j*j+i*i);
			H[2*i+(2*lLineBytes)*j+1]=1/(1+pow((d0/d),(2*n)));
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}


/*************************************************************************
* 函数：PerfectFilterL(int u,int v)
*参数：u、v分别是截止频率的x、y分量值，由用户给定
*功能：此函数用来实现图象的理想低通滤波
/*************************************************************************/

 void PinYuLuBoDib::PerfectFilterL(int u,int v)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double d0,max=0.0;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			if((sqrt(i*i+j*j))<=d0)
				H[2*i+(2*lLineBytes)*j+1]=1.0;
			else
				H[2*i+(2*lLineBytes)*j+1]=0.0;
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}


////////////////////////////////////////////////
//此函数用来实现图象的理想高通滤波
//参数u、v分别是截止频率的x、y分量值，由用户给定
////////////////////////////////////////////////
void PinYuLuBoDib::PerfectFilterH(int u,int v)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double d0,max=0.0;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			if((sqrt(i*i+j*j))<=d0)
				H[2*i+(2*lLineBytes)*j+1]=0.0;
			else
				H[2*i+(2*lLineBytes)*j+1]=1.0;
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}

/*************************************************************************
* 函数：TLFilter(int u,int v,int u1,int v1)
*参数：u、v分别是截止频率的x、y分量值，由用户给定
*功能：此函数用来实现图象的梯形低通滤波
/*************************************************************************/

void PinYuLuBoDib::TLFilter(int u,int v,int u1,int v1)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double max=0.0,d0,d,d1;//中间变量
	double *t,*H;

	if(this->byBitCount==8)//灰度图像
		   p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		   p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	d1=sqrt(u1*u1+v1*v1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			d=sqrt(i*i+j*j);
			if(d<d0)
			{
				H[2*i+(2*lLineBytes)*j+1]=1;
				H[2*i+(2*lLineBytes)*j+2]=0.0;
			}
			if(d>d1)
			{
				H[2*i+(2*lLineBytes)*j+1]=0.0;
				H[2*i+(2*lLineBytes)*j+2]=0.0;
			}
			else
			{
				H[2*i+(2*lLineBytes)*j+1]=(d-d1)/(d0-d1);
				H[2*i+(2*lLineBytes)*j+2]=0.0;
			}				
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}


////////////////////////////////////////////////
//此函数用来实现图象的梯形高通滤波
//参数u、v分别是截止频率的x、y分量值，由用户给定
////////////////////////////////////////////////
void PinYuLuBoDib::THFilter(int u,int v,int u1,int v1)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double max=0.0,d0,d,d1;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	d1=sqrt(u1*u1+v1*v1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			d=sqrt(i*i+j*j);
			
			if(d<d0)
			{
				H[2*i+(2*lLineBytes)*j+1]=0;
				H[2*i+(2*lLineBytes)*j+2]=0.0;
			}
			if(d>d1)
			{
				H[2*i+(2*lLineBytes)*j+1]=1;
				H[2*i+(2*lLineBytes)*j+2]=0.0;
			}
			else
			{
				H[2*i+(2*lLineBytes)*j+1]=(d-d0)/(d1-d0);
				H[2*i+(2*lLineBytes)*j+2]=0.0;
			}						
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}

/*************************************************************************
* 函数：ZLFilter(int u,int v,int u1,int v1)
*参数：u、v分别是截止频率的x、y分量值，由用户给定
*功能：此函数用来实现图象的指数低通滤波
///////////////////////////////////////////////*/
void PinYuLuBoDib::ZLFilter(int u,int v,int n)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double max=0.0,d0,d;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			d=sqrt(i*i+j*j);
			H[2*i+(2*lLineBytes)*j+1]=exp(-pow((d/d0),n));
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}


////////////////////////////////////////////////
//此函数用来实现图象的指数高通滤波
//参数u、v分别是截止频率的x、y分量值，由用户给定
////////////////////////////////////////////////
void PinYuLuBoDib::ZHFilter(int u,int v,int n)
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	int i,j;
	double max=0.0,d0,d;//中间变量
	double *t,*H;
	if(this->byBitCount==8)//灰度图像
		p_data=this->GetData();//指向原图象数据区
	else//24位真彩色
		p_data=this->GetData2();//指向原图象数据区
	width=this->GetWidth();//得到图象宽度
	height=this->GetHeight();//得到图象高度
    long lLineBytes=WIDTHBYTES(width*8);//计算图象每行的字节数
	t=new double [height*lLineBytes*2+1];//分配存储器空间
	H=new double [height*lLineBytes*2+1];
	d0=sqrt(u*u+v*v);//计算截止频率d0
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;//指向第i行第j列象素
			t[(2*lLineBytes)*j+2*i+1]=*(p);//给时域赋值
			t[(2*lLineBytes)*j+2*i+2]=0.0;
			d=sqrt(i*i+j*j);
			H[2*i+(2*lLineBytes)*j+1]=exp(-pow((d0/d),n));
			H[2*i+(2*lLineBytes)*j+2]=0.0;
		}
	}
    fourier(t,height,lLineBytes,1);
	for(j=1;j<height*lLineBytes*2;j+=2)
	{
		t[j]=t[j]*H[j]-t[j+1]*H[j+1];
		t[j+1]=t[j]*H[j+1]+t[j+1]*H[j];
	}
	fourier(t,height,lLineBytes,-1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			t[(2*lLineBytes)*j+2*i+1]=sqrt(t[(2*lLineBytes)*j+2*i+1]*t[(2*lLineBytes)*j+2*i+1]+t[(2*lLineBytes)*j+2*i+2]*t[(2*lLineBytes)*j+2*i+2]);
			if(max<t[(2*lLineBytes)*j+2*i+1])
				max=t[(2*lLineBytes)*j+2*i+1];
		}
	}       
	for(j=0;j<height;j++)
	{
		for(i=0;i<lLineBytes;i++)
		{
			p=p_data+lLineBytes*j+i;
			*(p)=(BYTE)(t[(2*lLineBytes)*j+2*i+1]*255.0/max);
		}
	}
	delete t;
	delete H;
}
