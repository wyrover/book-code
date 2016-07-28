#ifndef ASYNCSVR_HPP
#define ASYNCSVR_HPP
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <iostream>
#include "asynconn.hpp"

namespace asio = boost::asio;
namespace sys = boost::system;
typedef boost::shared_ptr<TCPAsyncConnection>
TCPAsyncConnectionPtr;

class TCPAsyncServer
{
public:
    TCPAsyncServer(asio::io_service& service, unsigned short p)
        : acceptor(service,
                   asio::ip::tcp::endpoint(
                       asio::ip::tcp::v4(), p))
    {
        waitForConnection();
    }

    void waitForConnection()
    {
        TCPAsyncConnectionPtr connectionPtr = boost::make_shared
                                              <TCPAsyncConnection>(acceptor.get_io_service());
        acceptor.async_accept(connectionPtr->getSocket(),
        [this, connectionPtr](const sys::error_code & ec) {
            if (ec) {
                std::cerr << "Failed to accept connection: "
                          << ec.message() << "\n";
            } else {
                connectionPtr->waitForReceive();
                waitForConnection();
            }
        });
    }

private:
    asio::ip::tcp::acceptor acceptor;
};

#endif /* ASYNCSVR_HPP */

