//
//  SimpsonsIntegration.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "SimpsonsIntegration.h"

#include "MathFunction.h"

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

namespace  {
    const int DEFAULT_NUM_INTERVALS = 100;
}

SimpsonsIntegration::SimpsonsIntegration(MathFunction<double> &f)
: m_f(f),
  m_numIntervals(DEFAULT_NUM_INTERVALS)
{
}

SimpsonsIntegration::SimpsonsIntegration(const SimpsonsIntegration &p)
: m_f(p.m_f),
  m_numIntervals(p.m_numIntervals)
{
}

SimpsonsIntegration::~SimpsonsIntegration()
{
}

SimpsonsIntegration &SimpsonsIntegration::operator=(const SimpsonsIntegration &p)
{
    if (this != &p)
    {
        m_f = p.m_f;
        m_numIntervals = p.m_numIntervals;
    }
    return *this;
}

double SimpsonsIntegration::getIntegral(double a, double b)
{
    double S = 0;
    double intSize = (b - a)/m_numIntervals;
    double x = a;

    for (int i=0; i<m_numIntervals; ++i)
    {
        S += (intSize / 6) * ( m_f(x) + m_f(x+intSize) + 4* m_f ((x + x+intSize)/2) );
        x += intSize;
    }
    return S;
}

void SimpsonsIntegration::setNumIntervals(int n)
{
    m_numIntervals = n;
}

// Example function

namespace  {

    class F2 : public MathFunction<double>
    {
    public:
        ~F2();
        double operator()(double x);
    };


    F2::~F2()
    {
    }

    double F2::operator()(double x)
    {
        return sin(x);
    }

}

int main_simp()
{
    F2 f;
	SimpsonsIntegration si(f);
    double integral = si.getIntegral(0.5, 2.5);
	cout << " the integral of the function is " << integral << endl;

    si.setNumIntervals(200);
    integral = si.getIntegral(0.5, 2.5);
	cout << " the integral of the function with 200 intervals is " << integral << endl;
    return 0;
}



