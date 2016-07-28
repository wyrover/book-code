#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>

using namespace boost::asio;

int main(int argc, char *argv[])
{
   if (argc != 2) {
     std::cout << "Usage : sync_server port" << endl;
     return 1;
   }

   typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
   io_service service;
   ip::tcp::endpoint ep( ip::tcp::v4(), atoi(argv[1])); // listen on 2001
   ip::tcp::acceptor acc(service, ep);
   socket_ptr sock(new ip::tcp::socket(service));
   start_accept(sock);
   service.run();
}

void start_accept(socket_ptr sock) {
   acc.async_accept(*sock,  boost::bind( handle_accept, sock, _1) );
}

void handle_accept(socket_ptr sock, const boost::system::error_code &
   err) {
   if ( err) return;
   //at this point, you can read/write to the socket
   socket_ptr sock(new ip::tcp::socket(service));
   start_accept(sock);
}
