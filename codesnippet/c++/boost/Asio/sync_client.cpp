#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
  using namespace boost::asio;
  io_service service;
  ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 1024);
  ip::tcp::socket sock(service);
  sock.connect(ep);

  return 0;
}
