//
//  ModifiedCAP.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__ModifiedCAP__
#define __FinancialSamples__ModifiedCAP__

#include "Matrix.h"

// a modified (linearized) model for Capital Asset Pricing
class ModifiedCAP {
public:
    ModifiedCAP(int N, int T, double R, Matrix &retMatrix, const std::vector<double> &ret);
    ModifiedCAP(const ModifiedCAP &p);
    ~ModifiedCAP();
    ModifiedCAP &operator=(const ModifiedCAP &p);

    void solveModel(std::vector<double> &results, double &objVal);
    void solveExtendedModel(std::vector<double> &results, double &objVal);
private:
    int m_N;  // number of investment
    int m_T;  // number of periods
    double m_R;  // desired return
    Matrix m_retMatrix;
    std::vector<double> m_assetRet; // single returns
};

#endif /* defined(__FinancialSamples__ModifiedCAP__) */
