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
#include <unordered_set>
#include <string>
#include <iostream>
#include "hashval.hpp"
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
        : fname(fn), lname(ln), no(n) {}
    friend ostream& operator << (ostream& strm, const Customer& c)
    {
        return strm << "[" << c.fname << "," << c.lname << ","
               << c.no << "]";
    }
    friend class CustomerHash;
    friend class CustomerEqual;
};

class CustomerHash
{
public:
    std::size_t operator()(const Customer& c) const
    {
        return hash_val(c.fname, c.lname, c.no);
    }
};

class CustomerEqual
{
public:
    bool operator()(const Customer& c1, const Customer& c2) const
    {
        return c1.no == c2.no;
    }
};

int main()
{
    // unordered set with own hash function and equivalence criterion
    unordered_set<Customer, CustomerHash, CustomerEqual> custset;
    custset.insert(Customer("nico", "josuttis", 42));
    PRINT_ELEMENTS(custset);
}
