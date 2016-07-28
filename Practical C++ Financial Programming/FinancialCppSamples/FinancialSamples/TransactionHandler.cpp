//
//  TransactionHandler.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "TransactionHandler.h"

#include <fstream>

FileError::FileError(const std::string &s)
: std::runtime_error(s)
{
}


TransactionTypeError::TransactionTypeError(const std::string &s)
: std::runtime_error(s)
{

}

const std::string TransactionHandler::SELL_OP = "SELL";
const std::string TransactionHandler::BUY_OP = "BUY";
const std::string TransactionHandler::SHORT_OP = "SHORT";

TransactionHandler::TransactionHandler(const std::string &fileName)
: m_fileName(fileName)
{

}

TransactionHandler::TransactionHandler(const TransactionHandler &a)
: m_fileName(a.m_fileName)
{

}

TransactionHandler::~TransactionHandler()
{
}

TransactionHandler &TransactionHandler::operator=(const TransactionHandler&a)
{
    if (this != &a)
    {
        m_fileName = a.m_fileName;
    }
    return *this;
}

void TransactionHandler::handleTransactions()
{
    std::ifstream file;
    file.open(m_fileName, std::ifstream::in);
    if (file.fail())
    {
        throw new FileError(std::string("error opening file ") + m_fileName);
    }

    std::string op;
    file >> op;
    while (file.good() && !file.eof())
    {
        if (op != SELL_OP && op != BUY_OP && op !=  SHORT_OP)
        {
            throw new TransactionTypeError(std::string("unknown transaction ") + op);
        }

        // process remaining transaction data...
    }
}




