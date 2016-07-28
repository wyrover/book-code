//
//  PolymonialInterpolation.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__PolymonialInterpolation__
#define __FinancialSamples__PolymonialInterpolation__

#include <vector>

class PolynomialInterpolation {
public:
    PolynomialInterpolation();
    PolynomialInterpolation(const PolynomialInterpolation &p);
    ~PolynomialInterpolation();
    PolynomialInterpolation &operator=(const PolynomialInterpolation &);
    void setPoints(const std::vector<double> &x, const std::vector<double> &y);
    double getPolynomial(double x);
private:
    std::vector<double> m_x;
    std::vector<double> m_y;
};

#endif /* defined(__FinancialSamples__PolymonialInterpolation__) */
