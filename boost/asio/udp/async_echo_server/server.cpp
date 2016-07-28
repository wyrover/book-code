#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

io_service svc;
ip::udp::socket sock(svc);
ip::udp::endpoint sender_ep;
char buf[512];

void on_read(const boost::system::error_code& ec, std::size_t read_bytes)
{
	std::cout << "read " << read_bytes << " bytes" << std::endl;
	buf[read_bytes] = '\0';
	std::cout << buf << std::endl;
	sock.async_receive_from(buffer(buf), sender_ep, on_read);
}

int main()
{
	ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8888);
	sock.open(ep.protocol());
	sock.set_option(ip::udp::socket::reuse_address(true));
	sock.bind(ep);
	sock.async_receive_from(buffer(buf, sizeof(buf)-1), sender_ep, on_read);

	svc.run();
	return 0;
}

