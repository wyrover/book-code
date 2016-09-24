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
    list<int> coll1;
    INSERT_ELEMENTS(coll1, 1, 6);
    INSERT_ELEMENTS(coll1, 1, 9);
    PRINT_ELEMENTS(coll1);
    // print elements without those having the value 3
    remove_copy(coll1.cbegin(), coll1.cend(),       // source
                ostream_iterator<int>(cout, " "),   // destination
                3);                                 // removed value
    cout << endl;
    // print elements without those having a value greater than 4
    remove_copy_if(coll1.cbegin(), coll1.cend(),    // source
                   ostream_iterator<int>(cout, " "), // destination
    [](int elem) {     // criterion for elements NOT copied
        return elem > 4;
    });
    cout << endl;
    // copy all elements not less than 4 into a multiset
    multiset<int> coll2;
    remove_copy_if(coll1.cbegin(), coll1.cend(),    // source
                   inserter(coll2, coll2.end()),    // destination
                   bind(less<int>(), _1, 4));       // elements NOT copied
    PRINT_ELEMENTS(coll2);
}
