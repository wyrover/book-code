//
//  InvestmentInstrument.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__InvestmentInstrument__
#define __FinancialSamples__InvestmentInstrument__

#include <iostream>

class BondInstrument {
public:
    BondInstrument() {}
    BondInstrument(double principal, double monthlyPayment);
    ~BondInstrument();
    BondInstrument(const BondInstrument &a);
    BondInstrument &operator =(const BondInstrument &a);

    double getMonthlyPayment();
    double getPrincipal();

    // other methods here...
private:
    double
        m_monthlyPay,
        m_principal;
};

class MortgageInstrument {
public:
    MortgageInstrument() {}
    MortgageInstrument(double monthlyPay, double propertyValue, double downpayment);
    ~MortgageInstrument();
    MortgageInstrument(const MortgageInstrument &a);
    MortgageInstrument &operator =(const MortgageInstrument &a);

    double getMonthlyPayment();
    double getPrincipal();

    // other methods here...
private:
    double
        m_monthlyPay,
        m_propertyValue,
        m_downPayment;
};

#endif /* defined(__FinancialSamples__InvestmentInstrument__) */
