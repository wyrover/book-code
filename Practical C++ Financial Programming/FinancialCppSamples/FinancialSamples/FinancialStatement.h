//
//  FinancialStatement.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__FinancialStatement__
#define __FinancialSamples__FinancialStatement__

#include <string>
#include <vector>
#include <memory>

class FinancialStatement {
public:
    FinancialStatement();
    ~FinancialStatement();
    FinancialStatement(const FinancialStatement&);
    FinancialStatement &operator=(FinancialStatement &);

    double getReturn();
    void addTransaction(const std::string &security, double val);

private:
    double m_return;
    std::vector<std::pair<std::string,double> > m_transactions;
};

std::auto_ptr<FinancialStatement> getSampleStatement();

void transferFinancialStatement(std::auto_ptr<FinancialStatement> statement);

#endif /* defined(__FinancialSamples__FinancialStatement__) */
