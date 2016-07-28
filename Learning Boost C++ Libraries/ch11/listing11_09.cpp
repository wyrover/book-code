#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " ip [port]\n";
        exit(1);
    }

    const char *addr = argv[1];
    unsigned short port = (argc > 2) ? atoi(argv[2]) : 0;

    try {
        asio::io_service service;
        asio::ip::tcp::endpoint ep(
            asio::ip::address::from_string(addr), port);
        asio::ip::tcp::resolver resolver(service);
        asio::ip::tcp::resolver::iterator iter =
            resolver.resolve(ep), end;

        while (iter != end) {
            std::cout << iter->host_name() << " "
                      << iter->service_name() << '\n';
            iter++;
        }
    } catch (std::exception& ex) {
        std::cerr << ex.what() << '\n';
    }
}

