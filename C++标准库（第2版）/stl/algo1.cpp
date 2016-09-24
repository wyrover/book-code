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
    vector<int> coll = { 2, 5, 4, 1, 6, 3 };
    // find and print minimum and maximum elements
    auto minpos = min_element(coll.cbegin(), coll.cend());
    cout << "min: "  << *minpos << endl;
    auto maxpos = max_element(coll.cbegin(), coll.cend());
    cout << "max: "  << *maxpos << endl;
    // sort all elements
    sort(coll.begin(), coll.end());
    // find the first element with value 3
    // - no cbegin()/cend() because later we modify the elements pos3 refers to
    auto pos3 = find(coll.begin(), coll.end(),   // range
                     3);                        // value
    // reverse the order of the found element with value 3 and all following elements
    reverse(pos3, coll.end());

    // print all elements
    for (auto elem : coll) {
        cout << elem << ' ';
    }

    cout << endl;
}
