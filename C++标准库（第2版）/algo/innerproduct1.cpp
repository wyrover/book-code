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
#include "algostuff.hpp"
using namespace std;

int main()
{
    list<int> coll;
    INSERT_ELEMENTS(coll, 1, 6);
    PRINT_ELEMENTS(coll);
    // process sum of all products
    // (0 + 1*1 + 2*2 + 3*3 + 4*4 + 5*5 + 6*6)
    cout << "inner product: "
         << inner_product(coll.cbegin(), coll.cend(),   // first range
                          coll.cbegin(),               // second range
                          0)                           // initial value
         << endl;
    // process sum of 1*6 ... 6*1
    // (0 + 1*6 + 2*5 + 3*4 + 4*3 + 5*2 + 6*1)
    cout << "inner reverse product: "
         << inner_product(coll.cbegin(), coll.cend(),   // first range
                          coll.crbegin(),              // second range
                          0)                           // initial value
         << endl;
    // process product of all sums
    // (1 * 1+1 * 2+2 * 3+3 * 4+4 * 5+5 * 6+6)
    cout << "product of sums: "
         << inner_product(coll.cbegin(), coll.cend(),   // first range
                          coll.cbegin(),               // second range
                          1,                           // initial value
                          multiplies<int>(),           // outer operation
                          plus<int>())                 // inner operation
         << endl;
}
