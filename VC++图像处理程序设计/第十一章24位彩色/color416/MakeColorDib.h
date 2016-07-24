// MakeColorDib.h: interface for the MakeColorDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAKECOLORDIB_H__C1036995_2726_4538_BDAC_B392A7192AD9__INCLUDED_)
#define AFX_MAKECOLORDIB_H__C1036995_2726_4538_BDAC_B392A7192AD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cdib.h"

class MakeColorDib : public CDib  
{
public:
	void Mosaic();
	void LowLVBObig();
	void ShuangXiangGROW();
	void ChuiZhiGROW();
	void ShuiPingGROW();
	void LowLVBO();
	void HighLVBO(int m_GaoTong);
	void Sharp();
	void Spread();
	void Embossment();
	void NeonLight();
	void Smoothness();
	void PaintColor(int m_Red,int m_Green,int m_Blue);
	void Exposal();
	void ContrastAlter(int m_Increment);
	void LightReverse();
	void LightAlter(int m_Light);
	MakeColorDib();
	virtual ~MakeColorDib();
	void MakegGray();
};

#endif // !defined(AFX_MAKECOLORDIB_H__C1036995_2726_4538_BDAC_B392A7192AD9__INCLUDED_)
