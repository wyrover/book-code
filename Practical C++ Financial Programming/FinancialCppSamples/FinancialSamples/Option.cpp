//
//  Option.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Option.h"


Option::Option(const std::string &ticker, double strike)
: m_ticker(ticker),
  m_strike(strike)
{
}

Option::Option(const Option &p)
: m_ticker(p.m_ticker),
  m_strike(p.m_strike)
{
}

Option::~Option()
{
}

Option &Option::operator=(const Option &p)
{
    if (this != &p)
    {
        m_ticker = p.m_ticker;
        m_strike = p.m_strike;
    }
    return *this;
}

std::string Option::ticker()
{
    return m_ticker;
}

double Option::strike()
{
    return m_strike;
}

void Option::setTicker(const std::string &s)
{
    m_ticker = s;
}

void Option::setStrike(double val)
{
    m_strike = val;
}
