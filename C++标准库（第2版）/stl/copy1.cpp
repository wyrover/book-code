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
#include <list>
#include <vector>
#include <deque>
using namespace std;

int main()
{
    list<int>   coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    vector<int> coll2;
    // resize destination to have enough room for the overwriting algorithm
    coll2.resize(coll1.size());
    // copy elements from first into second collection
    // - overwrites existing elements in destination
    copy(coll1.cbegin(), coll1.cend(),    // source
         coll2.begin());                 // destination
    // create third collection with enough room
    // - initial size is passed as parameter
    deque<int> coll3(coll1.size());
    // copy elements from first into third collection
    copy(coll1.cbegin(), coll1.cend(),    // source
         coll3.begin());                 // destination
}
