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
    list<int> coll1;
    set<int> coll2;
    // fill both collections with some sorted elements
    INSERT_ELEMENTS(coll1, 1, 6);
    INSERT_ELEMENTS(coll2, 3, 8);
    PRINT_ELEMENTS(coll1, "coll1:  ");
    PRINT_ELEMENTS(coll2, "coll2:  ");
    // print merged sequence
    cout << "merged: ";
    merge(coll1.cbegin(), coll1.cend(),
          coll2.cbegin(), coll2.cend(),
          ostream_iterator<int>(cout, " "));
    cout << endl;
}
