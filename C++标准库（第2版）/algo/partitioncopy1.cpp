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
    vector<int> coll = { 1, 6, 33, 7, 22, 4, 11, 33, 2, 7, 0, 42, 5 };
    PRINT_ELEMENTS(coll, "coll: ");
    // destination collections:
    vector<int> evenColl;
    vector<int> oddColl;
    // copy all elements partitioned accordingly into even and odd elements
    partition_copy(coll.cbegin(), coll.cend(), // source range
                   back_inserter(evenColl),   // destination for even elements
                   back_inserter(oddColl),    // destination for odd elements
    [](int elem) {             // predicate: check for even elements
        return elem % 2 == 0;
    });
    PRINT_ELEMENTS(evenColl, "evenColl: ");
    PRINT_ELEMENTS(oddColl, "oddColl:  ");
}
