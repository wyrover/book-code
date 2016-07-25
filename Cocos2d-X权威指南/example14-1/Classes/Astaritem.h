#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class AstarItem:public CCNode
{
public:
    AstarItem(void);
	~AstarItem(void);
	void setpos(int col,int row);
	int getcol(){return id_col;};
	int getrow(){return id_row;};
	void setg(int g);
	int getg(){return id_g;};
	void seth(int h);
	int geth(){return id_h;};
	void setfid(int fid);
	int getfid(){return id_fid;};
	void setf(int f);
	int getf(){return id_f;};
private:
	int id_col;//列
	int id_row;//行
	int id_g;// 实际代价
	int id_h;// 估计代价
	int id_fid;// 父节点id
	int id_f;// 估价函数f = g + h
};