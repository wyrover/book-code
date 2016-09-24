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
#include <deque>
#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>
#include "print.hpp"
using namespace std;
using namespace std::placeholders;

int main()
{
    set<int, greater<int>> coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    deque<int> coll2;
    // Note: due to the sorting criterion greater<>() elements have reverse order:
    PRINT_ELEMENTS(coll1, "initialized: ");
    // transform all elements into coll2 by multiplying them with 10
    transform(coll1.cbegin(), coll1.cend(),      // source
              back_inserter(coll2),             // destination
              bind(multiplies<int>(), _1, 10)); // operation
    PRINT_ELEMENTS(coll2, "transformed: ");
    // replace value equal to 70 with 42
    replace_if(coll2.begin(), coll2.end(),       // range
               bind(equal_to<int>(), _1, 70),   // replace criterion
               42);                             // new value
    PRINT_ELEMENTS(coll2, "replaced:    ");
    // remove all elements with values between 50 and 80
    coll2.erase(remove_if(coll2.begin(), coll2.end(),
                          bind(logical_and<bool>(),
                               bind(greater_equal<int>(), _1, 50),
                               bind(less_equal<int>(), _1, 80))),
                coll2.end());
    PRINT_ELEMENTS(coll2, "removed:     ");
}
