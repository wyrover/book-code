//
//  RungeKuttaODEMethod.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__RungeKuttaODEMethod__
#define __FinancialSamples__RungeKuttaODEMethod__

template <class T>
class MathFunction;

class RungeKuttaODEMethod {
public:
    RungeKuttaODEMethod(MathFunction<double> &f);
    RungeKuttaODEMethod(const RungeKuttaODEMethod &p);
    ~RungeKuttaODEMethod();
    RungeKuttaODEMethod &operator=(const RungeKuttaODEMethod &p);
    double solve(int n, double x0, double y0, double c);
private:
    MathFunction<double> &m_f;
};

#endif /* defined(__FinancialSamples__RungeKuttaODEMethod__) */
