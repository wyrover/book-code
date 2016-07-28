//
//  Stock.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Stock__
#define __FinancialSamples__Stock__

#include <string>

class Stock {
public:
    Stock(const std::string &ticker, double price, double div);
    Stock(const Stock &p);
    ~Stock();
    Stock &operator=(const Stock &p);

    std::string ticker();
    double price();
    void setPrice(double price);
    double dividend();
    void setDividend(double div);
    double dividendYield();

private:
    std::string m_ticker;
    double m_currentPrice;
    double m_dividend;
};

#endif /* defined(__FinancialSamples__Stock__) */
