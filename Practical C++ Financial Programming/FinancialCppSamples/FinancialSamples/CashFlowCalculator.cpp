//
//  CashFlowCalculator.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "CashFlowCalculator.h"

#include <cmath>
#include <iostream>

CashFlowCalculator::CashFlowCalculator(double rate)
: m_rate(rate)
{

}

CashFlowCalculator::CashFlowCalculator(const CashFlowCalculator &v)
: m_rate(v.m_rate)
{

}

CashFlowCalculator::~CashFlowCalculator()
{

}

CashFlowCalculator &CashFlowCalculator::operator =(const CashFlowCalculator &v)
{
    if (this != &v)
    {
        this->m_cashPayments = v.m_cashPayments;
        this->m_timePeriods = v.m_timePeriods;
        this->m_rate = v.m_rate;
    }
    return *this;
}


void CashFlowCalculator::addCashPayment(double value, int timePeriod)
{
    m_cashPayments.push_back(value);
    m_timePeriods.push_back(timePeriod);
}

double CashFlowCalculator::presentValue(double futureValue, int timePeriod)
{
    double pValue = futureValue / pow(1+m_rate, timePeriod);
    std::cout << " value " << pValue << std::endl;
    return pValue;
}

double CashFlowCalculator::presentValue()
{
    double total = 0;
    for (unsigned i=0; i<m_cashPayments.size(); ++i)
    {
        total += presentValue(m_cashPayments[i], m_timePeriods[i]);
    }
    return total;
}

