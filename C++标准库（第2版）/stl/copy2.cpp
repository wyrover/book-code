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
#include <algorithm>
#include <iterator>
#include <list>
#include <vector>
#include <deque>
#include <set>
#include <iostream>
using namespace std;

int main()
{
    list<int> coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // copy the elements of coll1 into coll2 by appending them
    vector<int> coll2;
    copy(coll1.cbegin(), coll1.cend(),     // source
         back_inserter(coll2));           // destination
    // copy the elements of coll1 into coll3 by inserting them at the front
    // - reverses the order of the elements
    deque<int> coll3;
    copy(coll1.cbegin(), coll1.cend(),     // source
         front_inserter(coll3));          // destination
    // copy elements of coll1 into coll4
    // - only inserter that works for associative collections
    set<int> coll4;
    copy(coll1.cbegin(), coll1.cend(),     // source
         inserter(coll4, coll4.begin())); // destination
}
