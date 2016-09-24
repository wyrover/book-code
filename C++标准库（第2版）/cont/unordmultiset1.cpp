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
#include "print.hpp"
using namespace std;

int main()
{
    // create and initialize, expand, and print unordered multiset
    unordered_multiset<int> coll = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 77 };
    coll.insert({ -7, 17, 33, -11, 17, 19, 1, 13});
    PRINT_ELEMENTS(coll);
    // remove all elements with specific value
    coll.erase(17);
    // remove one of the elements with specific value
    auto pos = coll.find(13);

    if (pos != coll.end()) {
        coll.erase(pos);
    }

    PRINT_ELEMENTS(coll);
}
