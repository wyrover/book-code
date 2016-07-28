#ifndef ASYNCONN_HPP
#define ASYNCONN_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#define MAXBUF 1024

namespace asio = boost::asio;
namespace sys = boost::system;

class TCPAsyncConnection
    : public boost::enable_shared_from_this<TCPAsyncConnection>
{
public:
    TCPAsyncConnection(asio::io_service& service) :
        socket(service) {}

    asio::ip::tcp::socket& getSocket()
    {
        return socket;
    }

    void waitForReceive()
    {
        auto thisPtr = shared_from_this();
        socket.async_read_some(asio::buffer(buf, sizeof(buf)),
        [thisPtr](const sys::error_code & ec, size_t sz) {
            if (ec) {
                if (ec == asio::error::eof) {
                    thisPtr->socket.close();
                }

                std::cerr << "Error receiving data from "
                          "client: " << ec.message() << "\n";
            } else {
                thisPtr->startSend();
                std::cout << thisPtr->buf << '\n';
                thisPtr->waitForReceive();
            }
        });
    }

    void startSend()
    {
        const char *msg = "Hello from server";
        auto thisPtr = shared_from_this();
        async_write(socket, asio::buffer(msg, strlen(msg)),
        [thisPtr](const sys::error_code & ec, size_t sz) {
            if (ec) {
                if (ec == asio::error::eof) {
                    thisPtr->socket.close();
                }

                std::cerr << "Failed to send response to "
                          "client: " << ec.message() << '\n';
            }
        });
    }

private:
    asio::ip::tcp::socket socket;
    char buf[MAXBUF];
};

#endif /* ASYNCONN_HPP */
