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

bool differenceOne(int elem1, int elem2)
{
    return elem1 + 1 == elem2 || elem1 - 1 == elem2;
}

int main()
{
    // source data
    int source[] = { 1, 4, 4, 6, 1, 2, 2, 3, 1, 6, 6, 6, 5, 7,
                     5, 4, 4
                   };
    // initialize coll with elements from source
    list<int> coll;
    copy(begin(source), end(source),               // source
         back_inserter(coll));                     // destination
    PRINT_ELEMENTS(coll);
    // print elements with consecutive duplicates removed
    unique_copy(coll.cbegin(), coll.cend(),        // source
                ostream_iterator<int>(cout, " ")); // destination
    cout << endl;
    // print elements without consecutive entries that differ by one
    unique_copy(coll.cbegin(), coll.cend(),        // source
                ostream_iterator<int>(cout, " "),  // destination
                differenceOne);                    // duplicates criterion
    cout << endl;
}
