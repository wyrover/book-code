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
#include <set>
#include <algorithm>
#include <iterator>
using namespace std;

int main()
{
    // type of the collection:
    // - no duplicates
    // - elements are integral values
    // - descending order
    set<int, greater<int>> coll1;
    // insert elements in random order using different member functions
    coll1.insert({4, 3, 5, 1, 6, 2});
    coll1.insert(5);

    // print all elements
    for (int elem : coll1) {
        cout << elem << ' ';
    }

    cout << endl;
    // insert 4 again and process return value
    auto status = coll1.insert(4);

    if (status.second) {
        cout << "4 inserted as element "
             << distance(coll1.begin(), status.first) + 1 << endl;
    } else {
        cout << "4 already exists" << endl;
    }

    // assign elements to another set with ascending order
    set<int> coll2(coll1.cbegin(), coll1.cend());
    // print all elements of the copy using stream iterators
    copy(coll2.cbegin(), coll2.cend(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
    // remove all elements up to element with value 3
    coll2.erase(coll2.begin(), coll2.find(3));
    // remove all elements with value 5
    int num;
    num = coll2.erase(5);
    cout << num << " element(s) removed" << endl;
    // print all elements
    copy(coll2.cbegin(), coll2.cend(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
}
