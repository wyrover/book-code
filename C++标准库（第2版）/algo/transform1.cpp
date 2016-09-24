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
    vector<int> coll1;
    list<int> coll2;
    INSERT_ELEMENTS(coll1, 1, 9);
    PRINT_ELEMENTS(coll1, "coll1:   ");
    // negate all elements in coll1
    transform(coll1.cbegin(), coll1.cend(),     // source range
              coll1.begin(),                   // destination range
              negate<int>());                  // operation
    PRINT_ELEMENTS(coll1, "negated: ");
    // transform elements of coll1 into coll2 with ten times their value
    transform(coll1.cbegin(), coll1.cend(),     // source range
              back_inserter(coll2),            // destination range
              bind(multiplies<int>(), _1, 10)); // operation
    PRINT_ELEMENTS(coll2, "coll2:   ");
    // print coll2 negatively and in reverse order
    transform(coll2.crbegin(), coll2.crend(),   // source range
              ostream_iterator<int>(cout, " "), // destination range
    [](int elem) {                   // operation
        return -elem;
    });
    cout << endl;
}
