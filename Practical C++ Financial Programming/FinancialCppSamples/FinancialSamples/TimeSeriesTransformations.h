//
//  TimeSeriesTransformations.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__TimeSeriesAnalysis__
#define __FinancialSamples__TimeSeriesAnalysis__

#include <vector>

class TimeSeriesTransformations {
public:
    TimeSeriesTransformations();
    TimeSeriesTransformations(const TimeSeriesTransformations &);
    ~TimeSeriesTransformations();
    TimeSeriesTransformations &operator=(const TimeSeriesTransformations &);
    void reducePrices(double val);
    void increasePrices(double val);
    void removePricesLessThan(double val);
    void removePricesGreaterThan(double val);
    double getFirstPriceLessThan(double val);
    void addValue(double val);
    void addValues(const std::vector<double> &val);
private:
    std::vector<double> m_prices;
};

#endif /* defined(__FinancialSamples__TimeSeriesAnalysis__) */
