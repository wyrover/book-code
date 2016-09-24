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
#include <array>
#include <algorithm>
#include <functional>
#include <numeric>
#include "print.hpp"
using namespace std;

int main()
{
    // create array with 10 ints
    array<int, 10> a = { 11, 22, 33, 44 };
    PRINT_ELEMENTS(a);
    // modify last two elements
    a.back() = 9999999;
    a[a.size() - 2] = 42;
    PRINT_ELEMENTS(a);
    // process sum of all elements
    cout << "sum: "
         << accumulate(a.begin(), a.end(), 0)
         << endl;
    // negate all elements
    transform(a.begin(), a.end(),   // source
              a.begin(),            // destination
              negate<int>());       // operation
    PRINT_ELEMENTS(a);
}

