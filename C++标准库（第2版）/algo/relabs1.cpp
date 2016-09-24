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
    vector<int> coll = { 17, -3, 22, 13, 13, -9 };
    PRINT_ELEMENTS(coll, "coll:     ");
    // convert into relative values
    adjacent_difference(coll.cbegin(), coll.cend(),   // source
                        coll.begin());               // destination
    PRINT_ELEMENTS(coll, "relative: ");
    // convert into absolute values
    partial_sum(coll.cbegin(), coll.cend(),           // source
                coll.begin());                       // destination
    PRINT_ELEMENTS(coll, "absolute: ");
}
