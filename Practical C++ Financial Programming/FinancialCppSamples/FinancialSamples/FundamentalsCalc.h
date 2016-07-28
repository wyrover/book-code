//
//  FundamentalsCalc.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__FundamentalsCalc__
#define __FinancialSamples__FundamentalsCalc__

#include <string>

class FundamentalsCalculator {
public:
    FundamentalsCalculator(const std::string &ticker, double price, double dividend);
    ~FundamentalsCalculator();
    FundamentalsCalculator(const FundamentalsCalculator &);
    FundamentalsCalculator &operator=(const FundamentalsCalculator&);

    void setNumOfShares(int n);
    void setEarnings(double val);
    void setExpectedEarnings(double val);
    void setBookValue(double val);
    void setAssets(double val);
    void setLiabilitiesAndIntangibles(double val);
    void setEpsGrowth(double val);
    void setNetIncome(double val);
    void setShareHoldersEquity(double val);

    double PE();
    double forwardPE();
    double bookValue();
    double priceToBookRatio();
    double priceEarningsToGrowth();
    double returnOnEquity();
    double getDividend();

private:

    std::string m_ticker;
    double m_price;
    double m_dividend;
    double m_earningsEstimate;
    int m_numShares;
    double m_earnings;
    double m_bookValue;
    double m_assets;
    double m_liabilitiesAndIntangibles;
    double m_epsGrowth;
    double m_netIncome;
    double m_shareholdersEquity;
};

#endif /* defined(__FinancialSamples__FundamentalsCalc__) */
