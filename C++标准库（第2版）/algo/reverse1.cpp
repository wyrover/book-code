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
    INSERT_ELEMENTS(coll, 1, 9);
    PRINT_ELEMENTS(coll, "coll: ");
    // reverse order of elements
    reverse(coll.begin(), coll.end());
    PRINT_ELEMENTS(coll, "coll: ");
    // reverse order from second to last element but one
    reverse(coll.begin() + 1, coll.end() - 1);
    PRINT_ELEMENTS(coll, "coll: ");
    // print all of them in reverse order
    reverse_copy(coll.cbegin(), coll.cend(),          // source
                 ostream_iterator<int>(cout, " "));  // destination
    cout << endl;
}
