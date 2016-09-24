/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <string>
#include <iostream>
#include <unordered_set>
#include "../cont/hashval.hpp"
#include <functional>
#include "print.hpp"
using namespace std;

class Customer
{
private:
    string fname;
    string lname;
    long   no;
public:
    Customer(const string& fn, const string& ln, long n)
        : fname(fn), lname(ln), no(n)
    {
    }
    string firstname() const
    {
        return fname;
    };
    string lastname() const
    {
        return lname;
    };
    long number() const
    {
        return no;
    };
    friend ostream& operator << (ostream& strm, const Customer& c)
    {
        return strm << "[" << c.fname << "," << c.lname << "," << c.no << "]";
    }
};

int main()
{
    // lambda for user-defined hash function
    auto hash = [](const Customer & c) {
        return hash_val(c.firstname(), c.lastname(), c.number());
    };
    // lambda for user-defined equality criterion
    auto eq = [](const Customer & c1, const Customer & c2) {
        return c1.number() == c2.number();
    };
    // create unordered set with user-defined behavior
    unordered_set<Customer, decltype(hash), decltype(eq)> custset(10, hash, eq);
    // ERROR: unordered_set<Customer,function<size_t(Customer,Customer)>,decltype(eq)> custset(10,hash,eq);
    custset.insert(Customer("nico", "josuttis", 42));
    PRINT_ELEMENTS(custset);
}
