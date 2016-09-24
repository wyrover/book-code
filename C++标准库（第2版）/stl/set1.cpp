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
#include <set>
#include <iostream>

int main()
{
    // type of the collection
    typedef std::set<int> IntSet;
    IntSet coll;        // set container for int values
    // insert elements from 1 to 6 in arbitrary order
    // - note that there are two calls of insert() with value 1
    coll.insert(3);
    coll.insert(1);
    coll.insert(5);
    coll.insert(4);
    coll.insert(1);
    coll.insert(6);
    coll.insert(2);
    // print all elements
    // - iterate over all elements
    IntSet::const_iterator pos;

    for (pos = coll.begin(); pos != coll.end(); ++pos) {
        std::cout << *pos << ' ';
    }

    std::cout << std::endl;
}
