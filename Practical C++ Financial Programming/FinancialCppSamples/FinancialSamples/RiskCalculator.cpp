//
//  RiskCalculator.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "RiskCalculator.h"

#include <memory>

RiskCalculator::RiskCalculator()
{

}

RiskCalculator::~RiskCalculator()
{

}

RiskCalculator::RiskCalculator(const RiskCalculator &v)
: m_creditRisks(v.m_creditRisks)
{

}

RiskCalculator &RiskCalculator::operator =(const RiskCalculator &v)
{
    if (this != &v)
    {
        m_creditRisks = v.m_creditRisks;
    }
    return *this;
}

void RiskCalculator::addCreditRisk(std::shared_ptr<CreditRisk> risk)
{
    m_creditRisks.push_back(risk);
}

CreditRisk::RiskType RiskCalculator::portfolioMaxRisk()
{
    auto risk = CreditRisk::RiskType::AAA;
    for (std::vector<std::shared_ptr<CreditRisk> >::iterator p = m_creditRisks.begin();
         p != m_creditRisks.end(); ++p)
    {

    }
    for (auto p = m_creditRisks.begin(); p != m_creditRisks.end(); ++p)
    {
        if ((*p)->getRating() < risk)
        {
            risk = (*p)->getRating();
        }
    }
    return risk;
}

CreditRisk::RiskType RiskCalculator::portfolioMinRisk()
{
    CreditRisk::RiskType risk = CreditRisk::RiskType::C;
    for (unsigned i=0; i<m_creditRisks.size(); ++i)
    {
        if (m_creditRisks[i]->getRating() > risk)
        {
            risk = m_creditRisks[i]->getRating();
        }
    }
    return risk;
}
