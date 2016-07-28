#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <iostream>

void wait_and_print(boost::asio::yield_context yield,
                    boost::asio::io_service& service)
{
    boost::asio::deadline_timer timer(service);
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait(yield);
    std::cout << "Hello, " << std::flush;
    timer.expires_from_now(boost::posix_time::seconds(10));
    timer.async_wait(yield);
    std::cout << "world!\n";
}

int main()
{
    boost::asio::io_service service;
    boost::asio::spawn(service,
                       boost::bind(wait_and_print, ::_1,
                                   boost::ref(service)));
    service.run();
}

