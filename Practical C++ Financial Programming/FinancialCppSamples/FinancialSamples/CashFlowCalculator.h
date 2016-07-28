//
//  CashFlowCalculator.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__CashFlowCalculator__
#define __FinancialSamples__CashFlowCalculator__

#include <vector>

class CashFlowCalculator {
public:
    CashFlowCalculator(double rate);
    CashFlowCalculator(const CashFlowCalculator &v);
    CashFlowCalculator &operator =(const CashFlowCalculator &v);
    ~CashFlowCalculator();

    void addCashPayment(double value, int timePeriod);
    double presentValue();
private:
    std::vector<double> m_cashPayments;
    std::vector<int> m_timePeriods;
    double m_rate;
    double presentValue(double futureValue, int timePeriod);
};

#endif /* defined(__FinancialSamples__CashFlowCalculator__) */
