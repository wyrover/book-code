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
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
using namespace std::placeholders;

class Person
{
private:
    string name;
public:
    Person(const string& n) : name(n)
    {
    }
    void print() const
    {
        cout << name << endl;
    }
    void print2(const string& prefix) const
    {
        cout << prefix << name << endl;
    }
    //...
};

int main()
{
    vector<Person> coll
        = { Person("Tick"), Person("Trick"), Person("Track") };
    // call member function print() for each person
    for_each(coll.begin(), coll.end(),
             bind(&Person::print, _1));
    cout << endl;
    // call member function print2() with additional argument for each person
    for_each(coll.begin(), coll.end(),
             bind(&Person::print2, _1, "Person: "));
    cout << endl;
    // call print2() for temporary Person
    bind(&Person::print2, _1, "This is: ")(Person("nico"));
}
