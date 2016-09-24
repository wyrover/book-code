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
#include "print.hpp"
using namespace std;

class Nth      // function object that returns true for the nth call
{
private:
    int nth;       // call for which to return true
    int count;     // call counter
public:
    Nth(int n) : nth(n), count(0)
    {
    }
    bool operator()(int)
    {
        return ++count == nth;
    }
};

int main()
{
    list<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    PRINT_ELEMENTS(coll, "coll:        ");
    // remove third element
    list<int>::iterator pos;
    pos = remove_if(coll.begin(), coll.end(), // range
                    Nth(3));                  // remove criterion
    coll.erase(pos, coll.end());
    PRINT_ELEMENTS(coll, "3rd removed: ");
}
