// MyMath.cpp: implementation of the MyMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaterShed.h"
#include "MyMath.h"
#include <MATH.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyMath::MyMath()
{
	srand( (unsigned)time( NULL ) );
}

MyMath::~MyMath()
{

}

DOUBLE MyMath::LineDistance(My3DPoint inPoint1, My3DPoint inPoint2)
{
	DOUBLE tempoffx = inPoint1.x - inPoint2.x;
	DOUBLE tempoffy = inPoint1.y - inPoint2.y;
	DOUBLE tempoffh = inPoint1.h - inPoint2.h;

	return sqrt(tempoffx*tempoffx + tempoffy*tempoffy + 
		tempoffh*tempoffh);
}

DOUBLE MyMath::TriangleArea(My3DPoint inPoint1, 
	My3DPoint inPoint2, My3DPoint inPoint3)
{
	//计算边长；
	DOUBLE a = LineDistance(inPoint1, inPoint2);
	DOUBLE b = LineDistance(inPoint2, inPoint3);
	DOUBLE c = LineDistance(inPoint3, inPoint1);

	//计算周长的1/2；
	DOUBLE s = (a+b+c) / 2;

	//计算面积；
	return sqrt( s * (s-a) * (s-b) * (s-c) );
}

DOUBLE MyMath::TetrahedronVol(My3DPoint inPoint1,
		My3DPoint inPoint2, My3DPoint inPoint3,
		My3DPoint inPoint4)
{		
	//四面体体积；
	DOUBLE xa,ya,ha,xb,yb,hb,xc,yc,hc,xd,yd,hd;//四个点的三维坐标；
	xa = inPoint1.x;
	ya = inPoint1.y;
	ha = inPoint1.h;
	xb = inPoint2.x;
	yb = inPoint2.y;
	hb = inPoint2.h;
	xc = inPoint3.x;
	yc = inPoint3.y;
	hc = inPoint3.h;
	xd = inPoint4.x;
	yd = inPoint4.y;
	hd = inPoint4.h;

	//求消减后的矩阵；
	DOUBLE m11 = xb - xa;
	DOUBLE m12 = yb - ya;
	DOUBLE m13 = hb - ha;
	DOUBLE m21 = xc - xa;
	DOUBLE m22 = yc - ya;
	DOUBLE m23 = hc - ha;
	DOUBLE m31 = xd - xa;
	DOUBLE m32 = yd - ya;
	DOUBLE m33 = hd - ha;

	//求行列式；
	DOUBLE determinant = fabs ( m11 * (m22*m33 - m32*m23) -
		m21 * (m12*m33 - m32*m13) +
		m31 * (m12*m23 - m22*m13) );

	//返回体积；
    return determinant / 6;	
}

DOUBLE MyMath::PointLineDistance(My3DPoint point, My3DPoint linePt1, My3DPoint linePt2)
//点线距离;
{
	DOUBLE area = TriangleArea(point, linePt1, linePt2);
	DOUBLE edgelength = LineDistance(linePt1, linePt2);
	return area/edgelength * 2;
}

DOUBLE MyMath::PrismVol(My3DPoint upPoint1, My3DPoint upPoint2, 
		My3DPoint upPoint3,	My3DPoint downPoint1, 
		My3DPoint downPoint2, My3DPoint downPoint3)
//三棱柱体积；
{
	DOUBLE tetra1, tetra2, tetra3;//分割后的三个四面体体积；
	tetra1 = TetrahedronVol(downPoint1, downPoint2, 
		                    downPoint3, upPoint1);
	tetra2 = TetrahedronVol(upPoint1, upPoint2, 
		                    upPoint3, downPoint2);
	tetra3 = TetrahedronVol(upPoint1, upPoint3, 
		                    downPoint2, downPoint3);
	return tetra1 + tetra2 + tetra3;
}

BOOL  MyMath::GetBinAt(LONG inLong, INT inPos)
//输入一个整数，返回该数在inPos处的二进制值，inPos位置从右算起；
//由于此函数是为了用于证据理论的组合公式，故其性能不宜被普通的转换二进
//制函数调用，如需进行该种转换需要另写转换函数；
{
	LONG backin1 = inLong;
	LONG backin2 = backin1;

	for (int i=0; i<=inPos; i++)
	{
		backin1 = backin2;
		backin2 = backin1 / 2;	
	}

	if ( backin2*2 != backin1 )
	{
		return 1;
	}else
	{
		return 0;
	}
}

