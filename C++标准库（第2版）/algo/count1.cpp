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
#include "algostuff.hpp"
using namespace std;

int main()
{
    vector<int> coll;
    int num;
    INSERT_ELEMENTS(coll, 1, 9);
    PRINT_ELEMENTS(coll, "coll: ");
    // count elements with value 4
    num = count(coll.cbegin(), coll.cend(),      // range
                4);                             // value
    cout << "number of elements equal to 4:      " << num << endl;
    // count elements with even value
    num = count_if(coll.cbegin(), coll.cend(),   // range
    [](int elem) {               // criterion
        return elem % 2 == 0;
    });
    cout << "number of elements with even value: " << num << endl;
    // count elements that are greater than value 4
    num = count_if(coll.cbegin(), coll.cend(),   // range
    [](int elem) {               // criterion
        return elem > 4;
    });
    cout << "number of elements greater than 4:  " << num << endl;
}
