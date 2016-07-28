#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;

int main()
{
    asio::io_service service;
    service.post(
    [] {
        std::cout << "Hello, world!" << '\n';
    });
    std::cout << "Greetings: \n";
    service.run();
}

