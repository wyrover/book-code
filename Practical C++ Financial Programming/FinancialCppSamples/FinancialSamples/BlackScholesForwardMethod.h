//
//  BlackScholesFowardMethod.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#ifndef __FinancialSamples__BlackScholesFowardMethod__
#define __FinancialSamples__BlackScholesFowardMethod__

#include <vector>

class BlackScholesForwardMethod {
public:
    BlackScholesForwardMethod(double expiration, double maxPrice, double strike, double intRate);
    BlackScholesForwardMethod(const BlackScholesForwardMethod &p);
    ~BlackScholesForwardMethod();
    BlackScholesForwardMethod &operator=(const BlackScholesForwardMethod &p);

    std::vector<double> solve(double volatility, int nx, int timeSteps);
private:
    double m_expiration;
    double m_maxPrice;
    double m_strike;
    double m_intRate;
};


#endif /* defined(__FinancialSamples__BlackScholesFowardMethod__) */
