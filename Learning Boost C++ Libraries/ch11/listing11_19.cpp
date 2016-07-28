#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <iostream>
namespace asio = boost::asio;
namespace sys = boost::system;

const size_t MAXBUF = 256;
typedef boost::shared_ptr<asio::ip::udp::socket>
shared_udp_socket;

void udp_send_to(boost::asio::yield_context yield,
                 shared_udp_socket socket,
                 asio::ip::udp::endpoint peer)
{
    const char *msg = "hello from server";
    socket->async_send_to(asio::buffer(msg, std::strlen(msg)),
                          peer, yield);
}

void udp_server(boost::asio::yield_context yield,
                asio::io_service& service,
                unsigned short port)
{
    shared_udp_socket socket =
        boost::make_shared<asio::ip::udp::socket>(service,
                asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
    char buffer[MAXBUF];
    asio::ip::udp::endpoint remote_peer;
    boost::system::error_code ec;

    while (true) {
        socket->async_receive_from(asio::buffer(buffer, MAXBUF),
                                   remote_peer, yield[ec]);

        if (!ec) {
            spawn(socket->get_io_service(),
                  boost::bind(udp_send_to, ::_1, socket,
                              remote_peer));
        }
    }
}

int main()
{
    asio::io_service service;
    spawn(service, boost::bind(udp_server, ::_1,
                               boost::ref(service), 55000));
    service.run();
}

