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
#include <deque>
#include <string>
#include <iostream>
using namespace std;

class Person
{
public:
    string firstname() const;
    string lastname() const;
    //...
};

int main()
{
    deque<Person> coll;
    //...
    // sort Persons according to lastname (and firstname):
    sort(coll.begin(), coll.end(),               // range
    [](const Person & p1, const Person & p2) { // sort criterion
        return p1.lastname() < p2.lastname() ||
               (p1.lastname() == p2.lastname() &&
                p1.firstname() < p2.firstname());
    });
    //...
}
