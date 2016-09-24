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
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cstdlib>
using namespace std;

int main()
{
    // process and print current date and time:
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    tm* nowTM = localtime(&t);
    cout << put_time(nowTM, "date: %x\ntime: %X\n") << endl;
    // read new time (same date)
    tm time(*nowTM);                 // copy date for new time
    cout << "new time [HH:MM]: ";
    cin >> get_time(&time, "%H:%M"); // read new time

    if (!cin) {
        cerr << "invalid format read" << endl;
        exit(EXIT_FAILURE);
    }

    // process difference in minutes:
    auto tp = chrono::system_clock::from_time_t(mktime(&time));
    auto diff = chrono::duration_cast<chrono::minutes>(tp - now);
    cout << "difference: " << diff.count() << " minutes" << endl;
}
