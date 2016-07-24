// chess.cpp: implementation of the chess class.
//
//////////////////////////////////////////////////////////////////////
//其中一种颜色的棋子的类

#include "stdafx.h"
#include "wuziqi.h"
#include "chess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

chess::chess()
{
	nCountkey=0;	

}

chess::~chess()
{

}

chess::keydown (int x,int y,int color)
{
	nCountkey++;
	nKeydown[nCountkey]=x*100+y;
	bqipan[x][y]=1;


	return 1;
}

void chess::clearall()
{
	int nCount1,nCount2;
	for(nCount1=0;nCount1<=17;nCount1++)
		for(nCount2=0;nCount2<=17;nCount2++)
		{
			bqipan[nCount1][nCount2]=0;
		}
	nCountkey=0;
	//nKeydown=-1;
}


