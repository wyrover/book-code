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
#include <mutex>
#include <iostream>
#include <string>

std::mutex printMutex;  // enable synchronized output with print()

void print(const std::string& s)
{
    std::lock_guard<std::mutex> l(printMutex);

    for (char c : s) {
        std::cout.put(c);
    }

    std::cout << std::endl;
}

int main()
{
    auto f1 = std::async(std::launch::async,
                         print, "Hello from a first thread");
    auto f2 = std::async(std::launch::async,
                         print, "Hello from a second thread");
    print("Hello from the main thread");
}
