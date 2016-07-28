#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <iostream>
namespace asio = boost::asio;

typedef boost::shared_ptr<asio::ip::tcp::socket> socket_ptr;

int main()
{
    const unsigned short port = 56000;
    asio::io_service service;
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
    asio::ip::tcp::acceptor acceptor(service, endpoint);

    while (true) {
        socket_ptr socket(new asio::ip::tcp::socket(service));
        acceptor.accept(*socket);
        boost::thread([socket]() {
            std::cout << "Service request from "
                      << socket->remote_endpoint() << '\n';
            boost::array<asio::const_buffer, 2> bufseq;
            const char *msg = "Hello, world!";
            const char *msg2 = "What's up?";
            bufseq[0] = asio::const_buffer(msg, strlen(msg));
            bufseq[1] = asio::const_buffer(msg2, strlen(msg2));

            try {
                boost::system::error_code ec;
                char recvbuf[BUFSIZ];
                auto sz = read(*socket, asio::buffer(recvbuf,
                                                     BUFSIZ), ec);

                if (!ec || ec == asio::error::eof) {
                    recvbuf[sz] = 0;
                    std::cout << "Received: " << recvbuf << " from "
                              << socket->remote_endpoint() << '\n';
                    write(*socket, bufseq);
                    socket->close();
                }
            } catch (std::exception& e) {
                std::cout << "Error encountered: " << e.what() << '\n';
            }
        });
    }
}

