//
//  Matrix_Py.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Matrix_Py__
#define __FinancialSamples__Matrix_Py__

#include <iostream>

#include "Matrix.h"

class MatrixP : public Matrix {
public:
    MatrixP(int a);
    MatrixP(int a, int b);
    MatrixP(const MatrixP &p);
    ~MatrixP();
    void set(int a, int b, double v);
    double get(int a, int b);
};

#endif /* defined(__FinancialSamples__Matrix_Py__) */
