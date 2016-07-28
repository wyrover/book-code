//
//  MatrixOperations.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__MatrixOperations__
#define __FinancialSamples__MatrixOperations__

#include <vector>

#include "Matrix.h"

class MatrixOperations {
public:
    MatrixOperations();
    ~MatrixOperations();
    MatrixOperations(const MatrixOperations &p);
    MatrixOperations &operator=(const MatrixOperations &p);

    void addRow(const std::vector<double> &row);
    Matrix multiply(Matrix &m);
    Matrix transpose();
    Matrix elementwiseMultiply(Matrix &m);
    Matrix scalarMultiply(double scalar);
    std::vector<double> preMultiply(const std::vector<double> &v);
    std::vector<double> postMultiply(const std::vector<double> &v);


private:
    std::vector<std::vector<double> > m_rows;
};


#endif /* defined(__FinancialSamples__MatrixOperations__) */
