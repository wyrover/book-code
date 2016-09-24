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
#include <future>
#include <list>
#include <iostream>
#include <exception>
using namespace std;

void task1()
{
    // endless insertion and memory allocation
    // - will sooner or later raise an exception
    // - BEWARE: this is bad practice
    list<int> v;

    while (true) {
        for (int i = 0; i < 1000000; ++i) {
            v.push_back(i);
        }

        cout.put('.').flush();
    }
}

int main()
{
    cout << "starting 2 tasks" << endl;
    cout << "- task1: process endless loop of memory consumption" << endl;
    cout << "- task2: wait for <return> and then for task1" << endl;
    auto f1 = async(task1);  // start task1() asynchronously (now or later or never)
    cin.get();  // read a character (like getchar())
    cout << "\nwait for the end of task1: " << endl;

    try {
        f1.get();  // wait for task1() to finish (raises exception if any)
    } catch (const exception& e) {
        cerr << "EXCEPTION: " << e.what() << endl;
    }
}
