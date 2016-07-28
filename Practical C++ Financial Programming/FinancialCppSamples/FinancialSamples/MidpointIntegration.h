//
//  MidpointIntegration.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__MidpointIntegration__
#define __FinancialSamples__MidpointIntegration__

template <class T>
class MathFunction;

class MidpointIntegration {
public:
    MidpointIntegration(MathFunction<double> &f);
    MidpointIntegration(const MidpointIntegration &p);
    ~MidpointIntegration();
    MidpointIntegration &operator=(const MidpointIntegration &p);

    void setNumIntervals(int n);
    double getIntegral(double a, double b);

private:
    MathFunction<double> &m_f;
    int m_numIntervals;

};
#endif /* defined(__FinancialSamples__MidpointIntegration__) */
