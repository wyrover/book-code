//
//  VolatilityCalculator.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__VolatilityCalculator__
#define __FinancialSamples__VolatilityCalculator__

#include <vector>

class VolatilityCalculator
{
public:
    VolatilityCalculator();
    ~VolatilityCalculator();
    VolatilityCalculator(const VolatilityCalculator &);
    VolatilityCalculator &operator=(const VolatilityCalculator &);

    void addPrice(double price);
    double rangeVolatility();
    double stdDev();
    double mean();
    double avgDailyRange();
private:
    std::vector<double> m_prices;
};

#endif /* defined(__FinancialSamples__VolatilityCalculator__) */
