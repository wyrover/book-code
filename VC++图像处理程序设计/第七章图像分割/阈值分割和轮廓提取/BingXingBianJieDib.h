#ifndef __BingXingBianJieDib_H
#define __BingXingBianJieDib_H
#include "Cdib.h"
 
typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;

typedef struct{
	int Height;
	int Width;
}	Seed;

typedef struct{
	int Height;
	int Width;
}	Point;

class BingXingBianJieDib :public CDib
{

public:
	BingXingBianJieDib();
	~BingXingBianJieDib();

public:
	void Lunkuogenzong(CPoint SeedPoint);
	void Lunkuotiqu(CPoint SeedPoint);
	void BanYuZhi(int Yuzhi);
	void Zhifangtu(float *tongji);
	void Qiyuzengzhang(CPoint point);
	void Yuzhifenge(int Yuzhi);
	void Zhongzitianchong(CPoint SeedPoint);
	void Lunkuogenzong();
	void Fenbutongji(int* tongji);
	void Diedaifazhi(int *fazhi);
	void Lunkuotiqu();


protected:
};
#endif