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
    vector<int> coll;
    INSERT_ELEMENTS(coll, 2, 6);
    INSERT_ELEMENTS(coll, 4, 9);
    INSERT_ELEMENTS(coll, 1, 7);
    PRINT_ELEMENTS(coll, "coll:               ");
    // remove all elements with value 5
    vector<int>::iterator pos;
    pos = remove(coll.begin(), coll.end(),         // range
                 5);                               // value to remove
    PRINT_ELEMENTS(coll, "size not changed:   ");
    // erase the "removed" elements in the container
    coll.erase(pos, coll.end());
    PRINT_ELEMENTS(coll, "size changed:       ");
    // remove all elements less than 4
    coll.erase(remove_if(coll.begin(), coll.end(), // range
    [](int elem) {            // remove criterion
        return elem < 4;
    }),
    coll.end());
    PRINT_ELEMENTS(coll, "<4 removed:         ");
}
