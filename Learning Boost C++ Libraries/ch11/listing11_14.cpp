#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <iostream>

void print_world(const boost::system::error_code& ec)
{
    std::cout << "world!\n";
}

void print_hello(boost::asio::deadline_timer& timer,
                 const boost::system::error_code& ec)
{
    std::cout << "Hello, " << std::flush;
    timer.expires_from_now(boost::posix_time::seconds(10));
    timer.async_wait(print_world);
}

int main()
{
    boost::asio::io_service service;
    boost::asio::deadline_timer timer(service);
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait(boost::bind(print_hello, boost::ref(timer),
                                 ::_1));
    service.run();
}

