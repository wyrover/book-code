//
//  PolymonialInterpolation.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "PolymonialInterpolation.h"

#include <algorithm>
#include <iostream>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

PolynomialInterpolation::PolynomialInterpolation()
: m_x(),
  m_y()
{
}

PolynomialInterpolation::PolynomialInterpolation(const PolynomialInterpolation &p)
: m_y(p.m_y),
  m_x(p.m_x)
{
}

PolynomialInterpolation::~PolynomialInterpolation()
{
}

PolynomialInterpolation &PolynomialInterpolation::operator=(const PolynomialInterpolation &p)
{
    if (this != &p)
    {
        m_x = p.m_x;
        m_y = p.m_y;
    }
    return *this;
}

void PolynomialInterpolation::setPoints(const std::vector<double> &x, const std::vector<double> &y)
{
    m_x = x;
    m_y = y;

    // update points to become sorted on x axis.
    std::sort(m_x.begin(), m_x.end());
    for (int i=0; i<m_x.size(); ++i)
    {
        for (int j=0; j<m_x.size(); ++j)
        {
            if (m_x[i] == x[j])
            {
                m_y[i] = y[j];
                break;
            }
        }
    }
}

double PolynomialInterpolation::getPolynomial(double x)
{
    double polynomialValue = 0;

    for (size_t i=0; i<m_x.size(); ++i)
    {

        // compute the numerator
        double num = 1;
        for (size_t j=0; j<m_x.size(); ++j)
        {
            if (j!=i)
            {
                num *= x - m_x[j];
            }
        }

        // compute the denominator
        double den = 1;
        for (size_t j=0; j<m_x.size(); ++j)
        {
            if (j!=i)
            {
                den *= m_x[i] - m_x[j];
            }
        }

        // value for i-th term
        polynomialValue += m_y[i] * (num/den);
    }

    return polynomialValue;
}

int main_polyint()
{
    double xi = 0;
    double yi = 0;
    vector<double> xvals;
    vector<double> yvals;
    while (cin >> xi)
    {
        if (xi == -1)
        {
            break;
        }
        xvals.push_back(xi);
        cin >> yi;
        yvals.push_back(yi);
    }
    double x = 0;
    cin >> x;
    PolynomialInterpolation pi;
    pi.setPoints(xvals, yvals);
    double y = pi.getPolynomial(x);
    cout << "interpolation result for value " << x << " is " << y << endl;
//    for (double v=10; v < 50; v += 0.1)
//    {
//        cout << v << ", ";
//    }
//    cout << endl;
//    for (double v=10; v < 50; v += 0.1)
//    {
//        cout << pi.getValue(v)  << ", ";
//    }
    return 0;
}
