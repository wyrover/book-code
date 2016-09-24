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
#include <deque>
#include <algorithm>
#include <functional>
#include <iostream>
#include "print.hpp"
using namespace std;

int main()
{
    deque<int> coll = { 1, 2, 3, 5, 7, 11, 13, 17, 19 };
    PRINT_ELEMENTS(coll, "initialized: ");
    // negate all values in coll
    transform(coll.cbegin(), coll.cend(),      // source
              coll.begin(),                   // destination
              negate<int>());                 // operation
    PRINT_ELEMENTS(coll, "negated:     ");
    // square all values in coll
    transform(coll.cbegin(), coll.cend(),      // first source
              coll.cbegin(),                  // second source
              coll.begin(),                   // destination
              multiplies<int>());             // operation
    PRINT_ELEMENTS(coll, "squared:     ");
}
