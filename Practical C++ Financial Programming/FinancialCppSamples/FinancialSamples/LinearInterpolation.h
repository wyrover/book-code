//
//  LinearInterpolation.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__LinearInterpolation__
#define __FinancialSamples__LinearInterpolation__

#include <vector>

class LinearInterpolation {
public:
    LinearInterpolation();
    LinearInterpolation(const LinearInterpolation &p);
    ~LinearInterpolation();
    LinearInterpolation &operator=(const LinearInterpolation &p);
    void setPoints(const std::vector<double> &xpoints, const std::vector<double> &ypoints);
    double getValue(double x);
private:
    std::vector<double> m_x;
    std::vector<double> m_y;
};

#endif /* defined(__FinancialSamples__LinearInterpolation__) */
