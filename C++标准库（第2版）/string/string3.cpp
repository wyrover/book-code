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
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <locale>
using namespace std;

int main()
{
    string input;
    // don't skip leading whitespaces
    cin.unsetf(ios::skipws);
    // read all characters while compressing whitespaces
    const locale& loc(cin.getloc());    // locale
    unique_copy(istream_iterator<char>(cin),  // beginning of source
                istream_iterator<char>(),     // end of source
                back_inserter(input),         // destination
    [ = ](char c1, char c2) {     // criterion for adj. duplicates
        return isspace(c1, loc) && isspace(c2, loc);
    });
    // process input
    // - here: write it to the standard output
    cout << input;
}
