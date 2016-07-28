//
//  Stock.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Stock.h"

Stock::Stock(const std::string &ticker, double price, double div)
: m_ticker(ticker),
m_currentPrice(price),
m_dividend(div)
{
}

Stock::Stock(const Stock &p)
: m_ticker(p.m_ticker),
m_currentPrice(p.m_currentPrice),
m_dividend(p.m_dividend)
{
}

Stock::~Stock()
{
}

Stock &Stock::operator=(const Stock &p)
{
    if (this != &p)
    {
        m_ticker = p.m_ticker;
        m_currentPrice = p.m_currentPrice;
        m_dividend = p.m_dividend;
    }
    return *this;
}

double Stock::price()
{
    return m_currentPrice;
}

void Stock::setPrice(double price)
{
    m_currentPrice = price;
}

double Stock::dividend()
{
    return m_dividend;
}

void Stock::setDividend(double div)
{
    m_dividend = div;
}

double Stock::dividendYield()
{
    return  m_dividend / m_currentPrice;
}

std::string Stock::ticker()
{
    return m_ticker;
}
