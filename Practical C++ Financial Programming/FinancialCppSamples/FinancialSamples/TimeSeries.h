//
//  TimeSeries.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__TimeSeries__
#define __FinancialSamples__TimeSeries__

#include <vector>
#include <stddef.h>

class TimeSeries
{
public:
    TimeSeries();
    TimeSeries(const TimeSeries &);
    TimeSeries &operator=(const TimeSeries &);
    ~TimeSeries();

    void addValue(double val);
    double stdDev();
    double mean();
    size_t size();
    double elem(int i);
private:
    std::vector<double> m_values;
};

#endif /* defined(__FinancialSamples__TimeSeries__) */
