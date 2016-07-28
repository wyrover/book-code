#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <cstring>

namespace asio = boost::asio;
typedef boost::shared_ptr<asio::ip::tcp::socket> socketptr;

void handle_connection(asio::yield_context yield,
                       socketptr socket)
{
    asio::io_service& service = socket->get_io_service();
    char msg[BUFSIZ];
    msg[0] = '\0';
    boost::system::error_code ec;
    const char *resp = "Hello from server";
    size_t size = asio::async_read(*socket,
                                   asio::buffer(msg, BUFSIZ), yield[ec]);

    if (!ec || ec == asio::error::eof) {
        msg[size] = '\0';
        boost::array<asio::const_buffer, 2> bufseq;
        bufseq[0] = asio::const_buffer(resp, ::strlen(resp));
        bufseq[1] = asio::const_buffer(msg, size);
        asio::async_write(*socket, bufseq, yield[ec]);

        if (ec) {
            std::cerr << "Error sending response to client: "
                      << ec.message() << '\n';
        }
    } else {
        std::cout << ec.message() << '\n';
    }
}

void accept_connections(asio::yield_context yield,
                        asio::io_service& service,
                        unsigned short port)
{
    asio::ip::tcp::endpoint server_endpoint(asio::ip::tcp::v4(),
                                            port);
    asio::ip::tcp::acceptor acceptor(service, server_endpoint);

    while (true) {
        auto socket =
            boost::make_shared<asio::ip::tcp::socket>(service);
        acceptor.async_accept(*socket, yield);
        std::cout << "Handling request from client\n";
        spawn(service, boost::bind(handle_connection, ::_1,
                                   socket));
    }
}

int main()
{
    asio::io_service service;
    spawn(service, boost::bind(accept_connections, ::_1,
                               boost::ref(service), 56000));
    service.run();
}

