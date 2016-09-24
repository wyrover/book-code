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
#include <queue>
#include <string>
using namespace std;

int main()
{
    queue<string> q;
    // insert three elements into the queue
    q.push("These ");
    q.push("are ");
    q.push("more than ");
    // read and print two elements from the queue
    cout << q.front();
    q.pop();
    cout << q.front();
    q.pop();
    // insert two new elements
    q.push("four ");
    q.push("words!");
    // skip one element
    q.pop();
    // read and print two elements
    cout << q.front();
    q.pop();
    cout << q.front() << endl;
    q.pop();
    // print number of elements in the queue
    cout << "number of elements in the queue: " << q.size()
         << endl;
}
