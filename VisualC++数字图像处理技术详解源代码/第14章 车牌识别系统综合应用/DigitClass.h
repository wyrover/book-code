// DigitClass.h: interface for the CDigitClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITCLASS_H__84110994_734D_4BC7_A51F_79A9EAA15792__INCLUDED_)
#define AFX_DIGITCLASS_H__84110994_734D_4BC7_A51F_79A9EAA15792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Dib.h"

//#define DigitMaxWidth 40
//#define DigitMaxHeight 60
//#define DigitSize 2400	//2400=40*60

#define DigitMaxWidth 100
#define DigitMaxHeight 100
#define DigitSize 10000	//2400=40*60

struct sample
{
	double feature[13];//样本的特征向量
	char trueClass[3];//样本所属的真实类别
	int serialnum;//序列号
};

typedef struct sample Sample;

class CDigitClass  
{
public:
	BYTE Interpolation (BYTE* image, LONG lWidth, LONG lHeight, FLOAT x, FLOAT y);
	void MarrBinary();
	float Marroperator(float dr,int x,int y);
	void FixSize();
	void FixSize1();
	//构造函数和析构函数
	CDigitClass();
	virtual ~CDigitClass();
	//变量
//	CDib digitdib;//对应着数字的位图
	BYTE digitarray[DigitMaxHeight][DigitMaxWidth];
	BYTE digitarray1[DigitSize];
	int digitWidth; //数字图象的宽度
	int digitHeight; //数字图象的高度
	double feature[13];
	//函数
//	void CopyArToBitmap(void);
//	void CopyBitmapToAr(void);
	void BinaryDigit(BYTE thre);
	void BinaryDigit();
	//用四种不同的方法细化
	void ThinDigit_1();
	void ThinDigit_2();
	void ThinDigit_3();
	void ThinDigit_4();
	void GetFeature();
	//提取特征函数
};

#endif // !defined(AFX_DIGITCLASS_H__84110994_734D_4BC7_A51F_79A9EAA15792__INCLUDED_)
