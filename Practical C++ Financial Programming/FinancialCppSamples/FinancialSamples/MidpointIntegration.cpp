//
//  MidpointIntegration.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "MidpointIntegration.h"

#include "MathFunction.h"

#include <iostream>

using std::cout;
using std::endl;

namespace  {
    const int DEFAULT_NUM_INTERVALS = 100;
}

MidpointIntegration::MidpointIntegration(MathFunction<double> &f)
: m_f(f),
  m_numIntervals(DEFAULT_NUM_INTERVALS)
{
}

MidpointIntegration::MidpointIntegration(const MidpointIntegration &p)
: m_f(p.m_f),
  m_numIntervals(p.m_numIntervals)
{
}

MidpointIntegration::~MidpointIntegration()
{
}

MidpointIntegration &MidpointIntegration::operator=(const MidpointIntegration &p)
{
    if (this != &p)
    {
        m_f = p.m_f;
        m_numIntervals = p.m_numIntervals;
    }
    return *this;
}

void MidpointIntegration::setNumIntervals(int n)
{
    m_numIntervals = n;
}

double MidpointIntegration::getIntegral(double a, double b)
{
    double S = 0;
    double intSize = (b - a)/m_numIntervals;
    double x = a;

    for (int i=0; i<m_numIntervals; ++i)
    {
        double midpt = m_f(x+(intSize/2));
        S += intSize * midpt;
        x += intSize;
    }
    return S;
}

// Example function

namespace  {

class F1 : public MathFunction<double>
{
public:
    ~F1();
    double operator()(double x);
};


F1::~F1()
{
}

double F1::operator()(double x)
{
    return x*x+1;
}

}

int main_mpi()
{
    F1 f;
	MidpointIntegration mpi(f);
    double integral = mpi.getIntegral(1, 5);
	cout << " the integral of the function is " << integral << endl;

    mpi.setNumIntervals(200);
    integral = mpi.getIntegral(1, 5);
	cout << " the integral of the function with 200 intervals is " << integral << endl;
    return 0;
}