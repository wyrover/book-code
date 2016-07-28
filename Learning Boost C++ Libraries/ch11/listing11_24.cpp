#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include "asyncsvr.hpp"
#define MAXBUF 1024
namespace asio = boost::asio;

int main()
{
    try {
        asio::io_service service;
        TCPAsyncServer server(service, 56000);
        service.run();
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}

