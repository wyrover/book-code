#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <iostream>

void on_timer_expiry(const boost::system::error_code& ec)
{
    if (ec) {
        std::cout << "Error occurred while waiting\n";
    } else {
        std::cout << "Timer expired\n";
    }
}

int main()
{
    boost::asio::io_service service;
    boost::asio::deadline_timer timer(service);
    long secs = 5;
    timer.expires_from_now(boost::posix_time::seconds(secs));
    std::cout << "Before calling deadline_timer::async_wait\n";
    timer.async_wait(on_timer_expiry);
    std::cout << "After calling deadline_timer::async_wait\n";
    service.run();
}
