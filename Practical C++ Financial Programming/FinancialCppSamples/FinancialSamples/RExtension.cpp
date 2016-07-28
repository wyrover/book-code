//
//  RExtension.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#include "RExtension.h"


#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

extern "C" {
void presentValue(int *, double *, int *, double *, double *);
}


void presentValue(int *numPayments, double *intRate,
                  int *timePeriods, double *payments, double *result)
{
    int n = *numPayments;
    RExtension re(*intRate);
    for (int i=0; i<n; ++i)
    {
        re.addCashPayment(payments[i], timePeriods[i]);
    }
    *result = re.presentValue();
}


RExtension::RExtension(double rate)
: m_rate(rate)
{

}

RExtension::RExtension(const RExtension &v)
: m_rate(v.m_rate)
{

}

RExtension::~RExtension()
{

}

RExtension &RExtension::operator =(const RExtension &v)
{
    if (this != &v)
    {
        this->m_cashPayments = v.m_cashPayments;
        this->m_timePeriods = v.m_timePeriods;
        this->m_rate = v.m_rate;
    }
    return *this;
}


void RExtension::addCashPayment(double value, int timePeriod)
{
    m_cashPayments.push_back(value);
    m_timePeriods.push_back(timePeriod);
}

double RExtension::presentValue(double futureValue, int timePeriod)
{
    double pValue = futureValue / pow(1+m_rate, timePeriod);
    cout << " value " << pValue << endl;
    return pValue;
}

double RExtension::presentValue()
{
    double total = 0;
    for (unsigned i=0; i<m_cashPayments.size(); ++i)
    {
        total += presentValue(m_cashPayments[i], m_timePeriods[i]);
    }
    return total;
}

