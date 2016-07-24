#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "ZhengJiaoBianHuanDib.h"
#include "malloc.h"
#include "MainFrm.h"
#include "DSplitView.h"
#include "Cdib.h"

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 



#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359
#define PI 3.1415926535 
 
ZhengJiaoBianHuanDib::ZhengJiaoBianHuanDib()
{
}
ZhengJiaoBianHuanDib::~ZhengJiaoBianHuanDib()
{
}

CNumber ZhengJiaoBianHuanDib::Add(CNumber c1,CNumber c2)
{
	CNumber c;
	c.re=c1.re+c2.re;
	c.im=c1.im+c2.im;
	return c;
}
CNumber ZhengJiaoBianHuanDib::Sub(CNumber c1,CNumber c2)
{
	CNumber c;
	c.re=c1.re-c2.re;
	c.im=c1.im-c2.im;
	return c;
}
CNumber ZhengJiaoBianHuanDib::Mul(CNumber c1,CNumber c2)
{
	CNumber c;
	c.re=c1.re*c2.re-c1.im*c2.im;
	c.im=c1.re*c2.im+c2.re*c1.im;
	return c;
}
///***************************************************************/           
/*函数名称：QFC(CNumber* t,CNumber* f,int r)
  参数:    t、f分别是指向时域和频域的指针，r是2的幂数                                   
/*函数类型：void
/*功能：此函数实现快速傅立叶变换         
/***************************************************************///此函数用来实现

void ZhengJiaoBianHuanDib::QFC(CNumber* t,CNumber* f,int r)
{
	long count;//傅立叶变换点数
	int i,j,k,p,bfsize;
	CNumber *w,*x,*a,*b;//复数结构类型的指针变量，其中w指向加权系数
	double angle;//计算加权系数所用的角度值
	count=1<<r;//计算傅立叶变换点数
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
	for(k=0;k<r;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			bfsize=1<<(r-k);
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
		for(i=0;i<r;i++)
		{
			if(j&(1<<i))
				p+=1<<(r-i-1);
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
void ZhengJiaoBianHuanDib::QuickFourier()
{
	LPBYTE  p_data, p;//指向原图象数据区指针
	int width,height;//原图象的宽度和高度       
	long w=1,h=1;//进行傅立叶变换的宽度和高度（2的整数次方）
	int wp=0,hp=0;//迭代次数
	int i,j;
	double temp;//中间变量
	CNumber *t,*f;
	p_data=this->GetData();//指向原图象数据区
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
			p=p_data+lLineBytes*(height-j-1)+i;//指向第j行第i列象素
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
	for(i=0;i<w;i++)//水平方向进行快速傅立叶变换
	{
		QFC(&t[i*h],&f[i*h],hp);
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


/*************************************************************************
 *
 * 函数名称：LiSan(double *t, double *f, int r)
 *
 * 参数:
 *   double * t				- 指向时域值的指针
 *   double * f				- 指向频域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速离散余弦变换。该函数利用2N点的快速付立叶变换
 * 来实现离散余弦变换。
 *
 ************************************************************************/
void ZhengJiaoBianHuanDib::LiSan(double *t, double *f, int r)
{
	// 离散余弦变换点数
	long	count;	
	// 循环变量
	int		i;	
	// 中间变量
	double	dTemp;
	CNumber *X;	
	// 计算离散余弦变换点数
	count = 1<<r;
	// 分配内存
	X=new CNumber[count*2];
	// 赋初值为0
	memset(X, 0, sizeof(CNumber) * count * 2);
	// 将时域点写入数组X
	for(i=0;i<count;i++)
	{
		X[i].re=t[i];
		X[i].im=0;
	}
	// 调用快速付立叶变换
	QFC(X,X,r+1);
	// 调整系数
	dTemp = 1/sqrt(count);
	// 求F[0]
	f[0] = X[0].re * dTemp;
	dTemp *= sqrt(2);
	// 求F[u]	
	for(i = 1; i < count; i++)
	{
		f[i]=(X[i].re * cos(i*PI/(count*2)) + X[i].im * sin(i*PI/(count*2))) * dTemp;
	}	
	// 释放内存
	delete X;
}

/*************************************************************************
 *
 * 函数名称：
 *   WALSH()
 *
 * 参数:
 *   double * t				- 指向时域值的指针
 *   double * f				- 指向频域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速沃尔什-哈达玛变换。
 *
 ************************************************************************/

void ZhengJiaoBianHuanDib::WALSH(double *t, double *f, int r)
{
	// 沃尔什-哈达玛变换点数
	long   count;	
	// 循环变量
	int		i,j,k;	
	// 中间变量
	int		bfsize,p;
	double *X1,*X2,*X;	
	// 计算快速沃尔什变换点数
	count = 1 << r;
	// 分配运算所需的数组
	X1 = new double[count];
	X2 = new double[count];
	// 将时域点写入数组X1
	memcpy(X1, t, sizeof(double) * count);
	// 蝶形运算
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1<<k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
			}
		}
		// 互换X1和X2  
		X = X1;
		X1 = X2;
		X2 = X;
	}
	// 调整系数
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1 << (r-i-1);
			}
		}
		f[j] = X1[p] / count;
	}	
	// 释放内存
	delete X1;
	delete X2;
}


