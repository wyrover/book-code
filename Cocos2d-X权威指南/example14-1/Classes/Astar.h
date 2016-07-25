#pragma once
#include "cocos2d.h"
#include "AstarItem.h"
using namespace cocos2d;
class Astar
{
private:
	int curCol, curRow, aimCol, aimRow;
	int AimX, AimY, AimW, AimH; 
	CCTMXTiledMap* map;
    CCArray *open;
    CCArray *close;
    CCArray *path;
public:
	Astar(void);
	~Astar(void);
	int getG(int col,int row,int id);
	int getH(int col,int row);
	void fromopentoclose();
	void removefromopen();
	void getpath();
	void starseach(int fid);
	void resetSort(int last);
	bool checkclose(int col,int row);
	void addtoopen(int col,int row,int id);
	bool checkmap(int col,int row);
	bool checkOpen(int col,int row,int id);
    CCArray *findPath(int curX, int curY, int aimX, int aimY, CCTMXTiledMap* passmap);
};
