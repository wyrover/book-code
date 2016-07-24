// WvltTrans.h: interface for the CWvltTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WVLTTRANS_H__ADB84B9A_B6D4_4770_B795_CCC4F2554D50__INCLUDED_)
#define AFX_WVLTTRANS_H__ADB84B9A_B6D4_4770_B795_CCC4F2554D50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Cdib.h"
#include "math.h"
class CWvltTransDib :public CDib  
{
public:
	//void GetDucument();
	CWvltTransDib();
	virtual ~CWvltTransDib();

public:
	void HighFilter(int n);
	void LowFilter(int n);
	void IDWT(int n);
	void Once(int n);
	void Liebianhuan();
	void Hangbianhuan();
	//图像小波变换的逆变换
	//三次小波变换
	//两次小波变换
	//一次小波变换
	//竖直方向的小波变换
	//水平方向的小波变换


};


#endif // !defined(AFX_WVLTTRANS_H__ADB84B9A_B6D4_4770_B795_CCC4F2554D50__INCLUDED_)
