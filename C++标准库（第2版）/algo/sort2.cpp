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

bool lessLength(const string& s1, const string& s2)
{
    return s1.length() < s2.length();
}

int main()
{
    // fill two collections with the same elements
    vector<string> coll1 = { "1xxx", "2x", "3x", "4x", "5xx", "6xxxx",
                             "7xx", "8xxx", "9xx", "10xxx", "11", "12",
                             "13", "14xx", "15", "16", "17"
                           };
    vector<string> coll2(coll1);
    PRINT_ELEMENTS(coll1, "on entry:\n ");
    // sort (according to the length of the strings)
    sort(coll1.begin(), coll1.end(),            // range
         lessLength);                          // criterion
    stable_sort(coll2.begin(), coll2.end(),     // range
                lessLength);                   // criterion
    PRINT_ELEMENTS(coll1, "\nwith sort():\n ");
    PRINT_ELEMENTS(coll2, "\nwith stable_sort():\n ");
}
