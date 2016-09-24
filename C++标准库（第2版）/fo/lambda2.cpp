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
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8 };
    // process and print mean value
    long sum = 0;
    for_each(coll.begin(), coll.end(),   // range
    [&sum](int elem) {
        sum += elem;
    });
    double mv = static_cast<double>(sum) / static_cast<double>(coll.size());
    cout << "mean value: " << mv << endl;
}
