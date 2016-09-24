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
    INSERT_ELEMENTS(coll, 2, 7);
    INSERT_ELEMENTS(coll, 4, 9);
    PRINT_ELEMENTS(coll, "coll: ");
    // replace all elements with value 6 with 42
    replace(coll.begin(), coll.end(),      // range
            6,                            // old value
            42);                          // new value
    PRINT_ELEMENTS(coll, "coll: ");
    // replace all elements with value less than 5 with 0
    replace_if(coll.begin(), coll.end(),   // range
    [](int elem) {             // criterion for replacement
        return elem < 5;
    },
    0);                        // new value
    PRINT_ELEMENTS(coll, "coll: ");
}
