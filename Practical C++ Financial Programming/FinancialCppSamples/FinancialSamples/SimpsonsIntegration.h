//
//  SimpsonsIntegration.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__SimpsonsIntegration__
#define __FinancialSamples__SimpsonsIntegration__

template <class T>
class MathFunction;

class SimpsonsIntegration {
public:
    SimpsonsIntegration(MathFunction<double> &f);
    SimpsonsIntegration(const SimpsonsIntegration &p);
    ~SimpsonsIntegration();
    SimpsonsIntegration &operator=(const SimpsonsIntegration &p);

    double getIntegral(double a, double b);
    void setNumIntervals(int n);
private:
    MathFunction<double> &m_f;
    int m_numIntervals;
};

#endif /* defined(__FinancialSamples__SimpsonsIntegration__) */
