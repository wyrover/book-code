//
//  EulersMethod.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__EulersMethod__
#define __FinancialSamples__EulersMethod__

template <class T>
class MathFunction;

class EulersMethod {
public:
    EulersMethod(MathFunction<double> &f);
    EulersMethod(const EulersMethod &p);
    ~EulersMethod();
    EulersMethod &operator=(const EulersMethod &p);

    double solve(int n, double x0, double y0, double c);
private:
    MathFunction<double> &m_f;
};

#endif /* defined(__FinancialSamples__EulersMethod__) */
