#include "Astaritem.h"
AstarItem::AstarItem(void)
{
}
AstarItem::~AstarItem(void)
{
}
void AstarItem::setpos(int col,int row)
{
	id_col = col;
	id_row = row;
}
void AstarItem::setg(int g)
{
	id_g = g;
}
void AstarItem::seth(int h)
{
	id_h = h;
}
void AstarItem::setfid(int fid)
{
	id_fid = fid;
}
void AstarItem::setf(int f)
{
	id_f = f;
}