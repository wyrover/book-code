//
//  TransactionHandler.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__TransactionHandler__
#define __FinancialSamples__TransactionHandler__

#include <iostream>
#include <stdexcept>

enum TransactionType {
    TRANSACTION_SELL,
    TRANSACTION_BUY,
    TRANSACTION_SHORT,
};

class FileError :public std::runtime_error {
public:
    FileError(const std::string &s);
};

class TransactionTypeError :public std::runtime_error {
public:
    TransactionTypeError(const std::string &s);
};

class TransactionHandler {
public:
    static const std::string SELL_OP;
    static const std::string BUY_OP;
    static const std::string SHORT_OP;

    TransactionHandler(const std::string &fileName);
    TransactionHandler(const TransactionHandler &);
    ~TransactionHandler();
    TransactionHandler &operator=(const TransactionHandler&);

    void handleTransactions();
private:
    std::string m_fileName;
};

#endif /* defined(__FinancialSamples__TransactionHandler__) */
