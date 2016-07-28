#ifndef ACCCOUNT_H
#define ACCCOUNT_H
#include <iostream>                                   // For stream class
#include <iomanip>                                    // For stream manipulators
#include <string>                                     // For string class
#include <utility>                                    // For pair template type
#include "Transaction.h"

using first_name = std::string;
using second_name = std::string;
using Name = std::pair<first_name, second_name>;

class Account
{
private:
    size_t account_number {};                           // 5-digit account number
    Name name {"", ""};                                 // A pair containing 1st & 2nd names
    double balance {};                                  // The account balance - negative when overdrawn

public:
    Account() = default;
    Account(size_t number, const Name& nm) : account_number {number}, name {nm} {}

    double get_balance() const
    {
        return balance;
    }
    void set_balance(double bal)
    {
        balance = bal;
    }

    size_t get_acc_number() const
    {
        return account_number;
    }
    const Name& get_name() const
    {
        return name;
    }

    // Apply a transaction to the account
    bool apply_transaction(const Transaction& transaction)
    {
        if (transaction.credit)                           // For a credit...
            balance += transaction.amount;                  // ...add the mount
        else                                              // For a debit...
            balance -= transaction.amount;                  // ...subtract the amount

        return balance < 0.0;                             // Return true when overdrawn
    }

    // Less-than operator - compares by account number
    bool operator<(const Account& acc) const
    {
        return account_number < acc.account_number;
    }

    friend std::ostream& operator<<(std::ostream& out, const Account& account);
};

// Stream insertion operator for Account objects
std::ostream& operator<<(std::ostream& out, const Account& acc)
{
    return out << std::left << std::setw(20) << acc.name.first + " " + acc.name.second
           << std::right << std::setfill('0') << std::setw(5) << acc.account_number
           << std::setfill(' ') << std::setw(8) << std::fixed << std::setprecision(2) << acc.balance;
}
#endif
