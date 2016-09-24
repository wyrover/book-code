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
    list<int> coll;
    INSERT_ELEMENTS(coll, 2, 6);
    INSERT_ELEMENTS(coll, 4, 9);
    PRINT_ELEMENTS(coll);
    // print all elements with value 5 replaced with 55
    replace_copy(coll.cbegin(), coll.cend(),         // source
                 ostream_iterator<int>(cout, " "),   // destination
                 5,                                  // old value
                 55);                                // new value
    cout << endl;
    // print all elements with a value less than 5 replaced with 42
    replace_copy_if(coll.cbegin(), coll.cend(),      // source
                    ostream_iterator<int>(cout, " "), // destination
                    bind(less<int>(), _1, 5),        // replacement criterion
                    42);                             // new value
    cout << endl;
    // print each element while each odd element is replaced with 0
    replace_copy_if(coll.cbegin(), coll.cend(),      // source
                    ostream_iterator<int>(cout, " "), // destination
    [](int elem) {                   // replacement criterion
        return elem % 2 == 1;
    },
    0);                              // new value
    cout << endl;
}
