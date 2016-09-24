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
    deque<int> coll;
    coll = { 1, 2, 7, 7, 6, 3, 9, 5, 7, 7, 7, 3, 6 };
    PRINT_ELEMENTS(coll);
    // find three consecutive elements with value 7
    deque<int>::iterator pos;
    pos = search_n(coll.begin(), coll.end(),     // range
                   3,                           // count
                   7);                          // value

    // print result
    if (pos != coll.end()) {
        cout << "three consecutive elements with value 7 "
             << "start with " << distance(coll.begin(), pos) + 1
             << ". element" << endl;
    } else {
        cout << "no four consecutive elements with value 7 found"
             << endl;
    }

    // find four consecutive odd elements
    pos = search_n(coll.begin(), coll.end(),     // range
                   4,                           // count
                   0,                           // value
    [](int elem, int value) {    // criterion
        return elem % 2 == 1;
    });

    // print result
    if (pos != coll.end()) {
        cout << "first four consecutive odd elements are: ";

        for (int i = 0; i < 4; ++i, ++pos) {
            cout << *pos << ' ';
        }
    } else {
        cout << "no four consecutive elements with value > 3 found";
    }

    cout << endl;
}
