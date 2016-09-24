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
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    // create constant string
    const string hello("Hello, how are you?");
    // initialize string s with all characters of string hello
    string s(hello.cbegin(), hello.cend());

    // ranged-based for loop that iterates through all the characters
    for (char c : s) {
        cout << c;
    }

    cout << endl;
    // reverse the order of all characters inside the string
    reverse(s.begin(), s.end());
    cout << "reverse:       " << s << endl;
    // sort all characters inside the string
    sort(s.begin(), s.end());
    cout << "ordered:       " << s << endl;
    // remove adjacent duplicates
    // - unique() reorders and returns new end
    // - erase() shrinks accordingly
    s.erase(unique(s.begin(),
                   s.end()),
            s.end());
    cout << "no duplicates: " << s << endl;
}
