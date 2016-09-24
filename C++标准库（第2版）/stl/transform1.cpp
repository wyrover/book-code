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
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "print.hpp"

int square(int value)
{
    return value * value;
}

int main()
{
    std::set<int>    coll1;
    std::vector<int> coll2;

    // insert elements from 1 to 9 into coll1
    for (int i = 1; i <= 9; ++i) {
        coll1.insert(i);
    }

    PRINT_ELEMENTS(coll1, "initialized: ");
    // transform each element from coll1 to coll2
    // - square transformed values
    std::transform(coll1.cbegin(), coll1.cend(),  // source
                   std::back_inserter(coll2),    // destination
                   square);                      // operation
    PRINT_ELEMENTS(coll2, "squared:     ");
}
