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
using namespace std::placeholders;

int main()
{
    vector<int> coll;
    vector<int>::iterator pos;
    INSERT_ELEMENTS(coll, 1, 9);
    PRINT_ELEMENTS(coll, "coll: ");
    // find first element greater than 3
    pos = find_if(coll.begin(), coll.end(),     // range
                  bind(greater<int>(), _1, 3)); // criterion
    // print its position
    cout << "the "
         << distance(coll.begin(), pos) + 1
         << ". element is the first greater than 3" << endl;
    // find first element divisible by 3
    pos = find_if(coll.begin(), coll.end(),
    [](int elem) {
        return elem % 3 == 0;
    });
    // print its position
    cout << "the "
         << distance(coll.begin(), pos) + 1
         << ". element is the first divisible by 3" << endl;
    // find first element not <5
    pos = find_if_not(coll.begin(), coll.end(),
                      bind(less<int>(), _1, 5));
    cout << "first value >=5: " << *pos << endl;
}
