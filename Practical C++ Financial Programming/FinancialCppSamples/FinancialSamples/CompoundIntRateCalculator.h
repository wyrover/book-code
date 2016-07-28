//
//  CompoundIntRateCalculator.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__CompoundIntRateCalculator__
#define __FinancialSamples__CompoundIntRateCalculator__


class CompoundIntRateCalculator {
public:
    CompoundIntRateCalculator(double rate);
    CompoundIntRateCalculator(const CompoundIntRateCalculator &v);
    CompoundIntRateCalculator &operator =(const CompoundIntRateCalculator &v);
    ~CompoundIntRateCalculator();

    double multiplePeriod(double value, int numPeriods);
    double continuousCompounding(double value, int numPeriods);
private:
    double m_rate;
};

#endif /* defined(__FinancialSamples__CompoundIntRateCalculator__) */
