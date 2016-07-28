//
//  BlackScholesForwardMethod.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#include "BlackScholesForwardMethod.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

using std::vector;
using std::cout;
using std::endl;
using std::setw;

BlackScholesForwardMethod::BlackScholesForwardMethod(double expiration, double maxPrice,
                                                   double strike, double intRate)
: m_expiration(expiration),
  m_maxPrice(maxPrice),
  m_strike(strike),
  m_intRate(intRate)
{
}

BlackScholesForwardMethod::BlackScholesForwardMethod(const BlackScholesForwardMethod &p)
: m_expiration(p.m_expiration),
  m_maxPrice(p.m_maxPrice),
  m_strike(p.m_strike),
  m_intRate(p.m_intRate)
{
}

BlackScholesForwardMethod::~BlackScholesForwardMethod()
{
}

BlackScholesForwardMethod &BlackScholesForwardMethod::operator=(const BlackScholesForwardMethod &p)
{
    if (this != &p)
    {
        m_expiration = p.m_expiration;
        m_maxPrice = p.m_maxPrice;
        m_strike = p.m_strike;
        m_intRate = p.m_intRate;
    }
    return *this;
}

vector<double> BlackScholesForwardMethod::solve(double volatility, int nx, int timeSteps)
{
    double dt = m_expiration /(double)timeSteps;
    double dx = m_maxPrice /(double)nx;

    vector<double> a(nx-1);
    vector<double> b(nx-1);
    vector<double> c(nx-1);

    int i;
    for (i = 0; i < nx - 1; i++)
    {
        b[i] = 1.0 - m_intRate * dt - dt * pow(volatility * (i+1), 2);
    }

    for (i = 0; i < nx - 2; i++)
    {
        c[i] = 0.5 * dt * pow(volatility * (i+1), 2) + 0.5 * dt * m_intRate * (i+1);
    }

    for (i = 1; i < nx - 1; i++)
    {
        a[i] = 0.5 * dt * pow(volatility * (i+1), 2) - 0.5 * dt * m_intRate * (i+1);
    }

    vector<double> u((nx-1)*(timeSteps+1));

    double u0 = 0.0;
    for (i = 0; i < nx - 1; i++)
    {
        u0 += dx;
        u[i+0*(nx-1)] = std::max(u0 - m_strike, 0.0);
    }

    for (int j = 0; j < timeSteps; j++)
    {
        double t = (double)(j) * m_expiration /(double)timeSteps;

        double p = 0.5 * dt * (nx - 1) * (volatility*volatility * (nx-1) + m_intRate)
        * (m_maxPrice-m_strike * exp(-m_intRate*t ) );

        for (i = 0; i < nx - 1; i++)
        {
            u[i+(j+1)*(nx-1)] = b[i] * u[i+j*(nx-1)];
        }
        for (i = 0; i < nx - 2; i++)
        {
            u[i+(j+1)*(nx-1)] += c[i] * u[i+1+j*(nx-1)];
        }
        for (i = 1; i < nx - 1; i++)
        {
            u[i+(j+1)*(nx-1)] += a[i] * u[i-1+j*(nx-1)];
        }
        u[nx-2+(j+1)*(nx-1)] += p;
    }

    return u;
}

int main_bsmeth()
{
    auto strike = 5.0;
    auto intRate = 0.03;
    auto sigma = 0.50;
    auto t1 = 1.0;
    auto numSteps = 11;
    auto numDays = 29;
    auto maxPrice = 10.0;


    BlackScholesForwardMethod bsfm(t1, maxPrice, strike, intRate);
    vector<double> u = bsfm.solve(sigma, numSteps, numDays);

    double minPrice = .0;
    for (int  i=0; i < numSteps-1; i++)
    {
        double s = ((numSteps-i-2) * minPrice+(i+1)*maxPrice)/ (double)(numSteps-1);
        cout << "  " << s << "  " << u[i+numDays*(numSteps-1)] << endl;
    }
    return 0;
}