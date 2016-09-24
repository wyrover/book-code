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
#include <cstdlib>
#include "algostuff.hpp"
using namespace std;

bool absLess(int elem1, int elem2)
{
    return abs(elem1) < abs(elem2);
}

int main()
{
    deque<int> coll;
    INSERT_ELEMENTS(coll, 2, 6);
    INSERT_ELEMENTS(coll, -3, 6);
    PRINT_ELEMENTS(coll);
    // process and print minimum and maximum
    cout << "minimum: "
         << *min_element(coll.cbegin(), coll.cend())
         << endl;
    cout << "maximum: "
         << *max_element(coll.cbegin(), coll.cend())
         << endl;
    // print min and max and their distance using minmax_element()
    auto mm = minmax_element(coll.cbegin(), coll.cend());
    cout << "min: " << *(mm.first) << endl;    // print minimum
    cout << "max: " << *(mm.second) << endl;   // print maximum
    cout << "distance: " << distance(mm.first, mm.second) << endl;
    // process and print minimum and maximum of absolute values
    cout << "minimum of absolute values: "
         << *min_element(coll.cbegin(), coll.cend(),
                         absLess)
         << endl;
    cout << "maximum of absolute values: "
         << *max_element(coll.cbegin(), coll.cend(),
                         absLess)
         << endl;
}
