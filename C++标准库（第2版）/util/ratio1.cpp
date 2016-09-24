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
#include <ratio>
#include <iostream>
using namespace std;

int main()
{
    typedef ratio<5, 3> FiveThirds;
    cout << FiveThirds::num << "/" << FiveThirds::den << endl;
    typedef ratio<25, 15> AlsoFiveThirds;
    cout << AlsoFiveThirds::num << "/" << AlsoFiveThirds::den << endl;
    ratio<42, 42> one;
    cout << one.num << "/" << one.den << endl;
    ratio<0> zero;
    cout << zero.num << "/" << zero.den << endl;
    typedef ratio < 7, -3 > Neg;
    cout << Neg::num << "/" << Neg::den << endl;
}
