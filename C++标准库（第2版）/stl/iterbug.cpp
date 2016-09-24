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
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> coll1;    // empty collection
    vector<int> coll2;    // empty collection
    // RUNTIME ERROR:
    // - beginning is behind the end of the range
    vector<int>::iterator pos = coll1.begin();
    reverse(++pos, coll1.end());

    // insert elements from 1 to 9 into coll1
    for (int i = 1; i <= 9; ++i) {
        coll1.push_back(i);
    }

    // RUNTIME ERROR:
    // - overwriting nonexisting elements
    copy(coll1.cbegin(), coll1.cend(),   // source
         coll2.begin());                // destination
    // RUNTIME ERROR:
    // - collections mistaken
    // - cbegin() and cend() refer to different collections
    copy(coll1.cbegin(), coll2.cend(),   // source
         coll1.end());                  // destination
}
