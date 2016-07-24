#ifndef __JiHeBianHuanDib_H
#define __JiHeBianHuanDib_H
#include "Cdib.h"
class JiHeBianHuanDib :public CDib
{
public:
	LPBYTE FangDa(LPBYTE temp,float xzoom ,float yzoom, LONG wide,LONG	height,LONG	newwide,LONG newheight);   
	void PingYi(int m_Xmove, int m_Ymove);
	void SuoXiao(float xzoom,float yzoom);
	void clearmem2();
	void Xuanzhuan(double iRotateAngle);
	void Zhuanzhi();
	void JingXiang(bool fangshi);
	JiHeBianHuanDib();
	~JiHeBianHuanDib();

protected:
    
};
#endif