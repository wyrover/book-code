//
//  FundamentalsCalc.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "FundamentalsCalc.h"

#include <iostream>


FundamentalsCalculator::FundamentalsCalculator(const std::string &ticker,
                                               double price, double dividend) :
m_ticker(ticker),
m_price(price),
m_dividend(dividend),
m_earningsEstimate(0),
m_numShares(0),
m_bookValue(0),
m_assets(0),
m_liabilitiesAndIntangibles(0),
m_epsGrowth(0),
m_netIncome(0),
m_shareholdersEquity(0)
{

}

FundamentalsCalculator::FundamentalsCalculator(const FundamentalsCalculator &v) :
m_ticker(v.m_ticker),
m_price(v.m_price),
m_dividend(v.m_dividend),
m_earningsEstimate(v.m_earningsEstimate),
m_numShares(v.m_numShares),
m_bookValue(v.m_bookValue),
m_assets(v.m_assets),
m_liabilitiesAndIntangibles(v.m_liabilitiesAndIntangibles),
m_epsGrowth(v.m_epsGrowth),
m_netIncome(v.m_netIncome),
m_shareholdersEquity(v.m_shareholdersEquity)
{

}

FundamentalsCalculator::~FundamentalsCalculator()
{

}

FundamentalsCalculator &FundamentalsCalculator::operator=(const FundamentalsCalculator &v)
{
    if (this != &v)
    {
        m_ticker = v.m_ticker;
        m_price = v.m_price;
        m_dividend = v.m_dividend;
        m_earningsEstimate = v.m_earningsEstimate;
        m_numShares = v.m_numShares;
        m_bookValue = v.m_bookValue;
        m_assets = v.m_assets;
        m_liabilitiesAndIntangibles = v.m_liabilitiesAndIntangibles;
        m_epsGrowth = v.m_epsGrowth;
        m_netIncome = v.m_netIncome;
        m_shareholdersEquity = v.m_shareholdersEquity;
    }
    return *this;
}

double FundamentalsCalculator::PE()
{
    return (m_price * m_numShares)/ m_earnings;
}

double FundamentalsCalculator::forwardPE()
{
    return (m_price * m_numShares)/ m_earningsEstimate;
}

double FundamentalsCalculator::returnOnEquity()
{
    return m_netIncome / m_shareholdersEquity;
}

double FundamentalsCalculator::getDividend()
{
    return m_dividend;
}

double FundamentalsCalculator::bookValue()
{
    return m_bookValue;
}

double FundamentalsCalculator::priceToBookRatio()
{
    return (m_price * m_numShares) / (m_assets - m_liabilitiesAndIntangibles);
}

double FundamentalsCalculator::priceEarningsToGrowth()
{
    return PE()/ m_epsGrowth;
}

void FundamentalsCalculator::setNumOfShares(int n)
{
    m_numShares = n;
}

void FundamentalsCalculator::setEarnings(double val)
{
    m_earnings = val;
}

void FundamentalsCalculator::setExpectedEarnings(double val)
{
    m_earningsEstimate = val;
}

void FundamentalsCalculator::setBookValue(double val)
{
    m_bookValue = val;
}

void FundamentalsCalculator::setEpsGrowth(double val)
{
    m_epsGrowth = val;
}

void FundamentalsCalculator::setNetIncome(double val)
{
    m_netIncome = val;
}

void FundamentalsCalculator::setShareHoldersEquity(double val)
{
    m_shareholdersEquity = val;
}

void FundamentalsCalculator::setLiabilitiesAndIntangibles(double val)
{
    m_liabilitiesAndIntangibles = val;
}

void FundamentalsCalculator::setAssets(double val)
{
    m_assets = val;
}