//
//  FinancialStatement.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "FinancialStatement.h"

#include <iostream>

FinancialStatement::FinancialStatement()
: m_return(0)
{

}

FinancialStatement::~FinancialStatement()
{

}

FinancialStatement::FinancialStatement(const FinancialStatement &v)
: m_return(v.m_return),
m_transactions(v.m_transactions)
{

}

FinancialStatement &FinancialStatement::operator=(FinancialStatement &v)
{
    if (this != &v)
    {
        m_return = v.m_return;
        m_transactions = v.m_transactions;
    }
    return *this;
}

double FinancialStatement::getReturn()
{
    return m_return;
}

void FinancialStatement::addTransaction(const std::string &security, double val)
{
    m_transactions.push_back(std::make_pair(security, val));
}

// returns a sample statement that includes a few common stocks
std::auto_ptr<FinancialStatement> getSampleStatement()
{
    std::auto_ptr<FinancialStatement> fs(new FinancialStatement);
    fs->addTransaction("IBM", 102.2);
    fs->addTransaction("AAPL", 523.0);
    return fs;
}

void transferFinancialStatement(std::auto_ptr<FinancialStatement> statement)
{
    // perform transfer here
    // ...
    // statement is still valid
    std::cout << statement->getReturn() << std::endl;
    // statement is released here
}
