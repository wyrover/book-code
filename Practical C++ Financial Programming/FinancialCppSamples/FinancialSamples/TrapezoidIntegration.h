//
//  TrapezoidIntegration.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__TrapezoidIntegration__
#define __FinancialSamples__TrapezoidIntegration__

template <class T>
class MathFunction;

class TrapezoidIntegration {
public:
    TrapezoidIntegration(MathFunction<double> &f);
    TrapezoidIntegration(const TrapezoidIntegration &p);
    ~TrapezoidIntegration();
    TrapezoidIntegration &operator=(const TrapezoidIntegration &p);

    void setNumIntervals(int n);
    double getIntegral(double a, double b);

private:
    MathFunction<double> &m_f;
    int m_numIntervals;

};

#endif /* defined(__FinancialSamples__TrapezoidIntegration__) */
