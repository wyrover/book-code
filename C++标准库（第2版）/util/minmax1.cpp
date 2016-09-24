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

// function that compares two pointers by comparing the values to which they point
bool int_ptr_less(int* a, int* b)
{
    return *a < *b;
}

int main()
{
    int x = 17;
    int y = 42;
    int z = 33;
    int* px = &x;
    int* py = &y;
    int* pz = &z;
    // call max() with special comparison function
    int* pmax = std::max(px, py, int_ptr_less);
    // call minmax() for initializer list with special comparison function
    std::pair<int*, int*> extremes = std::minmax({px, py, pz},
                                     int_ptr_less);
    //...
}
