//
//  BondCalculator.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "BondCalculator.h"

BondCalculator::BondCalculator(const std::string institution,
                               int numPeriods, double principal, double couponValue)
: m_institution(institution),
  m_principal(principal),
  m_coupon(couponValue),
  m_numPeriods(numPeriods)
{

}

BondCalculator::BondCalculator(const BondCalculator &v)
: m_institution(v.m_institution),
  m_principal(v.m_principal),
  m_coupon(v.m_coupon),
  m_numPeriods(v.m_numPeriods)
{

}

BondCalculator::~BondCalculator()
{

}

BondCalculator &BondCalculator::operator =(const BondCalculator &v)
{
    if (this != &v)
    {
        this->m_institution = v.m_institution;
        this->m_principal = v.m_principal;
        this->m_numPeriods = v.m_numPeriods;
        this->m_coupon = v.m_coupon;
    }
    return *this;
}

double BondCalculator::interestRate()
{
    return m_coupon / m_principal;
}
