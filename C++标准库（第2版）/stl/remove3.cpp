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
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

int main()
{
    // unordered set with elements from 1 to 9
    set<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // print all elements of the collection
    copy(coll.cbegin(), coll.cend(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
    // Remove all elements with value 3
    // - algorithm remove() does not work
    // - instead member function erase() works
    int num = coll.erase(3);
    // print number of removed elements
    cout << "number of removed elements: " << num << endl;
    // print all elements of the modified collection
    copy(coll.cbegin(), coll.cend(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
}
