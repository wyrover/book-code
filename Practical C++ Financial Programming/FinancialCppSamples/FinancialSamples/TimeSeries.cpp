//
//  TimeSeries.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "TimeSeries.h"
#include <cmath>
#include <iostream>

TimeSeries::TimeSeries()
: m_values()
{

}

TimeSeries::~TimeSeries()
{

}

TimeSeries::TimeSeries(const TimeSeries &ts)
: m_values(ts.m_values)
{
}

TimeSeries &TimeSeries::operator =(const TimeSeries &ts)
{
    if (this != &ts)
    {
        m_values = ts.m_values;
    }
    return *this;
}

void TimeSeries::addValue(double val)
{
    m_values.push_back(val);
}

double TimeSeries::mean()
{
    double sum = 0;
    for (int i=0; i<m_values.size(); ++i)
    {
        sum += m_values[i];
    //std::cout << " val is " << m_values[i] << std::endl;
    }
    std::cout << " avg is " << sum / m_values.size() << std::endl;
    return sum/m_values.size();
}

double TimeSeries::stdDev()
{
    double m = mean();
    double sum = 0;
    for (int i=0; i<m_values.size(); ++i)
    {
        double val = m_values[i] - m;
        sum += val * val;
    }
    // std::cout << " var is " << sum / (m_values.size()-1)
    //           << " stdev is " << sqrt(sum / (m_values.size()-1)) << std::endl;
    return sqrt(sum / (m_values.size()-1));
}

size_t TimeSeries::size()
{     
    return m_values.size();
}

double TimeSeries::elem(int pos)
{
    return m_values[pos];
}

