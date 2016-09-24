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
#include <sstream>

void printNumbers(std::default_random_engine& dre)
{
    for (int i = 0; i < 6; ++i) {
        std::cout << dre() << " ";
    }

    std::cout << std::endl;
}

int main()
{
    // create engine and generate numbers
    std::default_random_engine dre;
    printNumbers(dre);
    // create equal engine and generate numbers
    std::default_random_engine dre2;  // same initial state as dre
    printNumbers(dre2);
    // create engine with initial state specified by a seed
    std::default_random_engine dre3(42);
    printNumbers(dre3);
    // save state of engine dre
    std::stringstream engineState;
    engineState << dre;
    // generate numbers according to current state of dre
    printNumbers(dre);
    // restore saved state of engine dre and create numbers again
    engineState >> dre;
    printNumbers(dre);
    // restore saved state of engine dre, skip 2 numbers and create numbers again
    engineState.clear();   // clear EOF bit
    engineState.seekg(0);  // and move read position to the beginning
    engineState >> dre;
    dre.discard(2);
    printNumbers(dre);
}
