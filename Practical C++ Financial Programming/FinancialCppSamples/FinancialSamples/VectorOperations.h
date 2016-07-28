//
//  VectorOperations.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__VectorOperations__
#define __FinancialSamples__VectorOperations__

#include <vector>

// performs operations on std::vector using boost ublas
class VectorOperations {
public:
    VectorOperations(const std::vector<double> &v);
    VectorOperations(const VectorOperations &p);
    ~VectorOperations();
    VectorOperations &operator=(const VectorOperations &p);
    std::vector<double> scalarMult(double scalar);
    std::vector<double> addVector(const std::vector<double> &v);
    std::vector<double> subtractVector(const std::vector<double> &v);
    double dotProd(const std::vector<double> &v);
    double norm();
private:
    std::vector<double> m_data;
};

#endif /* defined(__FinancialSamples__VectorOperations__) */
