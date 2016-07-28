#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

int main()
{
	io_service svc;
	udp::socket sock(svc, udp::endpoint(udp::v4(), 8888));
	char buf[1024];
	while (true)
	{
		udp::endpoint sender_ep;
		size_t bytes = sock.receive_from(buffer(buf), sender_ep);
		std::string msg(buf, bytes);
		sock.send_to(buffer(msg), sender_ep);
	}
	return 0;
}

