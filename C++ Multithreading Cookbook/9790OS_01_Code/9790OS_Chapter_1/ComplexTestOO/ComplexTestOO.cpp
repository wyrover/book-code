#include "stdafx.h"
#include <iostream>

using namespace std;

double Rand(double dMin, double dMax)
{
    double dVal = (double)rand() / RAND_MAX;
    return dMin + dVal * (dMax - dMin);
}

class CComplex
{
public:
    CComplex()
    {
        dReal = Rand(-10, 10);
        dImg = Rand(-10, 10);
    }
    CComplex(double dReal, double dImg)
    {
        this->dReal = dReal;
        this->dImg = dImg;
    }
    friend CComplex operator+ (const CComplex& c1,
                               const CComplex& c2);
    friend ostream& operator<< (ostream& os,
                                const CComplex& c);
private:
    double dReal;
    double dImg;
};

CComplex operator+ (const CComplex& c1, const CComplex& c2)
{
    CComplex c(c1.dReal + c2.dReal, c1.dImg + c2.dImg);
    return c;
}

ostream& operator<< (ostream& os, const CComplex& c)
{
    return os << c.dReal << "+" << c.dImg << "i";
}

int _tmain(int argc, TCHAR* argv[])
{
    CComplex c1;
    CComplex c2(-2.3, 0.9);
    CComplex c = c1 + c2;
    cout << c << endl;
    return 0;
}
