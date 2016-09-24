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
#include <iterator>
#include <vector>
#include <iostream>

int main()
{
    int vals[] = { 33, 67, -4, 13, 5, 2 };
    // use begin() and end() for ordinary C arrays
    std::vector<int> v(std::begin(vals), std::end(vals));
    // use global begin() and end() for containers:
    std::copy(std::begin(v), std::end(v),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

