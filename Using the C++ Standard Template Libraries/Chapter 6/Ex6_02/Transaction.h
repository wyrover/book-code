#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>                              // For stream class
#include <iomanip>                               // For stream manipulators
#include "Account.h"

class Transaction
{
private:
    size_t account_number {};                      // The account number
    double amount {};                              // The amount
    bool credit {true};                            // credit = true debit=false

public:
    Transaction() = default;
    Transaction(size_t number, double amnt, bool cr) : account_number {number}, amount {amnt}, credit {cr} {}

    size_t get_acc_number() const
    {
        return account_number;
    }

    // Less-than operator - compares account numbers
    bool operator<(const Transaction& transaction) const
    {
        return account_number < transaction.account_number;
    }

    // Greater-than operator - compares account numbers
    bool operator>(const Transaction& transaction) const
    {
        return account_number > transaction.account_number;
    }

    friend std::ostream& operator<<(std::ostream& out, const Transaction& transaction);
    friend std::istream& operator>>(std::istream& in, Transaction& transaction);

    // Making the Account class a friend allows Account objects
    // to access private members of Transaction objects
    friend class Account;

};
// Stream insertion operator for Transaction objects
std::ostream& operator<<(std::ostream& out, const Transaction& transaction)
{
    return out << std::right << std::setfill('0') << std::setw(5) << transaction.account_number
           << std::setfill(' ') << std::setw(8) << std::fixed << std::setprecision(2) << transaction.amount
           << (transaction.credit ? " CR" : " DR");
}

// Stream extraction operator for Transaction objects
std::istream& operator>>(std::istream& in, Transaction& tr)
{
    if ((in >> std::skipws >> tr.account_number).eof())
        return in;

    return in >> tr.amount >> std::boolalpha >> tr.credit;
}

#endif