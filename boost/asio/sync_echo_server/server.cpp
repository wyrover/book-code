#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock)
{
	while (true)
	{
		char data[256];
		boost::system::error_code ec;
		size_t len = sock->read_some(buffer(data), ec);
		if (!ec)  // equals "if (ec.value() != boost::system::errc::success)"
		{
			std::cerr << "err: " << ec.value() << ", err msg: " << ec.message() << std::endl;
			return;
		}

		if (len > 0)
		{
			std::cout << "echo: " << std::string(data, len) << std::endl;
			write(*sock, buffer(data, len));
		}
	}
}

int main()
{
	io_service svc;
	ip::tcp::endpoint ep(ip::tcp::v4(), 8888);
	ip::tcp::acceptor acc(svc, ep);
	while (true)
	{
		socket_ptr sock(new ip::tcp::socket(svc));
		acc.accept(*sock);
		boost::thread(boost::bind(client_session, sock));
	}
	return 0;
}

