//
//  NumVector.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "NumVector.h"

#include <iostream>
#include <stdexcept>

NumVector::NumVector()
{

}

NumVector::~NumVector()
{

}

NumVector::NumVector(const NumVector &v)
: m_values(v.m_values)
{

}

NumVector &NumVector::operator=(const NumVector &v)
{
    if (this != &v)
    {
        m_values = v.m_values;
    }
    return *this;
}

size_t NumVector::size() const
{
    return m_values.size();
}

double NumVector::get(int pos) const
{
    return m_values[pos];
}

void NumVector::add(double val)
{
    m_values.push_back(val);
}

void NumVector::removeLast()
{
    m_values.pop_back();
}

NumVector operator +(const NumVector &a, const NumVector &b)
{
    if (a.size() != b.size())
    {
        throw new std::runtime_error("vectors must have the same size");
    }
    NumVector result;
    for (int i=0; i<a.size(); ++i)
    {
        result.add(a.get(i) + b.get(i));
    }
    return result;
}

NumVector operator -(const NumVector &a, const NumVector &b)
{
    if (a.size() != b.size())
    {
        throw new std::runtime_error("vectors must have the same size");
    }
    NumVector result;
    for (int i=0; i<a.size(); ++i)
    {
        result.add(a.get(i) - b.get(i));
    }
    return result;
}

NumVector operator *(const NumVector &a, const NumVector &b)
{
    if (a.size() != b.size())
    {
        throw new std::runtime_error("vectors must have the same size");
    }
    NumVector result;
    for (int i=0; i<a.size(); ++i)
    {
        result.add(a.get(i) * b.get(i));
    }
    return result;
}

