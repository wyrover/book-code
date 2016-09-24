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
    vector<int> coll = { 5, 3, 9, 1, 3, 4, 8, 2, 6 };
    PRINT_ELEMENTS(coll, "coll: ");
    // define predicate: check whether element is odd:
    auto isOdd = [](int elem) {
        return elem % 2 == 1;
    };

    // check whether coll is partitioned in odd and even elements
    if (is_partitioned(coll.cbegin(), coll.cend(),   // range
                       isOdd)) {                    // predicate
        cout << "coll is partitioned" << endl;
        // find first even element:
        auto pos = partition_point(coll.cbegin(), coll.cend(),
                                   isOdd);
        cout << "first even element: " << *pos << endl;
    } else {
        cout << "coll is not partitioned" << endl;
    }
}
