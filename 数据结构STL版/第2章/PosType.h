//PosType.h 二维坐标位置类型结构体及重载的运算符
#ifndef _POSTYPE_H_
#define _POSTYPE_H_
struct PosType {
    int row, col;
};
PosType operator+(const PosType p1, const PosType p2)
{
    PosType p;
    p.row = p1.row + p2.row;
    p.col = p1.col + p2.col;
    return p;
}
bool operator==(const PosType p1, const PosType p2)
{
    return p1.row == p2.row && p1.col == p2.col;
}
#endif

