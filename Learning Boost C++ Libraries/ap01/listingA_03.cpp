#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

void do_work()
{
    throw std::runtime_error("Exception in do_work");
}

std::vector<std::exception_ptr> exceptions;

void do_more_work()
{
    std::exception_ptr eptr;

    try {
        do_work();
    } catch (...) {
        eptr = std::current_exception();
    }

    std::exception_ptr eptr2(eptr);
    exceptions.push_back(eptr);
    exceptions.push_back(eptr2);
}

int main()
{
    do_more_work();

    for (auto& eptr : exceptions) {
        try {
            std::rethrow_exception(eptr);
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
        }
    }
}

