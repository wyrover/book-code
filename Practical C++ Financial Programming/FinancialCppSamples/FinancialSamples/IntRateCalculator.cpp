//
//  IntRateCalculator.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "IntRateCalculator.h"


IntRateCalculator::IntRateCalculator(double rate)
: m_rate(rate)
{

}

IntRateCalculator::~IntRateCalculator()
{
    
}

IntRateCalculator::IntRateCalculator(const IntRateCalculator &v)
: m_rate(v.m_rate)
{

}

IntRateCalculator &IntRateCalculator::operator=(const IntRateCalculator &v)
{
    if (&v != this) {
        this->m_rate = v.m_rate;
    }
    return *this;
}