//
//  Recommendation.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Recommendation.h"



Recommendation::Recommendation()
: m_recType(HOLD),
  m_target(0)
{

}

Recommendation::Recommendation(const std::string &ticker, RecommendationType rec, double target)
: m_ticker(ticker),
  m_recType(rec),
  m_target(target)
{

}

Recommendation::~Recommendation()
{

}

Recommendation::Recommendation(const Recommendation &r)
: m_ticker(r.m_ticker),
  m_recType(r.m_recType),
  m_target(r.m_target)
{

}

Recommendation &Recommendation::operator =(const Recommendation &r)
{
    if (this != &r)
    {
        m_ticker = r.m_ticker;
        m_recType = r.m_recType;
        m_target = r.m_target;
    }
    return *this;
}

double Recommendation::getTarget() const
{
    return m_target;
}

RecommendationType Recommendation::getRecommendation() const
{
    return m_recType;
}

std::string Recommendation::getTicker() const
{
    return m_ticker;
}
