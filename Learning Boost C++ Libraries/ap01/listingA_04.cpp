#include <boost/exception_ptr.hpp>
#include <iostream>

void do_work()
{
    throw boost::enable_current_exception(
        std::runtime_error("Exception in do_work"));
}

void do_more_work()
{
    boost::exception_ptr eptr;

    try {
        do_work();
    } catch (...) {
        eptr = boost::current_exception();
    }

    boost::rethrow_exception(eptr);
}

int main()
{
    try {
        do_more_work();
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}

