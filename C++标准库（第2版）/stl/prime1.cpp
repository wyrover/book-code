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
#include <list>
#include <algorithm>
#include <iostream>
#include <cstdlib>      // for abs()
using namespace std;

// predicate, which returns whether an integer is a prime number
bool isPrime(int number)
{
    // ignore negative sign
    number = abs(number);

    // 0 and 1 are no prime numbers
    if (number == 0 || number == 1) {
        return false;
    }

    // find divisor that divides without a remainder
    int divisor;

    for (divisor = number / 2; number % divisor != 0; --divisor) {
        ;
    }

    // if no divisor greater than 1 is found, it is a prime number
    return divisor == 1;
}

int main()
{
    list<int> coll;

    // insert elements from 24 to 30
    for (int i = 24; i <= 30; ++i) {
        coll.push_back(i);
    }

    // search for prime number
    auto pos = find_if(coll.cbegin(), coll.cend(),   // range
                       isPrime);                    // predicate

    if (pos != coll.end()) {
        // found
        cout << *pos << " is first prime number found" << endl;
    } else {
        // not found
        cout << "no prime number found" << endl;
    }
}
