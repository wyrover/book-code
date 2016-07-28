#include <boost/asio.hpp>
#include <stdio.h>

using namespace boost::asio;

int main()
{
	io_service svc;
	ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 8888);
	ip::tcp::socket sock(svc);
	sock.connect(ep);
	sock.send(buffer("Hello asio!", 11));

	char data[256];
	size_t len = sock.read_some(buffer(data));
	if (len > 0)
	{
		data[len] = '\0';
		printf("recv: %s\n", data);
	}
	sock.close();
	
	return 0;
}

