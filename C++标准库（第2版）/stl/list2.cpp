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
#include <list>
#include <iostream>
using namespace std;

int main()
{
    list<char> coll;      // list container for character elements

    // append elements from 'a' to 'z'
    for (char c = 'a'; c <= 'z'; ++c) {
        coll.push_back(c);
    }

    // print all elements
    // - while there are elements
    // - print and remove the first element
    while (! coll.empty()) {
        cout << coll.front() << ' ';
        coll.pop_front();
    }

    cout << endl;
}
