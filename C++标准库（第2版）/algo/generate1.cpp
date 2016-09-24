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
    list<int> coll;
    // insert five random numbers
    generate_n(back_inserter(coll),       // beginning of destination range
               5,                        // count
               rand);                    // new value generator
    PRINT_ELEMENTS(coll);
    // overwrite with five new random numbers
    generate(coll.begin(), coll.end(),    // destination range
             rand);                      // new value generator
    PRINT_ELEMENTS(coll);
}
