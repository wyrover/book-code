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
#include <iterator>
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

int main()
{
    // create list with elements from 1 to 9
    list<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // find position of element with value 5
    list<int>::const_iterator pos;
    pos = find(coll.cbegin(), coll.cend(),   // range
               5);                          // value
    // print value of the element
    cout << "pos:   " << *pos << endl;
    // convert iterator to reverse iterator
    list<int>::const_reverse_iterator rpos(pos);
    // print value of the element to which the reverse iterator refers
    cout << "rpos:  " << *rpos << endl;
    // convert reverse iterator back to normal iterator
    list<int>::const_iterator rrpos;
    rrpos = rpos.base();
    // print value of the element to which the normal iterator refers
    cout << "rrpos: " << *rrpos << endl;
}
