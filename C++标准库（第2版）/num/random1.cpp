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
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
    // create default engine as source of randomness
    std::default_random_engine dre;
    // use engine to generate integral numbers between 10 and 20 (both included)
    std::uniform_int_distribution<int> di(10, 20);

    for (int i = 0; i < 20; ++i) {
        std::cout << di(dre) << " ";
    }

    std::cout << std::endl;
    // use engine to generate floating-point numbers between 10.0 and 20.0
    // (10.0 included, 20.0 not included)
    std::uniform_real_distribution<double> dr(10, 20);

    for (int i = 0; i < 8; ++i) {
        std::cout << dr(dre) << " ";
    }

    std::cout << std::endl;
    // use engine to shuffle elements
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    //...
    std::shuffle(v.begin(), v.end(),   // range
                 dre);                // source of randomness

    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }

    std::cout << std::endl;
}
