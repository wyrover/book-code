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
#include <iostream>
#include <deque>
#include <algorithm>
using namespace std;

void print(int elem)
{
    cout << elem << ' ';
}

int main()
{
    // create deque with elements from 1 to 9
    deque<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // find position of element with value 2
    deque<int>::const_iterator pos1;
    pos1 = find(coll.cbegin(), coll.cend(),   // range
                2);                          // value
    // find position of element with value 7
    deque<int>::const_iterator pos2;
    pos2 = find(coll.cbegin(), coll.cend(),   // range
                7);                          // value
    // print all elements in range [pos1,pos2)
    for_each(pos1, pos2,      // range
             print);         // operation
    cout << endl;
    // convert iterators to reverse iterators
    deque<int>::const_reverse_iterator rpos1(pos1);
    deque<int>::const_reverse_iterator rpos2(pos2);
    // print all elements in range [pos1,pos2) in reverse order
    for_each(rpos2, rpos1,    // range
             print);         // operation
    cout << endl;
}
