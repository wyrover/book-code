// MyMath.h: interface for the MyMath class.
/*
   一些数学处理函数，始于WS3D的空间计算。

   by dzj;
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMATH_H__CFBFD7BB_A90A_43E8_8018_3CE1AAA3A24E__INCLUDED_)
#define AFX_MYMATH_H__CFBFD7BB_A90A_43E8_8018_3CE1AAA3A24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PI 3.14159265359


typedef struct tagMyImageGraPt{
	INT   x;//横向坐标；
	INT   y;//纵向坐标；
	INT   gradient;//该点梯度(去掉小数点后的位数，避免因为极小差异导致排序错误)；
}MyImageGraPt;//用于分水岭分割时记录各点位置及相应梯度等信息，为使MyMath独立，不得不将其从DOC移至此处，因为要对其进行比较排序；

struct  My3DPoint//三维空间点
{
	DOUBLE x;//X坐标;
	DOUBLE y;//y坐标;
	DOUBLE h;//Z坐标;
};

int CompareFloat( const void* in1, const void* in2 );//比较浮点值；
int CompareGraPt( const void* in1, const void* in2 );//比较梯度；
int CompareGraPtInver( const void* in1, const void* in2 );//比较梯度,空间逆序；


class MyMath  
{
public:
	MyMath();
	virtual ~MyMath();
    void GetNeiInt(INT posx, INT posy, INT currentpos, INT width
		, INT height, INT& left, INT& right, INT& up, INT& down);//在矩阵中找pos的四个邻域；
	void QickSort(FLOAT* inArr, LONG inNum);//快速排序；
	void QickSort(MyImageGraPt* inArr, LONG inNum);//按梯度值空间顺序对各点排序；
	void QickSortInver(MyImageGraPt* inArr, LONG inNum);//按梯度值空间逆序对各点排序；
    BYTE GetMaxValue(BYTE* pWinValue,int nWinLength);
	BYTE GetMinValue(BYTE* pWinValue,int nWinLength);
    BYTE GetAveValue(BYTE* pWinValue,int nWinLength);
	BOOL isOdd(INT inInt);//是否奇数；
    FLOAT CalcuMatrixMean(FLOAT* inMatrix, INT width, INT height);//计算输入矩阵的平均值；
	void ClacuMean(DOUBLE* inData, LONG inNumber, FLOAT& outMean, FLOAT& outSerr);//计算输入数组的均值与方差；
    void ClacuMeanPositive(FLOAT* inData, LONG inNumber, FLOAT& outMean, FLOAT& outSerr);//只计算正值元素的均值与方差；
	void RevertCopyMatrix(BYTE* inMatrix, INT width, BYTE* outMatrix, INT sita, INT mode);//sita 0-4对应0,45,90和135，mode 0对应上-下，左-右，mode 1对应下-上,右-左,输入输出数组不能相同；
	INT FindMaxInThree(DOUBLE inval1, DOUBLE inval2, DOUBLE inval3);//找三个数中最大者，返回为最大者的序号，从0开始编号；
	DOUBLE ReturnMinInThree(DOUBLE inval1, DOUBLE inval2, DOUBLE inval3);//找三个数中最小者，并将其返回；
	DOUBLE ReturnMaxInThree(DOUBLE inval1, DOUBLE inval2, DOUBLE inval3);//找三个数中最大者，并将其返回；

    DOUBLE GetARandom();//得到一个0-1之间的随机数；
	DOUBLE PointLineDistance(My3DPoint point, My3DPoint linePt1, My3DPoint linePt2);//点线
	DOUBLE LineDistance(My3DPoint inPoint1, My3DPoint inPoint2);//直线距离；
	DOUBLE TriangleArea(My3DPoint inPoint1, My3DPoint inPoint2, My3DPoint inPoint3);//三角形面积；
	DOUBLE TetrahedronVol(My3DPoint inPoint1,
		My3DPoint inPoint2, My3DPoint inPoint3,
		My3DPoint inPoint4);//四面体体积；
	DOUBLE PrismVol(My3DPoint upPoint1, My3DPoint upPoint2, 
		My3DPoint upPoint3,	My3DPoint downPoint1, 
		My3DPoint downPoint2, My3DPoint downPoint3);//三棱柱体积；
	BOOL  GetBinAt(LONG inLong, INT inPos);//输入一个整数，返回该数在inPos处的二进制值，inPos位置从右算起；
	DOUBLE Gaussian(DOUBLE inputx, DOUBLE mean, DOUBLE diff);//计算高斯函数值；

};

#endif // !defined(AFX_MYMATH_H__CFBFD7BB_A90A_43E8_8018_3CE1AAA3A24E__INCLUDED_)