/*************************************************************************
 *
 * 函数名称：DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
 *
 * 参数:
 *   LPBYTE lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *  bool               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行离散余弦变换。
 *
 ************************************************************************/
bool ZhengJiaoBianHuanDib::DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 循环变量
	LONG	i;
	LONG	j;
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	// 中间变量
	double	dTemp;
	int		wp;
	int		hp;
	// 图像每行的字节数
	LONG	lLineBytes;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	// 分配内存
	double *f = new double[w * h];
	double *F = new double[w * h];
	// 行
	for(j = 0; j < h; j++)
	{
		// 列
		for(i = 0; i < w; i++)
		{
			// 指向DIB第j行，第i个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			// 给时域赋值
			f[i + j * w] = *(lpSrc);
		}
	}
	
	for(j = 0; j < h; j++)
	{
		// 对y方向进行离散余弦变换
		LiSan(&f[w * j], &F[w * j], wp);
	}
	// 保存计算结果
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
			f[i * h + j] = F[i + w * j];
		}
	}
	for(i = 0; i < w; i++)
	{
		// 对x方向进行离散余弦变换
		LiSan(&f[i * h], &F[i * h], hp);
	}
	// 行
	for(j = 0; j < h; j++)
	{
		// 列
		for(i = 0; i < w; i++)
		{
			// 计算频谱
			dTemp = fabs(F[i*h+j]);
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}	
	// 释放内存
	delete f;
	delete F;
	//返回
	return true;
}



/*************************************************************************
 *
 * 函数名称：
 *   DIBWalsh()
 *
 * 参数:
 *   LPBYTE lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *  bool               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行沃尔什-哈达玛变换。于上面不同的是，此处是将二维
 * 矩阵转换成一个列向量，然后对该列向量进行一次一维沃尔什-哈达玛变换。
 *
 ************************************************************************/

bool ZhengJiaoBianHuanDib::DIBWalsh(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
{	
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 循环变量
	LONG	i;
	LONG	j;
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	// 中间变量
	double	dTemp;
	int		wp;
	int		hp;
	// 图像每行的字节数
	LONG	lLineBytes;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	// 分配内存
	double *f = new double[w * h];
	double *F = new double[w * h];
	// 列
	for(j = 0; j < w; j++)
	{
		// 行
		for(i = 0; i < h; i++)
		{
			// 指向DIB第j行，第i个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// 给时域赋值
			f[i + j * w] = *(lpSrc);
		}
	}	
	// 调用快速沃尔什－哈达玛变换
	WALSH(f, F, wp + hp);
	// 列
	for(j = 0; j < w; j++)
	{
		// 行
		for(i = 0; i < h; i++)
		{
			// 计算频谱
			dTemp = fabs(F[j * w + i] * 1000);
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			// 指向DIB第j行，第i个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}	
	//释放内存
	delete f;
	delete F;
	//返回
	return true;
}
