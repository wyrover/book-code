//
//  VolatilityCalculator.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "VolatilityCalculator.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

VolatilityCalculator::VolatilityCalculator()
{

}


VolatilityCalculator::~VolatilityCalculator()
{

}

VolatilityCalculator::VolatilityCalculator(const VolatilityCalculator &v)
: m_prices(v.m_prices)
{

}

VolatilityCalculator &VolatilityCalculator::operator =(const VolatilityCalculator &v)
{
    if (&v != this)
    {
        m_prices = v.m_prices;
    }
    return *this;
}

void VolatilityCalculator::addPrice(double price)
{
    m_prices.push_back(price);
}

double VolatilityCalculator::rangeVolatility()
{
    if (m_prices.size() < 1)
    {
        return 0;
    }

    double min = m_prices[0];
    double max = min;
    for (int i=1; i<m_prices.size(); ++i)
    {
        if (m_prices[i] < min)
        {
            min = m_prices[i];
        }
        if (m_prices[i] > max)
        {
            max = m_prices[i];
        }
    }
    return max - min;
}

double VolatilityCalculator::avgDailyRange()
{
    unsigned long n = m_prices.size();
    if (n < 2)
    {
        return 0;
    }

    double previous = m_prices[0];
    double sum = 0;
    for (int i=1; i<m_prices.size(); ++i)
    {
        double range = abs(m_prices[i] - previous);
        sum += range;
    }
    return sum / n - 1;
}

double VolatilityCalculator::mean()
{
    double sum = 0;
    for (unsigned i=0; i<m_prices.size(); ++i)
    {
        sum += m_prices[i];
    }
    return sum/m_prices.size();
}

double VolatilityCalculator::stdDev()
{
    double m = mean();
    double sum = 0;
    for (unsigned i=0; i<m_prices.size(); ++i)
    {
        double val = m_prices[i] - m;
        sum += val * val;
    }
    return sqrt(sum / (m_prices.size()-1));
}

