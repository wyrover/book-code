//
//  TwoDimensionalLPSolver.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__TwoDimensionalLPSolver__
#define __FinancialSamples__TwoDimensionalLPSolver__

#include <vector>

class TwoDimensionalLPSolver {
public:
    typedef std::vector<double> Vector;

    TwoDimensionalLPSolver(const Vector &c, const Vector &A1, const Vector &A2, const Vector &b);
    TwoDimensionalLPSolver(const TwoDimensionalLPSolver &p);
    ~TwoDimensionalLPSolver();
    TwoDimensionalLPSolver &operator=(const TwoDimensionalLPSolver &p);

    bool solveProblem(Vector &results, double &objVal);
private:
    std::vector<double> m_c;
    std::vector<double> m_A1;
    std::vector<double> m_A2;
    std::vector<double> m_b;
};

#endif /* defined(__FinancialSamples__TwoDimensionalLPSolver__) */
