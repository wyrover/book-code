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
    vector<int> coll;
    vector<int>::iterator pos;
    INSERT_ELEMENTS(coll, 1, 9);
    PRINT_ELEMENTS(coll, "coll: ");
    // define an object for the predicate (using a lambda)
    auto isEven = [](int elem) {
        return elem % 2 == 0;
    };
    // print whether all, any, or none of the elements are/is even
    cout << boolalpha << "all even?:  "
         << all_of(coll.cbegin(), coll.cend(), isEven) << endl;
    cout << "any even?:  "
         << any_of(coll.cbegin(), coll.cend(), isEven) << endl;
    cout << "none even?: "
         << none_of(coll.cbegin(), coll.cend(), isEven) << endl;
}
