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
#include<functional>
#include<iostream>

std::function<int(int, int)> returnLambda()
{
    return [](int x, int y) {
        return x * y;
    };
}

int main()
{
    auto lf = returnLambda();
    std::cout << lf(6, 7) << std::endl;
}
