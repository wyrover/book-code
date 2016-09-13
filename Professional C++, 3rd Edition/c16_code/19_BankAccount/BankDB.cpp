#include "BankDB.h"
#include <utility>
#include <stdexcept>

using namespace std;

bool BankDB::addAccount(const BankAccount& acct)
{
    // Do the actual insert, using the account number as the key
    auto res = mAccounts.insert(make_pair(acct.getAcctNum(), acct));
    // If your compiler doesn't support C++11 'auto', you need the following:
    // Declare a variable to store the return from insert()
    //pair<map<int, BankAccount>::iterator, bool> res;
    // do the actual insert, using the account number as the key
    //res = mAccounts.insert(make_pair(acct.getAcctNum(), acct));
    // Return the bool field of the pair specifying success or failure
    return res.second;
}

void BankDB::deleteAccount(int acctNum)
{
    mAccounts.erase(acctNum);
}

BankAccount& BankDB::findAccount(int acctNum)
{
    // Finding an element via its key can be done with find()
    auto it = mAccounts.find(acctNum);
    // If your compiler doesn't support C++11 'auto', you need:
    //map<int, BankAccount>::iterator it = mAccounts.find(acctNum);

    if (it == end(mAccounts)) {
        throw out_of_range("No account with that number.");
    }

    // Remember that iterators into maps refer to pairs of key/value
    return it->second;
}

BankAccount& BankDB::findAccount(const string& name)
{
    // Finding an element by a non-key attribute requires a linear
    // search through the elements.
    for (auto& p : mAccounts) {
        if (p.second.getClientName() == name) {
            // found it!
            return p.second;
        }
    }

    // If your compiler doesn't support the above C++11 range based for
    // loop, you need the following loop
    //for (map<int, BankAccount>::iterator it = mAccounts.begin();
    //     it != mAccounts.end(); ++it) {
    //  if (it->second.getClientName() == name) {
    //    // found it!
    //    return it->second;
    //  }
    //}
    throw out_of_range("No account with that name.");
}

void BankDB::mergeDatabase(BankDB& db)
{
    // Just insert copies of all the accounts in the old db
    // to the new database.
    mAccounts.insert(begin(db.mAccounts), end(db.mAccounts));
    // Now delete all the accounts in the old database.
    db.mAccounts.clear();
}
