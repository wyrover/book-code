//
//  NumVector.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__NumVector__
#define __FinancialSamples__NumVector__

#include <vector>
#include <stddef.h>

class NumVector {
public:
    NumVector();
    ~NumVector();
    NumVector(const NumVector &);
    NumVector &operator =(const NumVector &);

    void add(double val);
    void removeLast();
    double get(int pos) const;
    size_t size() const;
private:
    std::vector<double> m_values;
};

NumVector operator +(const NumVector &a, const NumVector &b);
NumVector operator -(const NumVector &a, const NumVector &b);
NumVector operator *(const NumVector &a, const NumVector &b);

#endif /* defined(__FinancialSamples__NumVector__) */
