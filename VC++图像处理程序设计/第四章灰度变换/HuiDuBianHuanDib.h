#ifndef __HuiDuBianHuanDib_H
#define __HuiDuBianHuanDib_H
#include "Cdib.h"

class HuiDuBianHuanDib :public CDib
{
public:
	HuiDuBianHuanDib();
	~HuiDuBianHuanDib();
	
public:
	void Midufenbu(int *nNs_R,int *nNs_G,int *nNs_B);
	void ZhiFangTu(float *tongji,float *tongji2,float *tongji3);
	void GreenZhiFangTu(float *tongji);
	void BlueZhiFangTu(float *tongji);
	void RedZhiFangTu(float *tongji);
	void Fei0();//对图像进行非零取一运算
	void GuDing(int YuZhi);//对图像进行固定阀值运算
	void ShuangYu(int YuZhi1,int YuZhi2,int mode);//双固定域值法
	void Chuizhitouying();//对图像进行垂直投影
	void Shuipingtouying();//对图像进行水平投影
	void Midufenbu(int* nNs_R);//计算图像的密度分布
	void Pipeibianhuan(BYTE bNum, int *npNu, float *fpPu);//进行灰度匹配变换
	int GetWidth();
	void Fenbujunhenghua(  );//进行分布均衡变换
	void Chuangkoubianhuan(BYTE bLow, BYTE bUp);//灰度窗口变换
	void Zhexianbianhuan(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2);//灰度折线变换
	void ZhiFangTu(float* tongji);//灰度直方图现实
	void FanSeBianHuan();//图像反色效果


protected:
};
#endif