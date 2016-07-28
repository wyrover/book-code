//
//  TrapezoidIntegration.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "TrapezoidIntegration.h"

#include "MathFunction.h"

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

namespace  {
    const int DEFAULT_NUM_INTERVALS = 100;
}

TrapezoidIntegration::TrapezoidIntegration(MathFunction<double> &f)
: m_f(f),
  m_numIntervals(DEFAULT_NUM_INTERVALS)
{
}

TrapezoidIntegration::TrapezoidIntegration(const TrapezoidIntegration &p)
: m_f(p.m_f),
  m_numIntervals(p.m_numIntervals)
{
}

TrapezoidIntegration::~TrapezoidIntegration()
{
}

TrapezoidIntegration &TrapezoidIntegration::operator=(const TrapezoidIntegration &p)
{
    if (this != &p)
    {
        m_f = p.m_f;
        m_numIntervals = p.m_numIntervals;
    }
    return *this;
}

void TrapezoidIntegration::setNumIntervals(int n)
{
    m_numIntervals = n;
}

double TrapezoidIntegration::getIntegral(double a, double b)
{
    double S = 0;
    double intSize = (b - a)/m_numIntervals;
    double x = a;

    for (int i=0; i<m_numIntervals; ++i)
    {
        double midpt = (m_f(x) + m_f(x+intSize))/ 2;
        S += intSize * midpt;
        x += intSize;
    }
    return S;
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

int main_trapez()
{
    F2 f;
	TrapezoidIntegration ti(f);
    double integral = ti.getIntegral(0.5, 2.5);
	cout << " the integral of the function is " << integral << endl;

    ti.setNumIntervals(200);
    integral = ti.getIntegral(0.5, 2.5);
	cout << " the integral of the function with 200 intervals is " << integral << endl;
    return 0;
}


