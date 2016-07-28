#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port\n";
        exit(1);
    }

    const char *host = argv[1], *port = argv[2];
    asio::io_service service;
    asio::ip::tcp::resolver resolver(service);

    try {
        asio::ip::tcp::resolver::query query(asio::ip::tcp::v4(),
                                             host, port);
        asio::ip::tcp::resolver::iterator end,
             iter = resolver.resolve(query);
        asio::ip::tcp::endpoint server(iter->endpoint());
        std::cout << "Connecting to " << server << '\n';
        asio::ip::tcp::socket socket(service,
                                     asio::ip::tcp::v4());
        socket.connect(server);
        std::string message = "Hello from client";
        asio::write(socket, asio::buffer(message.c_str(),
                                         message.size()));
        socket.shutdown(asio::ip::tcp::socket::shutdown_send);
        char msg[BUFSIZ];
        boost::system::error_code ec;
        size_t sz = asio::read(socket,
                               asio::buffer(msg, BUFSIZ), ec);

        if (!ec || ec == asio::error::eof) {
            msg[sz] = 0;
            std::cout << "Received: " << msg << '\n';
        } else {
            std::cerr << "Error reading response from server: "
                      << ec.message() << '\n';
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

