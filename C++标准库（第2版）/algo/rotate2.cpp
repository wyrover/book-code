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
    set<int> coll;
    INSERT_ELEMENTS(coll, 1, 9);
    PRINT_ELEMENTS(coll);
    // print elements rotated one element to the left
    set<int>::const_iterator pos = next(coll.cbegin());
    rotate_copy(coll.cbegin(),                    // beginning of source
                pos,                              // new first element
                coll.cend(),                      // end of source
                ostream_iterator<int>(cout, " ")); // destination
    cout << endl;
    // print elements rotated two elements to the right
    pos = coll.cend();
    advance(pos, -2);
    rotate_copy(coll.cbegin(),                    // beginning of source
                pos,                              // new first element
                coll.cend(),                      // end of source
                ostream_iterator<int>(cout, " ")); // destination
    cout << endl;
    // print elements rotated so that element with value 4 is the beginning
    rotate_copy(coll.cbegin(),                    // beginning of source
                coll.find(4),                     // new first element
                coll.cend(),                      // end of source
                ostream_iterator<int>(cout, " ")); // destination
    cout << endl;
}
