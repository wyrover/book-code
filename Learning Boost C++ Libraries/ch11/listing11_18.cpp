#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;
namespace sys = boost::system;

const size_t MAXBUF = 256;

class UDPAsyncServer
{
public:
    UDPAsyncServer(asio::io_service& service,
                   unsigned short port)
        : socket(service,
                 asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
        waitForReceive();
    }

    void waitForReceive()
    {
        socket.async_receive_from(asio::buffer(buffer, MAXBUF),
                                  remote_peer,
                                  [this](const sys::error_code & ec,
        size_t sz) {
            const char *msg = "hello from server";
            std::cout << "Received: [" << buffer << "] "
                      << remote_peer << '\n';
            waitForReceive();
            socket.async_send_to(
                asio::buffer(msg, strlen(msg)),
                remote_peer,
                [this](const sys::error_code & ec,
            size_t sz) {});
        });
    }

private:
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint remote_peer;
    char buffer[MAXBUF];
};

int main()
{
    asio::io_service service;
    UDPAsyncServer server(service, 55000);
    service.run();
}

