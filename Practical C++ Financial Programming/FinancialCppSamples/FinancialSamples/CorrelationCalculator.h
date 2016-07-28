//
//  CorrelationCalculator.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__CorrelationCalculator__
#define __FinancialSamples__CorrelationCalculator__

class TimeSeries;

class CorrelationCalculator
{
public:
    CorrelationCalculator(TimeSeries &a, TimeSeries &b);
    ~CorrelationCalculator();
    CorrelationCalculator(const CorrelationCalculator &);
    CorrelationCalculator &operator =(const CorrelationCalculator &);

    double correlation();
private:
    TimeSeries &m_tsA;
    TimeSeries &m_tsB;
};

#endif /* defined(__FinancialSamples__CorrelationCalculator__) */
