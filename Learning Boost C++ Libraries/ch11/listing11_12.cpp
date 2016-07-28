#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <iostream>

int main()
{
    boost::asio::io_service service;
    boost::asio::deadline_timer timer(service);
    long secs = 5;
    std::cout << "Waiting for " << secs << " seconds ..."
              << std::flush;
    timer.expires_from_now(boost::posix_time::seconds(secs));
    timer.wait();
    std::cout << " done\n";
}

