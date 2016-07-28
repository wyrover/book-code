//
//  LinearInterpolation.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "LinearInterpolation.h"

#include <iostream>
#include <algorithm>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

LinearInterpolation::LinearInterpolation()
: m_x(),
  m_y()
{
}

LinearInterpolation::LinearInterpolation(const LinearInterpolation &p)
: m_x(p.m_x),
  m_y(p.m_y)
{
}

LinearInterpolation::~LinearInterpolation()
{
}

LinearInterpolation &LinearInterpolation::operator=(const LinearInterpolation &p)
{
    if (this != &p)
    {
        m_x = p.m_x;
        m_y = p.m_y;
    }
    return *this;
}

void LinearInterpolation::setPoints(const std::vector<double> &xpoints,
                                    const std::vector<double> &ypoints)
{
    m_x = xpoints;
    m_y = ypoints;

    // update points to become sorted on x axis.
    std::sort(m_x.begin(), m_x.end());
    for (int i=0; i<m_x.size(); ++i)
    {
        for (int j=0; j<m_x.size(); ++j)
        {
            if (m_x[i] == xpoints[j])
            {
                m_y[i] = ypoints[j];
                break;
            }
        }
    }

}

double LinearInterpolation::getValue(double x)
{

    double x0=0, y0=0, x1=0, y1=0;
    if (x < m_x[0] || x > m_x[m_x.size()-1])
    {
        cout << " the value " << x << "is outside the domain "
             << m_x[0] << "," << m_x[m_x.size()-1] << endl;
        return 0; // outside of domain
    }

    for (int i=0; i<m_x.size(); ++i)
    {
        if (m_x[i] < x)
        {
            x0 = m_x[i];
            y0 = m_y[i];
        }
        else if (m_x[i] >= x)
        {
            x1 = m_x[i];
            y1 = m_y[i];
            break;
        }
    }

    return y0 * (x-x1)/(x0-x1) +  y1 * (x-x0)/(x1-x0);
}

int main_linear_interp()
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
    LinearInterpolation li;
    li.setPoints(xvals, yvals);
    double y = li.getValue(x);
    cout << "interpolation result for value " << x << " is " << y << endl;
    return 0;
}
