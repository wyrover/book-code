#include <boost/asio.hpp>
#include <iostream>
#include <exception>
namespace asio = boost::asio;

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port\n";
        return 1;
    }

    asio::io_service service;

    try {
        asio::ip::udp::resolver::query query(asio::ip::udp::v4(),
                                             argv[1], argv[2]);
        asio::ip::udp::resolver resolver(service);
        auto iter = resolver.resolve(query);
        asio::ip::udp::endpoint endpoint = iter->endpoint();
        asio::ip::udp::socket socket(service,
                                     asio::ip::udp::v4());
        const char *msg = "Hello from client";
        socket.send_to(asio::buffer(msg, strlen(msg)), endpoint);
        char buffer[256];
        size_t recvd = socket.receive_from(asio::buffer(buffer,
                                           sizeof(buffer)), endpoint);
        buffer[recvd] = 0;
        std::cout << "Received " << buffer << " from "
                  << endpoint.address() << ':' << endpoint.port() << '\n';
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

