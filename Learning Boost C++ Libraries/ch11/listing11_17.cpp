#include <boost/asio.hpp>
#include <exception>
#include <iostream>
namespace asio = boost::asio;

int main()
{
    const unsigned short port = 55000;
    const std::string greet("Hello, world!");
    asio::io_service service;
    asio::ip::udp::endpoint endpoint(asio::ip::udp::v4(), port);
    asio::ip::udp::socket socket(service, endpoint);
    asio::ip::udp::endpoint ep;

    while (true) try {
            char msg[256];
            auto recvd = socket.receive_from(asio::buffer(msg,
                                             sizeof(msg)), ep);
            msg[recvd] = 0;
            std::cout << "Received: [" << msg << "] from ["
                      << ep << "]\n";
            socket.send_to(asio::buffer(greet.c_str(), greet.size()),
                           ep);
            socket.send_to(asio::buffer(msg, strlen(msg)), ep);
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
        }
}

