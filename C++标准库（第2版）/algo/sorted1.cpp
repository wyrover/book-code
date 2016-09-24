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

int main()
{
    vector<int> c1 = { 1, 2, 2, 4, 6, 7, 7, 9 };
    deque<int>  c2 = { 2, 2, 2, 3, 6, 6, 8, 9 };
    // print source ranges
    cout << "c1:                         " ;
    copy(c1.cbegin(), c1.cend(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
    cout << "c2:                         " ;
    copy(c2.cbegin(), c2.cend(),
         ostream_iterator<int>(cout, " "));
    cout << '\n' << endl;
    // sum the ranges by using merge()
    cout << "merge():                    ";
    merge(c1.cbegin(), c1.cend(),
          c2.cbegin(), c2.cend(),
          ostream_iterator<int>(cout, " "));
    cout << endl;
    // unite the ranges by using set_union()
    cout << "set_union():                ";
    set_union(c1.cbegin(), c1.cend(),
              c2.cbegin(), c2.cend(),
              ostream_iterator<int>(cout, " "));
    cout << endl;
    // intersect the ranges by using set_intersection()
    cout << "set_intersection():         ";
    set_intersection(c1.cbegin(), c1.cend(),
                     c2.cbegin(), c2.cend(),
                     ostream_iterator<int>(cout, " "));
    cout << endl;
    // determine elements of first range without elements of second range
    // by using set_difference()
    cout << "set_difference():           ";
    set_difference(c1.cbegin(), c1.cend(),
                   c2.cbegin(), c2.cend(),
                   ostream_iterator<int>(cout, " "));
    cout << endl;
    // determine difference the ranges with set_symmetric_difference()
    cout << "set_symmetric_difference(): ";
    set_symmetric_difference(c1.cbegin(), c1.cend(),
                             c2.cbegin(), c2.cend(),
                             ostream_iterator<int>(cout, " "));
    cout << endl;
}
