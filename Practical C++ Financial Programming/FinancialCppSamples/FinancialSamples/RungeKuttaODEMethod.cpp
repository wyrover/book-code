//
//  RungeKuttaODEMethod.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "RungeKuttaODEMethod.h"

#include "MathFunction.h"

#include <iostream>

using std::cout;
using std::endl;

RungeKuttaODEMethod::RungeKuttaODEMethod(MathFunction<double> &f)
: m_f(f)
{
}

RungeKuttaODEMethod::RungeKuttaODEMethod(const RungeKuttaODEMethod &p)
: m_f(p.m_f)
{
}

RungeKuttaODEMethod::~RungeKuttaODEMethod()
{
}

RungeKuttaODEMethod &RungeKuttaODEMethod::operator=(const RungeKuttaODEMethod &p)
{
    if (this != &p)
    {
        m_f = p.m_f;
    }
    return *this;
}

double RungeKuttaODEMethod::solve(int n, double x0, double y0, double c)
{
    auto x = x0;
    auto y = y0;
    auto h = (c - x0)/n;

    for (int i=0; i<n; ++i)
    {
        auto k1 = h * m_f(x, y);
        auto k2 = h * m_f(x + (h/2), y + (k1/2));
        auto k3 = h * m_f(x + (h/2), y + (k2/2));
        auto k4 = h * m_f(x + h, y + k3);

        x += h;
        y += ( k1 + 2*k2 + 2*k3 + k4)/6;
        cout << " x: " << x << " y: " << y << endl;
    }
    return y;
}

/// -----

class RKMethSampleFunc : public MathFunction<double> {
public:
    double operator()(double x) { return x; } // not used
    double operator()(double x, double y);
};

double RKMethSampleFunc::operator()(double x, double y)
{
    return  3 * x + 2 * y + 1;
}

int main_rkm()
{
    RKMethSampleFunc f;
    RungeKuttaODEMethod m(f);
    double res = m.solve (100, 0, 0.25, 2);
    cout << " result is " << res << endl;
    return 0;
}



