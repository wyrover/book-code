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

int main()
{
    vector<int> coll;
    INSERT_ELEMENTS(coll, 1, 9);
    PRINT_ELEMENTS(coll, "coll:     ");
    // shuffle all elements randomly
    random_shuffle(coll.begin(), coll.end());
    PRINT_ELEMENTS(coll, "shuffled: ");
    // sort them again
    sort(coll.begin(), coll.end());
    PRINT_ELEMENTS(coll, "sorted:   ");
    // shuffle elements with default engine
    default_random_engine dre;
    shuffle(coll.begin(), coll.end(),   // range
            dre);                      // random-number generator
    PRINT_ELEMENTS(coll, "shuffled: ");
}
