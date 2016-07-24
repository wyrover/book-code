#ifndef __TuXiangHeChengDib_H
#define __TuXiangHeChengDib_H
#include "Cdib.h"
class TuXiangHeChengDib :public CDib
{
public:
	 CBitmap m_bitmap;
	 CString filename;
	  
	 BOOL f_Show;//刷新视图时的开关量
public:
	void Yihuo(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	void Yufei(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	void Huofei(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	void Feiyunsuan();
	void Huoyunsuan(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	void Yuyunsuan(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	void Sub(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	void Add(LPBYTE p_data, LPBYTE p_dataBK, int wide, int height);
	 
	TuXiangHeChengDib();
	~TuXiangHeChengDib();



protected:
	 
    
};
#endif