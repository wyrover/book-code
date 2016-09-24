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
#include <map>
#include <string>
#include <iostream>
using namespace std;

int main()
{
    multimap<int, string> coll;       // container for int/string values
    // assign some elements in arbitrary order
    // - a value with key 1 gets inserted twice
    coll = { {5, "tagged"},
        {2, "a"},
        {1, "this"},
        {4, "of"},
        {6, "strings"},
        {1, "is"},
        {3, "multimap"}
    };

    // print all element values
    // - element member second is the value
    for (auto elem : coll) {
        cout << elem.second << ' ';
    }

    cout << endl;
}
