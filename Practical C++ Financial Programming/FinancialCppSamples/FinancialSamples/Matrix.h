//
//  Matrix.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Matrix__
#define __FinancialSamples__Matrix__

#include <vector>

class Matrix {
public:
    typedef std::vector<double> Row;

    Matrix(int size);
    Matrix(int size1, int size2);
    Matrix(const Matrix &s);
    ~Matrix();
    Matrix &operator=(const Matrix &s);

    void transpose();
    double trace();
    void add(const Matrix &s);
    void subtract(const Matrix &s);
    void multiply(const Matrix &s);

    Row & operator[](int pos);
    int numRows() const;
private:
    std::vector<Row> m_rows;
};

// free operators
//
Matrix operator+(const Matrix &s1, const Matrix &s2);
Matrix operator-(const Matrix &s1, const Matrix &s2);
Matrix operator*(const Matrix &s1, const Matrix &s2);

#endif /* defined(__FinancialSamples__Matrix__) */
