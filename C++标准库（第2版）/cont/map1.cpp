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
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    map<string, double> coll { { "tim", 9.9 },
        { "struppi", 11.77 }
    } ;
    // square the value of each element:
    for_each(coll.begin(), coll.end(),
    [](pair<const string, double>& elem) {
        elem.second *= elem.second;
    });
    // print each element:
    for_each(coll.begin(), coll.end(),
    [](const map<string, double>::value_type & elem) {
        cout << elem.first << ": " << elem.second << endl;
    });
}
