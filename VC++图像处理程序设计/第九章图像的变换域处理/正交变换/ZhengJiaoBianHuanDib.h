#ifndef __ZhengJiaoBianHuanDib_H
#define __ZhengJiaoBianHuanDib_H

#include "Cdib.h"

struct CNumber
{
	double re;
	double im;
};
class ZhengJiaoBianHuanDib :public CDib
{
public:
	ZhengJiaoBianHuanDib();
	~ZhengJiaoBianHuanDib();

public:
	 CNumber Add(CNumber c1,CNumber c2);//实现复数的加法运算
	 CNumber Sub(CNumber c1,CNumber c2);//实现复数的减法运算
	 CNumber Mul(CNumber c1,CNumber c2);//实现复数的乘法运算
	 void QFC(CNumber* t,CNumber* f,int power);//此函数实现快速傅立叶变换
	 void QuickFourier();//此函数用来实现图象的傅立叶变换
     void LiSan(double *t, double *f, int r);//此函数实现离散余弦变换
     void WALSH(double *t, double *f, int r);//该函数用来实现快速沃尔什-哈达玛变换。
     bool DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);//此函数实现图像的离散余弦变换
     bool DIBWalsh(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);//该函数用来实现图像的沃尔什-哈达玛变换。

protected:

};
#endif