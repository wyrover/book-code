#ifndef __PinYuLuBoDib_H
#define __PinYuLuBoDib_H
#include "Cdib.h"
struct CNumber
{
	double re;
	double im;
};
class PinYuLuBoDib :public CDib
{

public:
	PinYuLuBoDib();
	~PinYuLuBoDib();

public:
	 CNumber Add(CNumber c1,CNumber c2);//实现复数的加法运算
	 CNumber Sub(CNumber c1,CNumber c2);//实现复数的减法运算
	 CNumber Mul(CNumber c1,CNumber c2);//实现复数的乘法运算
	 void QFC(CNumber* t,CNumber* f,int power);//实现快速傅立叶变换
	 void QuickFourier();//实现图像快速傅立叶变换
     void fourier(double * data, int height, int width, int isign);//实现二维傅立叶变换
	 void BWFilterL(int u,int v,int n);//布特沃斯低通滤波
	 void BWFilterH(int u,int v,int n);//布特沃斯高通滤波
	 void PerfectFilterL(int u,int v);//理想低通滤波器
	 void PerfectFilterH(int u,int v);//理想低通滤波器
	 void TLFilter(int u,int v,int u1,int v1);//梯形低通滤波
	 void THFilter(int u,int v,int u1,int v1);//梯形高通滤波
	 void ZLFilter(int u,int v,int n);//指数低通滤波
	 void ZHFilter(int u,int v,int n);//指数高通滤波
     void FirstQuickFourier();//实现图像快速傅立叶变换(未进行平移)
	 

protected:

};
#endif