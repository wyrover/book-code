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
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include "fopow.hpp"
using namespace std;
using namespace std::placeholders;

int main()
{
    vector<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // print 3 raised to the power of all elements
    transform(coll.begin(), coll.end(),            // source
              ostream_iterator<float>(cout, " "), // destination
              bind(fopow<float, int>(), 3, _1));  // operation
    cout << endl;
    // print all elements raised to the power of 3
    transform(coll.begin(), coll.end(),            // source
              ostream_iterator<float>(cout, " "), // destination
              bind(fopow<float, int>(), _1, 3));  // operation
    cout << endl;
}
