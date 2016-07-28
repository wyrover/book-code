//
//  BondCalculator.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__BondCalculator__
#define __FinancialSamples__BondCalculator__

#include <string>

class BondCalculator {
public:
    BondCalculator(const std::string institution, int numPeriods, double principal, double couponValue);
    BondCalculator(const BondCalculator &v);
    BondCalculator &operator =(const BondCalculator &v);
    ~BondCalculator();

    double interestRate();

private:
    std::string m_institution;
    double m_principal;
    double m_coupon;
    int m_numPeriods;
};

#endif /* defined(__FinancialSamples__BondCalculator__) */