DOUBLE MyMath::Gaussian(DOUBLE inputx, DOUBLE mean, DOUBLE diff)
//计算高斯函数值；
{
	return exp( -1.0 * (inputx - mean) * (inputx - mean) / 
		( 2 * diff * diff) );
}

DOUBLE MyMath::GetARandom()
//得到一个介于0-1之间的双精度随机数；
{
	return  ( (DOUBLE)rand() / (DOUBLE)RAND_MAX );
}

void MyMath::RevertCopyMatrix(BYTE* inMatrix, INT width, BYTE* outMatrix, INT sita, INT mode)
//sita 0-4对应0,45,90和135，mode 0对应上-下，左-右
//mode 1对应下-上,右-左；输入必须为奇数阶方阵；
{
	INT pos = -1;
	INT replacepos = -1;
	if (sita==0)
	{
		if (mode==0)
		{
			//0度上-下
			INT tempi = INT ( width/2 );
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					if (y>tempi)
					{						
						replacepos = (width-y)*width + x;
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}else
		{
			//0度下-上
			INT tempi = INT ( width/2 );
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					if (y<tempi)
					{						
						replacepos = (width-y)*width + x;
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}
	}

	if (sita==1)
	{
		//45度
		if (mode==0)
		{
			//45度左-右
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					INT tempoff = abs( y - (width-x) );
					if (y>(width-x))
					{						
						replacepos = (y-tempoff)*width + (x-tempoff);
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}else
		{
			//45度右-左
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					INT tempoff = abs( y - (width-x) );
					if (y<(width-x))
					{						
						replacepos = (y+tempoff)*width + (x+tempoff);
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}
	}

	if (sita==2)
	{
		//90度
		if (mode==0)
		{
			//90度左-右
			INT tempi = INT ( width/2 );
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					if (x>tempi)
					{						
						replacepos = y*width + (width-x);
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}else
		{
			//90度右-左
			INT tempi = INT ( width/2 );
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					if (x<tempi)
					{						
						replacepos = y*width + (width-x);
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}
	}

	if (sita==3)
	{
		//135度
		if (mode==0)
		{
			//135度左-右
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					if (y>x)
					{
						//XY翻转；
						replacepos = x*width + y;
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}else
		{
			//135度右-左
			for (INT y=0; y<width; y++)
			{
				for (INT x=0; x<width; x++)
				{
					pos = y*width + x;
					if (y<x)
					{
						//XY翻转；
						replacepos = x*width + y;
						outMatrix[pos] = inMatrix[replacepos];
					}else
					{
						outMatrix[pos] = inMatrix[pos];
					}
				}
			}
		}
	}
}

FLOAT MyMath::CalcuMatrixMean(FLOAT* inMatrix, INT width, INT height)
//计算输入矩阵的平均值；
{
	INT count = width * height;
	FLOAT added = 0;

	for (INT pos=0; pos<count; pos++)
	{
		added += inMatrix[pos];
	}

	return added / (FLOAT)count;
}

INT MyMath::FindMaxInThree(DOUBLE inval1, DOUBLE inval2, DOUBLE inval3)
//找三个数中最大者，返回为最大者的序号，从0开始编号；
{
	if (inval1>inval2)
	{
		if (inval1>inval3)
		{
			return 0;
		}else
		{
			return 2;
		}
	}else
	{
		if (inval2>inval3)
		{
			return 1;
		}else
		{
			return 2;
		}
	}
}

DOUBLE MyMath::ReturnMinInThree(DOUBLE inval1, DOUBLE inval2, DOUBLE inval3)
//找三个数中最小者，并将其返回；
{
	if (inval1<inval2)
	{
		if (inval1<inval3)
		{
			return inval1;
		}else
		{
			return inval3;
		}
	}else
	{
		if (inval2<inval3)
		{
			return inval2;
		}else
		{
			return inval3;
		}
	}
}

DOUBLE MyMath::ReturnMaxInThree(DOUBLE inval1, DOUBLE inval2, DOUBLE inval3)
//找三个数中最大者，并将其返回；
{
	if (inval1>inval2)
	{
		if (inval1>inval3)
		{
			return inval1;
		}else
		{
			return inval3;
		}
	}else
	{
		if (inval2>inval3)
		{
			return inval2;
		}else
		{
			return inval3;
		}
	}
}

void MyMath::ClacuMean(DOUBLE* inData, LONG inNumber, FLOAT& outMean, FLOAT& outSerr)
//计算输入数组的均值与方差；
{
	DOUBLE total = 0;
	//计算均值；
	for (INT i=0; i<inNumber; i++)
	{
		total += inData[i];
	}
	outMean = (FLOAT) ( total / (FLOAT)inNumber );

	//计算方差；
	total = 0;
    int i;
	for (i=0; i<inNumber; i++)
	{
		total += (inData[i] - outMean) * (inData[i] - outMean);
	}
	total = total / (DOUBLE)inNumber;//误差平方和的期望；
	outSerr = (FLOAT) ( sqrt(total) );//标准差；
}

void MyMath::ClacuMeanPositive(FLOAT* inData, LONG inNumber, FLOAT& outMean, FLOAT& outSerr)
//计算输入数组的均值与方差,只计算正值元素；
{
	FLOAT total = 0;
	FLOAT count = 0;
	//计算均值；
	for (INT i=0; i<inNumber; i++)
	{
		if (inData[i]>0)
		{
			total += inData[i];
			count++;
		}
		
	}

	outMean = (FLOAT) ( total / (FLOAT)count );

	//计算方差；
	total = 0;
    int i;
	for (i=0; i<inNumber; i++)
	{
		if (inData[i]>0)
		{
			total += (inData[i] - outMean) 
				* (inData[i] - outMean);
		}		
	}

	total = (FLOAT) ( total / (FLOAT)count );//误差平方和的期望；
	outSerr = (FLOAT) ( sqrt(total) );//标准差；
}

BOOL MyMath::isOdd(INT inInt)
//是否奇数；
{
	INT tempi = (INT) (inInt/2);
	if (tempi*2 == inInt)
	{
		//偶数；
		return FALSE;
	}

	//奇数；
	return TRUE;
}

/*******************************************************************************
* 函数名称：GetMaxValue()
*
* 参数：    BYTE* pWinValue    -- 指向数组的指针
*           int nWinLength     -- 数组的长度
*
* 返回值：  BYTE               -- 数组的最大值
*
* 说明：    该函数用于得到一个数组的最大值，由函数 LocalThresholding() 调用。
********************************************************************************/

BYTE MyMath::GetMaxValue(BYTE* pWinValue,int nWinLength)
{
	int i;    // 循环变量
	BYTE Tmp = 0; // 临时变量
    for(i=0;i<nWinLength;i++)
	{
		if(pWinValue[i] > Tmp)
		{
			Tmp = pWinValue[i];
		}
	}
	return Tmp;
}

/*******************************************************************************
* 函数名称：GetMinValue()
*
* 参数：    BYTE* pWinValue    -- 指向数组的指针
*           int nWinLength     -- 数组的长度
*
* 返回值：  BYTE               -- 数组的最小值
*
* 说明：    该函数用于得到一个数组的最小值，由函数 LocalThresholding() 调用。
********************************************************************************/

BYTE MyMath::GetMinValue(BYTE* pWinValue,int nWinLength)
{
	int i;    // 循环变量
	BYTE Tmp = 255; // 临时变量
    for(i=0;i<nWinLength;i++)
	{
		if(pWinValue[i] < Tmp)
		{
			Tmp = pWinValue[i];
		}
	}

	return Tmp;
}

/*******************************************************************************
* 函数名称：GetAveValue()
*
* 参数：    BYTE* pWinValue    -- 指向数组的指针
*           int nWinLength     -- 数组的长度
*
* 返回值：  BYTE               -- 数组的均值
*
* 说明：    该函数用于得到一个数组内非0数的均值，由函数 LocalThresholding() 调用。
********************************************************************************/

BYTE MyMath::GetAveValue(BYTE* pWinValue,int nWinLength)
{
	int i;    // 循环变量
	int nNonZeroNum = 0;  // 非0的数目
	int nTotal = 0;       // 临时变量，数值之和
	BYTE Average;
	double dAverage;
    for(i=0;i<nWinLength;i++)
	{
		if(pWinValue[i] != 0)
		{
			nTotal += pWinValue[i];
			nNonZeroNum++;
		}
	}
	dAverage = (double)nTotal/nNonZeroNum;
	if(dAverage - (int)dAverage != 0.0)
	{
		Average = (int)dAverage + 1;
	}
	else
	{
		Average = (int)dAverage;
	}

	return Average;
}

void MyMath::QickSort(FLOAT* inArr, LONG inNum)
//快速排序；
{
	qsort( inArr, inNum, sizeof(FLOAT), CompareFloat );
}

void MyMath::QickSort(MyImageGraPt* inArr, LONG inNum)
//按梯度值及空间顺序对各点排序；
{
    qsort( inArr, inNum, sizeof(MyImageGraPt), CompareGraPt );
}

void MyMath::QickSortInver(MyImageGraPt* inArr, LONG inNum)
//按梯度值及空间逆序对各点排序；
{
    qsort( inArr, inNum, sizeof(MyImageGraPt), CompareGraPtInver );
}

int CompareFloat( const void* in1, const void* in2 )
//比较；
{
	FLOAT tempf = *((FLOAT*)in1) - *((FLOAT*)in2);

	if (tempf==0)
	{
		return 0;
	}	
	return (INT) (tempf/fabs(tempf));
}

int CompareGraPt( const void* in1, const void* in2 )
//比较梯度；
{
	MyImageGraPt pt1, pt2;
	pt1 = *((MyImageGraPt*)in1);
	pt2 = *((MyImageGraPt*)in2);
	FLOAT tempf = (FLOAT) ( pt1.gradient - pt2.gradient );
	FLOAT tempx = (FLOAT) ( pt1.x - pt2.x );
	FLOAT tempy = (FLOAT) ( pt1.y - pt2.y );

	//如果梯度值一样，则按照空间位置排序；
	if (tempf==0)
	{
		if (tempy==0)
		{
			if (tempx==0)
			{
				return 0;
			}else
			{
				tempf = tempx;
			}
		}else
		{
			tempf = tempy;
		}
	}
	
	INT tempi = (INT) (tempf/fabs(tempf));
	return tempi;
}

int CompareGraPtInver( const void* in1, const void* in2 )
//比较梯度；
{
	MyImageGraPt pt1, pt2;
	pt1 = *((MyImageGraPt*)in1);
	pt2 = *((MyImageGraPt*)in2);
	FLOAT tempf = (FLOAT) ( pt1.gradient - pt2.gradient );
	FLOAT tempx = (FLOAT) ( pt1.x - pt2.x );
	FLOAT tempy = (FLOAT) ( pt1.y - pt2.y );

	//如果梯度值一样，则按照空间位置反向排序；
	if (tempf==0)
	{
		if (tempx==0)
		{
			if (tempy==0)
			{
				return 0;
			}else
			{
				tempf = -tempy;//空间位置反向排序；
			}
		}else
		{
			tempf = -tempx;//空间位置反向排序；
		}
	}

	INT tempi = (INT) (tempf/fabs(tempf));
	return tempi;
}

void MyMath::GetNeiInt(INT posx, INT posy, INT currentpos, INT width, INT height
		, INT& left, INT& right, INT& up, INT& down)
//在矩阵中找pos的四个邻域；
{
	//INT curpos = posy*width + posx;
	if ( posx-1 >= 0 )
	{
		left = currentpos - 1;
	}else
	{
		left = -1;
	}

	if ( posx+1 < width)
	{
		right = currentpos + 1;
	}else
	{
		right = -1;
	}

	if ( posy-1 >= 0 )
	{
		up = currentpos - width;
	}else
	{
		up = -1;
	}

	if ( posy+1 < height )
	{
		down = currentpos + width;
	}else
	{
		down = -1;
	}
}

