//
//  MIPSolver.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__MIPSolver__
#define __FinancialSamples__MIPSolver__

#include "LPSolver.h"

class MIPSolver : public LPSolver {
public:
    MIPSolver(Matrix &A, const std::vector<double> &b, const std::vector<double> &c);
    MIPSolver(const MIPSolver &p);
    ~MIPSolver();
    MIPSolver &operator=(const MIPSolver &p);

    void setColInteger(int colNum);
    void setColBinary(int colNum);
    virtual ResultType solve(std::vector<double> &result, double &objValue);
};

#endif /* defined(__FinancialSamples__MIPSolver__) */
