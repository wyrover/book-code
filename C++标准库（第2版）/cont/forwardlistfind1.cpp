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
#include <forward_list>
#include "print.hpp"
using namespace std;

int main()
{
    forward_list<int> list = { 1, 2, 3, 4, 5, 97, 98, 99 };
    // find the position before the first even element
    auto posBefore = list.before_begin();

    for (auto pos = list.begin(); pos != list.end(); ++pos, ++posBefore) {
        if (*pos % 2 == 0) {
            break;  // element found
        }
    }

    // and insert a new element in front of the first even element
    list.insert_after(posBefore, 42);
    PRINT_ELEMENTS(list);
}
