// Ex6_02.cpp
// Sorting and inplace merging
#include <iostream>                              // For standard streams
#include <string>                                // For string class
#include <algorithm>                             // For sort(), inplace_merge()
#include <functional>                            // For greater<T>
#include <vector>                                // For vector container
#include <utility>                               // For pair template type
#include <map>                                   // For map container
#include <iterator>                              // For stream and back insert iterators
#include "Account.h"
#include "Transaction.h"
#include "Compare_Names.h"

using std::string;
using first_name = string;
using second_name = string;
using Name = std::pair<first_name, second_name>;
using Account_Number = size_t;

// Read the name of an account holder
Name get_holder_name(Account_Number number)
{
    std::cout << "Enter the holder's first and second names for account number " << number << ": ";
    string first {};
    string second {};
    std::cin >> first  >> second;
    return std::make_pair(first, second);
}

int main()
{
    std::vector<Transaction> transactions;
    std::cout << "Enter each transaction as:\n   5 digit account number   amount   credit(true or false).\n"
              << "Enter Ctrl+Z to end.\n";
    // Read 1st set of transactions
    std::copy(std::istream_iterator <Transaction> {std::cin}, std::istream_iterator <Transaction> {}, std::back_inserter(transactions));
    std::cin.clear();                              // Clear the EOF flag for the stream
    // Sort 1st set in descending account sequence
    std::stable_sort(std::begin(transactions), std::end(transactions), std::greater<>());
    // List the transactions
    std::cout << "First set of transactions after sorting...\n";
    std::copy(std::begin(transactions), std::end(transactions), std::ostream_iterator<Transaction> {std::cout, "\n"});
    // Read 2nd set of transactions
    std::cout << "\nEnter more transactions:\n";
    std::copy(std::istream_iterator <Transaction> {std::cin}, std::istream_iterator <Transaction> {}, std::back_inserter(transactions));
    std::cin.clear();                              // Clear the EOF flag for the stream
    // List the transactions
    std::cout << "\nSorted first set of transactions with second set appended...\n";
    std::copy(std::begin(transactions), std::end(transactions), std::ostream_iterator<Transaction> {std::cout, "\n"});
    // Sort second set into descending account sequence
    auto iter = std::is_sorted_until(std::begin(transactions), std::end(transactions), std::greater<>());
    std::stable_sort(iter, std::end(transactions), std::greater<>());
    // List the transactions
    std::cout << "\nSorted first set of transactions with sorted second set appended...\n";
    std::copy(std::begin(transactions), std::end(transactions), std::ostream_iterator<Transaction> {std::cout, "\n"});
    // Merge transactions in place
    std::inplace_merge(std::begin(transactions), iter, std::end(transactions), std::greater<>());
    // List the transactions
    std::cout << "\nMerged sets of transactions...\n";
    std::copy(std::begin(transactions), std::end(transactions), std::ostream_iterator<Transaction> {std::cout, "\n"});
    // Process transactions creating Account objects when necessary
    std::map<Account_Number, Account> accounts;

    for (const auto& tr : transactions) {
        Account_Number number = tr.get_acc_number();
        auto iter = accounts.find(number);

        if (iter == std::end(accounts))
            iter = accounts.emplace(number, Account {number, get_holder_name(number)}).first;

        if (iter->second.apply_transaction(tr)) {
            const auto& name = iter->second.get_name();
            std::cout << "\nAccount number " << number
                      << " for " << name.first << " " << name.second << " is overdrawn!\n"
                      << "The concept is that you bank with us - not the other way round, so fix it!\n" << std::endl;
        }
    }

    // Copy accounts to a vector container
    std::vector<Account> accs;

    for (const auto& pr : accounts)
        accs.push_back(pr.second);

    // List accounts after sorting in name sequence
    std::stable_sort(std::begin(accs), std::end(accs), Compare_Names());
    std::copy(std::begin(accs), std::end(accs), std::ostream_iterator < Account > {std::cout, "\n"});
}