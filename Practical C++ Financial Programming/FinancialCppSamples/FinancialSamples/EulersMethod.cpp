//
//  EulersMethod.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "EulersMethod.h"

#include "MathFunction.h"

#include <iostream>

using std::cout;
using std::endl;


EulersMethod::EulersMethod(MathFunction<double> &f)
: m_f(f)
{
}

EulersMethod::EulersMethod(const EulersMethod &p)
: m_f(p.m_f)
{
}

EulersMethod::~EulersMethod()
{
}

EulersMethod &EulersMethod::operator=(const EulersMethod &p)
{
    if (this != &p)
    {
        m_f = p.m_f;
    }
    return *this;
}

double EulersMethod::solve(int n, double x0, double y0, double c)
{
    // problem :   y' = f(x,y) ;  y(x0) = y0

    auto x = x0;
    auto y = y0;
    auto h = (c - x0)/n;

    cout << " h is " << h << endl;

    for (int i=0; i<n; ++i)
    {
        double F = m_f(x, y);
        auto G = m_f(x + h, y + h*F);

        cout << " F: " << F << " G: " << G << "";

        // update values of x, y
        x += h;
        y += h * (F + G)/2;

        cout << " x: " << x << " y: " << y << endl;
}


    return y;
}

/// -----

class EulerMethSampleFunc : public MathFunction<double> {
public:
    double operator()(double x) { return x; } // not used
    double operator()(double x, double y);
};

double EulerMethSampleFunc::operator()(double x, double y)
{
    return  3 * x + 2 * y + 1;
}

int main_euler()
{
    EulerMethSampleFunc f;
    EulersMethod m(f);
    double res = m.solve (100, 0, 0.25, 2);
    cout << " result is " << res << endl;
    return 0;
}