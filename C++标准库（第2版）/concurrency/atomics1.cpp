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
#include <atomic>    // for atomics
#include <future>    // for async() and futures
#include <thread>    // for this_thread
#include <chrono>    // for durations
#include <iostream>

long data;
std::atomic<bool> readyFlag(false);

void provider()
{
    // after reading a character
    std::cout << "<return>" << std::endl;
    std::cin.get();
    // provide some data
    data = 42;
    // and signal readiness
    readyFlag.store(true);
}

void consumer()
{
    // wait for readiness and do something else
    while (!readyFlag.load()) {
        std::cout.put('.').flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // and process provided data
    std::cout << "\nvalue : " << data << std::endl;
}

int main()
{
    // start provider and consumer
    auto p = std::async(std::launch::async, provider);
    auto c = std::async(std::launch::async, consumer);
}
