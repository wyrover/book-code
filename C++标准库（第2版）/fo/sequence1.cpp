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
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include "print.hpp"
using namespace std;

class IntSequence
{
private:
    int value;
public:
    IntSequence(int initialValue)     // constructor
        : value(initialValue)
    {
    }

    int operator()()                  // "function call"
    {
        return value++;
    }
};

int main()
{
    list<int> coll;
    // insert values from 1 to 9
    generate_n(back_inserter(coll),   // start
               9,                    // number of elements
               IntSequence(1));      // generates values, starting with 1
    PRINT_ELEMENTS(coll);
    // replace second to last element but one with values starting at 42
    generate(next(coll.begin()),      // start
             prev(coll.end()),       // end
             IntSequence(42));       // generates values, starting with 42
    PRINT_ELEMENTS(coll);
}
