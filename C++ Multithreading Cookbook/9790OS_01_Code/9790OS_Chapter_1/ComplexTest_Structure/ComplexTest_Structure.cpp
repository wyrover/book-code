#include "stdafx.h"
#include <iostream>

using namespace std;

struct SComplex {
    double dReal;
    double dImg;
};

SComplex ComplexAdd(SComplex c1, SComplex c2)
{
    SComplex c;
    c.dReal = c1.dReal + c2.dReal;
    c.dImg = c1.dImg + c2.dImg;
    return c;
}

double Rand(double dMin, double dMax)
{
    double dVal = (double)rand() / RAND_MAX;
    return dMin + dVal * (dMax - dMin);
}

int _tmain(int argc, TCHAR* argv[])
{
    SComplex c1;
    c1.dReal = Rand(-10, 10);
    c1.dImg = Rand(-10, 10);
    SComplex c2;
    c2.dReal = Rand(-10, 10);
    c2.dImg = Rand(-10, 10);
    SComplex c = ComplexAdd(c1, c2);
    cout << c.dReal << "+" << c.dImg << "i" << endl;
    return 0;
}
