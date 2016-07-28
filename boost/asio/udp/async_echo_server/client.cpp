#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

int main()
{
	io_service svc;
	ip::udp::socket sock(svc);
	try
	{
		sock.open(ip::udp::v4());
		ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8888);
		sock.send_to(buffer("testing"), ep);
	}
	catch (const boost::system::system_error& e)
	{
		std::cout << e.what() << ":" << e.code() << std::endl;
	}
	return 0;
}

