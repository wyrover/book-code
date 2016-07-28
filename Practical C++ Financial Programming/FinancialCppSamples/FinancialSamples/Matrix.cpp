//
//  Matrix.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Matrix.h"

#include <stdexcept>

Matrix::Matrix(int size)
{
    for (int i=0; i<size; ++i )
    {
        std::vector<double> row(size, 0);
        m_rows.push_back(row);
    }
}

Matrix::Matrix(int size, int size2)
{
    for (int i=0; i<size; ++i )
    {
        std::vector<double> row(size2, 0);
        m_rows.push_back(row);
    }
}

Matrix::Matrix(const Matrix &s)
: m_rows(s.m_rows)
{
}

Matrix::~Matrix()
{
}

Matrix &Matrix::operator=(const Matrix &s)
{
    if (this != &s)
    {
        m_rows = s.m_rows;
    }
    return *this;
}

Matrix::Row &Matrix::operator[](int pos)
{
    return m_rows[pos];
}

void Matrix::transpose()
{
    std::vector<Row> rows;
    for (unsigned i=0;i <m_rows[0].size(); ++i)
    {
        std::vector<double> row;
        for (unsigned j=0; j<m_rows.size(); ++j)
        {
            row[j] = m_rows[j][i];
        }
        rows.push_back(row);
    }
    m_rows.swap(rows);
}

double Matrix::trace()
{
    if (m_rows.size() != m_rows[0].size())
    {
        return 0;
    }
    double total = 0;
    for (unsigned i=0; i<m_rows.size(); ++i)
    {
        total += m_rows[i][i];
    }
    return total;
}

void Matrix::add(const Matrix &s)
{
    if (m_rows.size() != s.m_rows.size() ||
        m_rows[0].size() != s.m_rows[0].size())
    {
        throw new std::runtime_error("invalid matrix dimensions");
    }
    for (unsigned i=0; i<m_rows.size(); ++i)
    {
        for (unsigned j=0; j<m_rows[0].size(); ++j)
        {
            m_rows[i][j] += s.m_rows[i][j];
        }
    }
}

void Matrix::subtract(const Matrix &s)
{
    if (m_rows.size() != s.m_rows.size() ||
        m_rows[0].size() != s.m_rows[0].size())
    {
        throw new std::runtime_error("invalid matrix dimensions");
    }
    for (unsigned i=0; i<m_rows.size(); ++i)
    {
        for (unsigned j=0; j<m_rows[0].size(); ++j)
        {
            m_rows[i][j] += s.m_rows[i][j];
        }
    }
}

void Matrix::multiply(const Matrix &s)
{
    if (m_rows[0].size() != s.m_rows.size())
    {
        throw new std::runtime_error("invalid matrix dimensions");
    }
    std::vector<Row> rows;
    for (unsigned i=0; i<m_rows.size(); ++i)
    {
        std::vector<double> row;
        for (unsigned j=0; j<s.m_rows.size(); ++j)
        {
            double Mij = 0;
            for (unsigned k=0; k<m_rows[0].size(); ++k)
            {
                Mij += m_rows[i][k] * s.m_rows[k][j];
            }
            row.push_back(Mij);
        }
        rows.push_back(row);
    }
    m_rows.swap(rows);
}

int Matrix::numRows() const
{
	return m_rows.size();
}

void sfsf()
{
    Matrix a(3,5);
    a[1][2] = 3;
    Matrix b(5,6);
    a * b;
}

Matrix operator+(const Matrix &s1, const Matrix &s2)
{
	Matrix s(s1);
	s.subtract(s2);
	return s;
}

Matrix operator-(const Matrix &s1, const Matrix &s2)
{
	Matrix s(s1);
	s.subtract(s2);
	return s;
}

Matrix operator*(const Matrix &s1, const Matrix &s2)
{
	Matrix s(s1);
	s.multiply(s2);
	return s;
}

