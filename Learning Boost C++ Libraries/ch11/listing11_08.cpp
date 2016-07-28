#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " host [service]\n";
        exit(1);
    }

    const char *host = argv[1];
    const char *svc = (argc > 2) ? argv[2] : "";

    try {
        asio::io_service service;
        asio::ip::tcp::resolver resolver(service);
        asio::ip::tcp::resolver::query query(host, svc);
        asio::ip::tcp::resolver::iterator end,
             iter = resolver.resolve(query);

        while (iter != end) {
            asio::ip::tcp::endpoint endpoint = iter->endpoint();
            std::cout << "Address: " << endpoint.address()
                      << ", Port: " << endpoint.port() << '\n';
            ++iter;
        }
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}

