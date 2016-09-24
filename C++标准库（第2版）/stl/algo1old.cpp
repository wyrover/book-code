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
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    // create vector with elements from 1 to 6 in arbitrary order
    vector<int> coll;
    coll.push_back(2);
    coll.push_back(5);
    coll.push_back(4);
    coll.push_back(1);
    coll.push_back(6);
    coll.push_back(3);
    // find and print minimum and maximum elements
    vector<int>::const_iterator minpos = min_element(coll.begin(),
                                         coll.end());
    cout << "min: "  << *minpos << endl;
    vector<int>::const_iterator maxpos = max_element(coll.begin(),
                                         coll.end());
    cout << "max: "  << *maxpos << endl;
    // sort all elements
    sort(coll.begin(), coll.end());
    // find the first element with value 3
    vector<int>::iterator pos3;
    pos3 = find(coll.begin(), coll.end(),   // range
                3);                        // value
    // reverse the order of the found element with value 3 and all following elements
    reverse(pos3, coll.end());
    // print all elements
    vector<int>::const_iterator pos;

    for (pos = coll.begin(); pos != coll.end(); ++pos) {
        cout << *pos << ' ';
    }

    cout << endl;
}
