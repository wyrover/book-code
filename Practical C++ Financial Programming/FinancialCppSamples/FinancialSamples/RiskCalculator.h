//
//  RiskCalculator.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__RiskCalculator__
#define __FinancialSamples__RiskCalculator__

#include "CreditRisk.h"

#include <memory>
#include <vector>

// calculates the risk associated to a portfolio
class RiskCalculator {
public:
    RiskCalculator();
    ~RiskCalculator();
    RiskCalculator(const RiskCalculator &);
    RiskCalculator &operator =(const RiskCalculator &);

    void addCreditRisk(std::shared_ptr<CreditRisk> risk);

    CreditRisk::RiskType portfolioMaxRisk();
    CreditRisk::RiskType portfolioMinRisk();
private:
    std::vector<std::shared_ptr<CreditRisk> > m_creditRisks;
};

#endif /* defined(__FinancialSamples__RiskCalculator__) */
