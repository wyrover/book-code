//PosDirect.h 带下一步方向的二维坐标位置类型结构体
#ifndef _POSDIRECT_H_
#define _POSDIRECT_H_
#include "PosType.h"
struct PosDirect: public PosType {
    int direct;
};
#endif

