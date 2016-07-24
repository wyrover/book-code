// JisuanProcessDib.h: interface for the JisuanProcessDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JISUANPROCESSDIB_H__6385E9FA_7E01_4785_9F75_56E9F77F4702__INCLUDED_)
#define AFX_JISUANPROCESSDIB_H__6385E9FA_7E01_4785_9F75_56E9F77F4702__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cdib.h"

typedef struct{
	int Height;
	int Width;
}Point;

struct object{
	int pp_x;
	int pp_y;
	int pp_area;
	int pp_line;
	int pp_number;
};


class JisuanProcessDib : public CDib  
{
public:
	void Baoliu(LPBYTE temp);

	void MakeGray();
	void LianTong(); 
	void Borderline();
	void ClearSMALL(int m_value);
	void biaoji();
	void xiaochugulidianBAI();
	void xiaochugulidianHEI();
	void erzhihua(int yuzhi_gray);
             JisuanProcessDib();
	virtual ~JisuanProcessDib();

	BYTE *p_data;     //原图数据区指针
	int wide,height;    //原图长、宽
////////////////////////////////////////////////////
    object pppp[255];//*pppp,

    int x_sign;
	int flag[255];
	int m_temp;
	int x_temp;
	int y_temp;
	BYTE *p_temp;

	int stop;
////////////////////////////////////////////////////

};

#endif // !defined(AFX_JISUANPROCESSDIB_H__6385E9FA_7E01_4785_9F75_56E9F77F4702__INCLUDED_)
