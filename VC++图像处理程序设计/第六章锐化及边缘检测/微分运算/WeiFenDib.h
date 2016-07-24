#ifndef __WeiFenDib_H
#define __WeiFenDib_H
#include "Cdib.h"
class WeiFenDib :public CDib
{
public:
	WeiFenDib();
	~WeiFenDib();
public:
	void HengXiang();   //横向微分函数
	void ZongXiang();   //纵向微分函数
	void ShuangXiang();    //双向一次微分函数
	void ErCi1();   //二次微分1函数
    void ErCi2();   //二次微分2函数
    void ErCi3();   //二次微分3函数
	
protected:
	
};
#endif