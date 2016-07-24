// chess.h: interface for the chess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHESS_H__C8AC0F7E_CDAE_4C3F_B5C8_EC25067A05CA__INCLUDED_)
#define AFX_CHESS_H__C8AC0F7E_CDAE_4C3F_B5C8_EC25067A05CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//其中一种颜色的棋子的类
class chess  
{
public:
	chess();
	virtual ~chess();
	void clearall();//clear all chess
	int nCountkey;//how many key downed
	int nKeydown[32];//downed key's position value=x*100+y exp:1205 means:x=12,y=5
	BOOL bqipan[18][18];//notused
	int keydown(int x,int y,int color);//put a key down
};

#endif // !defined(AFX_CHESS_H__C8AC0F7E_CDAE_4C3F_B5C8_EC25067A05CA__INCLUDED_)
